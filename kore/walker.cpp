//
// Pedestrian agent.
//

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <math.h>
#include <iostream>
#include <QDebug>
#include <QTime>
#include "walker.h"
#include "utils.h"
#include "config.h"

using namespace std;

Walker::Walker(QColor col, qreal scale) : angle(0), speed(0), eyeDirection(0), name(QString("")), position(200, 300)
{
    epsilon = qrandouble(MIN_EPSILON, MAX_EPSILON) ;
    index = 0;
    alpha = 0.0;
    colour = col;
    _scale = scale;
    setScale(_scale);
    setRotation(qrand() % (360 * 16));
    setZValue(Utils::VEHICLE_LAYER);

    UNUSED(angle);
    UNUSED(speed);
    UNUSED(eyeDirection);
}

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += 2*M_PI;
    while (angle > 2*M_PI)
        angle -= 2*M_PI;
    return angle;
}

void Walker::setView(QGraphicsView* _view)
{
    view = _view;
}

void Walker::setName(QString _name)
{
    name = _name;
}

QString Walker::getName()
{
    return name;
}

QRectF Walker::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust, 36 + adjust, 60 + adjust);
}

QPainterPath Walker::shape() const
{
    QPainterPath path;
    path.addRect(-10, -20, 20, 40);
    return path;
}

void Walker::set_walk(QPointF* array, int size)
{
    _walk = (QPointF*) malloc ( size * sizeof(QPointF) );
    _nwalk = size;
    for (int i = 0 ; i < _nwalk ; i++)
        _walk[i] = array[i];
}

// Paint the shape of the agent.
void Walker::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(colour);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(-10, -20, 20, 40);
}

// Move around block.
void Walker::advance_(int step)
{
    if (!step)
        return;

    if ( index >= _nwalk - 1)
        index = 0;

     if (alpha >= 1.0 )
     {
         QPointF destin = _walk[index + 1];
         setPos(destin.x(), destin.y());
         // We reached the end of the segment,
         index++;       // jump to the next segment
         alpha = 0.0;   // reset alpha
     }
     else
     {
         double dx = qrandouble(0.0, epsilon); // random epsilon
         alpha += dx;

         // Current segment of the path: [origin, destiation]
         QPointF origin = _walk[index];
         QPointF destin = _walk[index + 1];

         qreal x = (1.0-alpha) * origin.x() + alpha * destin.x();
         qreal y = (1.0-alpha) * origin.y() + alpha * destin.y();

         setPos(x, y);
    }
}
// End.
