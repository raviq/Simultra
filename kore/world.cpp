//
// Simulation world
//

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QTime>
#include <iostream>
#include <limits>
#include <QGraphicsPathItem>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <QTemporaryDir>

#include "readosm.h"
#include "parser.h"
#include "world.h"
#include "utils.h"
#include "config.h"
#include "debug.h"

using namespace std;

World::World()
{
    index = 0;
    alpha = 0.0;
    colour = Qt::red;
    _world = new osm_world_t;
    load_map(_OSM_MAP_, _world);

    UNUSED(angle);
    UNUSED(speed);
    UNUSED(eyeDirection);
}

qreal World::get_minLat()
{
    return _minLat;
}
qreal World::get_minLon()
{
    return _minLon;
}

qreal World::get_maxLat()
{
    return _maxLat;
}

qreal World::get_maxLon()
{
    return _maxLon;
}

QVector<QPainterPath> World::getPaths()
{
    return _paths;
}

QPainterPath  World::getPath(int index)
{
    if ( index >= 0  && index < _paths.size())
    {
        return _paths[index];
    }
    else
    {
        perror(" Error: paths number.");
        ::exit(0);
    }
}

osm_world_t World::getWorld()
{
    return *_world;
}

void World::plotMarker(QGraphicsScene* scene, double x, double y, QColor colour)
{
    scene->addEllipse(x, y, 20, 20, QPen(colour));
}

// conversion (lat, lon) -> (x, y)
QPointF World::convert(double lat, double lon, double minLat, double maxLat, double minLon, double maxLon)
{
    return QPointF(((lat - minLat) / (maxLat - minLat)) * (HEIGHT - 1),
                    ((lon - minLon) / (maxLon - minLon)) * (WIDTH - 1));
}

void PolygonCentroid(QPolygonF polygon, QPointF* c)
{

    QPointF centroid(0, 0);
    double signedArea = 0.0;
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double  a = 0.0;  // Partial signed area
    int vertexCount = polygon.size();

    // For all vertices
    for (int i=0 ; i < vertexCount-1 ; ++i)
    {

        x0 = polygon.at(i).x();
        y0 = polygon.at(i).y();
        x1 = polygon.at((i+1) % vertexCount).x();
        y1 = polygon.at((i+1) % vertexCount).y();
        a = x0 * y1 - x1 * y0;
        signedArea += a;
        centroid.setX(centroid.x() + (x0 + x1)*a);
        centroid.setY(centroid.y() + (y0 + y1)*a);
    }

    signedArea *= 0.5;
    centroid.setX(centroid.x() / (6.0*signedArea));
    centroid.setY(centroid.y() / (6.0*signedArea));

    c->setX(centroid.x());
    c->setY(centroid.y());
}

// Construct and render world scene.

