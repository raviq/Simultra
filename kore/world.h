#ifndef WORLD_H
#define WORLD_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <vector>

#include "readosm.h"
#include "osmdata.h"

using namespace std;

class World : public QGraphicsItem
{
public:
    World();

    osm_world_t getWorld();
    QPainterPath getPath(int index);
    QVector<QPainterPath> getPaths();

    // OSM MAP
    int render(QGraphicsScene* scene);

    // GUI
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getName();
    void setName(QString _name);
    void setView(QGraphicsView* _view);

    QPointF convert(double lat, double lon, double minLat, double maxLat, double minLon, double maxLon);

    void plotMarker(QGraphicsScene* scene, double x, double y, QColor colour);

    qreal get_minLat();
    qreal get_minLon();
    qreal get_maxLat();
    qreal get_maxLon();

protected:
    void moveSomething(int step);

private:
    QVector<QPainterPath> _paths;

    qreal alpha;
    qreal angle;
    qreal speed;
    qreal eyeDirection;
    QColor colour;
    QString name;
    QPointF position;
    QGraphicsView* view;
    int index;
    osm_world_t* _world;
    qreal _minLat, _minLon, _maxLat, _maxLon;
};

#endif // WORLD_H