int World::render(QGraphicsScene* scene)
{
    QRectF textRectangle(100, 200, 11, 16);
    QPainterPath path;
    QPointF begin = QPointF(100, 100);
    QPointF end = QPointF(200, 105);
    QLineF rectBase = QLineF(begin, end);
    qreal angle = rectBase.angle(QLineF(QPointF(0, 0), QPointF(1, 0)));
    QMatrix rotationMatrix;
    rotationMatrix.translate(begin.x(), begin.y());
    rotationMatrix.rotate(angle);
    path.addPolygon(rotationMatrix.map(QPolygonF(textRectangle)));

    // min/max lat/lon of all nodes.
    _minLat = 100000.0;
    _minLon = 100000.0;
    _maxLat = -100000.0;
    _maxLon = -100000.0;

    debug("World size: %lu", _world->nodes.size());

    for (uint i = 0 ; i < _world->nodes.size() ; i++ )
    {
        if (_world->nodes[i].latitude < _minLat)    _minLat = _world->nodes[i].latitude;
        if (_world->nodes[i].latitude > _maxLat)    _maxLat = _world->nodes[i].latitude;
        if (_world->nodes[i].longitude < _minLon)   _minLon = _world->nodes[i].longitude;
        if (_world->nodes[i].longitude > _maxLon)   _maxLon = _world->nodes[i].longitude;
    }

    // plot the min/max Lat/Lon rectangle
    QPointF p1 = convert(_minLat, _minLon, _minLat, _maxLat, _minLon, _maxLon);
    QPointF p2 = convert(_maxLat, _maxLon, _minLat, _maxLat, _minLon, _maxLon);
    scene->addRect(QRect(p1.x(), p1.y(), p2.x(), p2.y()), QPen(Qt::green, 0.1));
    // then,
    // Label the green rectangle
    QFont font_;
    font_.setFamily("Helvetica");
    font_.setItalic(true);
    font_.setPointSize(14);
    QPainterPath p_;
    p_.addText( QPointF(p2.x()-800, p1.y()-900), font_, "P h y s i c s    E n g i n e   :   S i m u l a t i o n    G l o b a l    R e f e r e n t i a l");
    QGraphicsPathItem* SimulationArea_ = new QGraphicsPathItem;
    SimulationArea_->setPath(p_);
    SimulationArea_->setRotation(85);
    SimulationArea_->setZValue(1);
    SimulationArea_->setPen(QPen(Qt::green, 0.1));
    scene->addItem(SimulationArea_);

    #if _TRACE_
    cout << " H = " << view->height() << "  W = " << view->width() << endl;
    cout << "Nodes done." << endl;
    cout << "###################################################\n";
    cout << " #ways = " << _world->ways.size() << endl;
    cout << "###################################################\n";
    #endif

    int id;

    for (uint i = 0 ; i < _world->ways.size() ; i++ )
    {
        id = _world->ways[i].id;

        #if _TRACE_
        cout << "way id = " << _world->ways[i].id << endl;
        cout << "Tags:  tag_count = " << _world->ways[i].tag_count << endl;
        #endif

        // no tags
        if (_world->ways[i].tag_count == 0)
            continue;

        QString source_ref    = QString();
        QString source_ja     = QString();
        QString source        = QString();
        QString source_oneway = QString();
        QString name          = QString();
        QString name_en  = QString();
        QString highway  = QString();
        QString building = QString();
        QString amenity  = QString();
        QString note     = QString();
        QString parking  = QString();
        QString tourism  = QString();

        QVector<QPointF> polyPoints_highway;
        QVector<QPoint> polyPoints_building;

        // Populate

        // tags loop {
        for (int j = 0 ; j < _world->ways[i].tag_count ; j++)
        {
            if ( !strcmp( _world->ways[i].tags[j].key, "note") )
            {
                note =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "tourism") )
            {
                tourism =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "parking") )
            {
                 parking =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "amenity") )
            {
                amenity =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "source_ref") )
            {
                source_ref =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "source:ja") )
            {
                source_ja =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "source") )
            {
                source =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "oneway") )
            {
                source_oneway =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "name:en") )
            {
                name_en =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "name") )
            {
                name =  QString(_world->ways[i].tags[j].value);
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "highway") )
            {
                highway =  QString(_world->ways[i].tags[j].value);

                for (int y = 0 ; y < _world->ways[i].node_ref_count - 1 ; y++)
                {
                    #if _TRACE_
                    cout << " \t\t node id = " << _world->ways[i].node_refs[y] << endl;
                    #endif
                    for (uint k = 0 ; k < _world->nodes.size() ; k++)
                    {
                        if ( _world->nodes[k].id == _world->ways[i].node_refs[y])
                        {
                            #if _TRACE_
                            cout << " \t\t\t latitude  = " << fixed << _world->nodes[k].latitude << endl;
                            cout << " \t\t\t longitude = " << fixed << _world->nodes[k].longitude << endl;
                            #endif

                            // add point to polypoint/polygon
                            QPointF point = convert(
                                        _world->nodes[k].latitude,
                                        _world->nodes[k].longitude,
                                        _minLat, _maxLat, _minLon, _maxLon);

                            #if HIGHWAY_POINTS
                            double radius = 0.5;
                            scene->addEllipse(point.x(), point.y(), radius, radius, QPen(Qt::green), QBrush(Qt::green));
                            #endif
                            polyPoints_highway << point;
                        }
                    }
                }
            }
            else
            if ( !strcmp( _world->ways[i].tags[j].key, "building") )
            {
                building =  QString(_world->ways[i].tags[j].value);
                for (int x = 0 ; x < _world->ways[i].node_ref_count ; x++)
                {
                    #if _TRACE_
                    cout << " \t\t node id = " << _world->ways[i].node_refs[x] << endl;
                    #endif

                    for (uint k = 0 ; k < _world->nodes.size() ; k++)
                    {
                        if ( _world->nodes[k].id == _world->ways[i].node_refs[x])
                        {
                            #if _TRACE_
                            cout << " \t\t\t latitude  = " << _world->nodes[k].latitude << endl;
                            cout << " \t\t\t longitude = " << _world->nodes[k].longitude << endl;
                            #endif
                            QPointF point = convert(_world->nodes[k].latitude, _world->nodes[k].longitude, _minLat, _maxLat, _minLon, _maxLon);
                            #if RENDER_BUILDINGS_NODES
                            double radius = 1.0;
                            scene->addEllipse(point.x(), point.y(), radius, radius, QPen(Qt::blue), QBrush(Qt::blue));
                            #endif
                            polyPoints_building << QPoint(point.x(), point.y());
                        }
                    }
                }
            }
            else
            {
               #if _TRACE_
               cout << "\t\t" << _world->ways[i].tags[j].key << ": " << _world->ways[i].tags[j].value << endl;
               #endif
            }

        }
        // tags loop }

        // Drawing

        #if _TRACE_
        cout << "   highway? :  " << highway.toStdString() << endl;
        cout << "  building? :  " << building.toStdString() << endl;
        cout << " source_ref :  " << source_ref.toStdString()  << endl;
        cout << " source_ja  :  " <<  source_ja.toStdString() << endl;
        cout << "   source   :  " <<  source.toStdString() << endl;
        cout << "  oneway?   :  " << source_oneway.toStdString() << endl;
        cout << "     name   :  " << name.toStdString() << endl;
        cout << "  name_en   :  " << name_en.toStdString() << endl;
        cout << "  amenity   :  " << amenity.toStdString() << endl;
        cout << "  note      :  " << note.toStdString() << endl;
        cout << "  parking   :  " << parking.toStdString() << endl;
        cout << "  tourism   :  " << tourism.toStdString() << endl;
        #endif

        if ( !highway.isEmpty() )
        {
            // building the polygon
            QPolygonF polygon_highway;

            for(int i = polyPoints_highway.size()-1; i > 0; i--)
                polygon_highway.append(polyPoints_highway[i]);

            QPainterPath path; // polygon <- path
            path.addPolygon(polygon_highway);

            #if DRAW_LINES
            QGraphicsPathItem* contour = new QGraphicsPathItem;
            contour->setPen(QPen( Qt::gray, 8));
            contour->setOpacity(0.01);
            contour->setToolTip(name_en);
            contour->setPath(path);
            scene->addItem(contour);
            #endif

            // adding the road name
            #if ROUTE_NAMES
            QFont font;
            font.setFamily("arial");
            font.setPointSize(ROUTE_NAME_SIZE);
            int a = polyPoints_highway.size()/2 - 2;    // TODO ctrl over # of elements of polyPoints_highway
            QPainterPath p;
            p.addText( mapToScene(polyPoints_highway[a]), font, name_en );
            QGraphicsPathItem* roadNameItem = new QGraphicsPathItem;
            roadNameItem->setPath(p);
            //roadNameItem->rotate(angle);
            roadNameItem->setZValue(1);
            roadNameItem->setPen(QPen(ROUTE_NAME_COLOR, 0.1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
            roadNameItem->setToolTip(name_en);
            scene->addItem(roadNameItem);
            #endif

            // add 'path' to 'paths'
            _paths.push_back(path);
        }

        if ( !amenity.compare("post_office")  )
        {
            QPolygonF polygon;
            for(int i = polyPoints_building.size()-1; i > 0; i--)
                polygon.append(polyPoints_building[i]);
            QGraphicsPixmapItem * poffice = new QGraphicsPixmapItem;
            poffice->setPixmap(QPixmap(":/images/postoffice"));
            poffice->setPos(polygon.boundingRect().center());
            poffice->setZValue(Utils::LANE_LAYER);
            poffice->setScale(0.01);
        }

        #if 0
        if (!amenity.compare("parking"))
        {

            cout << " in parking()" << endl;

            QPolygonF polygon;
            for(int i = polyPoints_building.size()-1; i > 0; i--)
                polygon.append(polyPoints_building[i]);
            QGraphicsPixmapItem *parkingP(new QGraphicsPixmapItem(QPixmap(":/images/parking"), 0, scene ));
            parkingP->setPos(polygon.boundingRect().center());
            parkingP->setZValue(Utils::LANE_LAYER);
            parkingP->scale(0.2, 0.2);


                cout << "   highway? :  " << highway.toStdString() << endl;
                cout << "  building? :  " << building.toStdString() << endl;
                cout << " source_ref :  " << source_ref.toStdString()  << endl;
                cout << " source_ja  :  " <<  source_ja.toStdString() << endl;
                cout << "   source   :  " <<  source.toStdString() << endl;
                cout << "  oneway?   :  " << source_oneway.toStdString() << endl;
                cout << "     name   :  " << name.toStdString() << endl;
                cout << "  name_en   :  " << name_en.toStdString() << endl;
                cout << "  amenity   :  " << amenity.toStdString() << endl;
                cout << "  note      :  " << note.toStdString() << endl;
                cout << "  parking   :  " << parking.toStdString() << endl;
                cout << "  tourism   :  " << tourism.toStdString() << endl;

        }
        #endif

        if ( !building.isEmpty() )
        {

            #if DRAW_BUILDINGS
            QPolygonF polygon;
            for(int i = polyPoints_building.size()-1; i > 0; i--)
                polygon.append(polyPoints_building[i]);

            QPainterPath path;
            path.addPolygon(polygon);

            // trying to use the centroid. requires transformation?
            //QPointF centroid(0,0);
            //PolygonCentroid(polygon, &centroid);
            //if ( !isnan(centroid.x()) && !isnan(centroid.y()) )
            //     scene->addEllipse(centroid.x(), centroid.y(), 0.1, 0.1, QPen(Qt::red), QBrush(Qt::SolidLine));

            // just drawing the rectangle containing the block
            //scene->addRect(polygon.boundingRect(), QPen(Qt::red), QBrush(Qt::transparent));

            path.addText(polygon.boundingRect().center().x(),
                         polygon.boundingRect().center().y(),
                         QFont("monospace", 1, 1, true),
                         QString::fromLocal8Bit(name.toAscii())
                         );

            QGraphicsPathItem* contour = new QGraphicsPathItem;
            contour->setPath(path);

            if (!amenity.compare("post_office"))
            {
                contour->setBrush(QBrush(Qt::darkRed));
                contour->setPen(QPen(QBrush(Qt::darkRed), 0.1));
            }
            else
            {
                contour->setBrush(QBrush(BUILDING_COLOR));
                contour->setPen(QPen(BUILDING_LINE_COLOR, BUILDING_LINE_THICKNESS));
            }

            contour->setZValue(1);
            contour->setToolTip(QString::fromLocal8Bit(name.toAscii()));
            scene->addItem(contour);
            #endif
        }
    }

    return 0;
}

//   Mobility

void World::setView(QGraphicsView* v)
{
    view = v;
}

void World::setName(QString _name)
{
    name = _name;
}

QString World::getName()
{
    return name;
}

QRectF World::boundingRect() const
{
    return QRect(0, 0, HEIGHT, WIDTH);
}

QPainterPath World::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, HEIGHT, WIDTH);
    return path;
}

// Paint the shape of the agent
void World::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    int radius = 4;
//    painter->setBrush(colour);
//    painter->drawEllipse(position, radius, radius);
//    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkGray : colour);
//    painter->drawEllipse(position, radius, radius);

    // Important, for the collision/contact with other particles (cars, humans, lights, etc.)
    // for (int i = 0 ; i < scene()->collidingItems(this).size() ; i++)
    // cout << "\t\t" << name.toStdString() << " colliding with " << scene()->collidingItems(this)[i] << endl;

      UNUSED(painter);
      UNUSED(option);
      UNUSED(widget);
}

// Monitoring, lights, etc.

void World::moveSomething(int step)
{
    if (!step)
        return;    
}

// End.
