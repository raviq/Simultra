//
// Detectors routines.
// Detects a vehicle passing by some line, adds/stores this event to a file as (t, vehicle).
//

#include <QGraphicsPixmapItem>
#include <iostream>
#include <QTime>
#include "detector.h"
#include "utils.h"
#include "config.h"

using namespace Utils;
using namespace std;

// detector: x1,y1 - x2,y2, QLine-like
// add, or red from XML?
// draw
// >-------------<  or >-------------- or >

Detector::Detector(QString name_, Plan::Road* road_, bool head_, qreal where_,  qreal proximity_)
{
    name = name_;
    where = where_;
    head = head_;
    road = road_;
    status = false;
    proximiy = proximity_;
    detectorIcon = QPixmap(_DETECOR_ICON_);
}

Detector::~Detector()
{
}

Point Detector::getPosition()
{
    return position;
}

QTime Detector::getTime()
{
    return time;
}

bool Detector::detected(Point v)
{
    qreal d = (horizontal) ? abs_(v.x() - position.x()) : abs_(v.y() - position.y());
    if (d <= proximiy)
    {
        status = true;
        time = QTime::currentTime();
      //  detectorIcon = QPixmap(_DETECOR_ICON_);
    }
    return status;
}

void Detector::unset()
{
    status = false;
}

Point Detector::draw(QGraphicsScene* scene)
{
    QGraphicsPixmapItem * detectorItem = new QGraphicsPixmapItem;
    detectorItem->setPixmap(detectorIcon);

    Point first = road->first();
    Point second = road->second();

    qreal jump = 100.0; // TODO fix for the horizontal case too.

    // Vertical
    if ( first.x() == second.x() && first.y() != second.y() )
    {
       horizontal = false;
       position = (head) ?
                   Point(first.x() - road->width()/2, first.y()) :
                   Point(second.x() - road->width()/2, second.y());
       detectorItem->setRotation(90.0);
    }

    // Horizontal
    if ( first.y() == second.y() && first.x() != second.x() )
    {
       horizontal = true;
       position = (head) ? Point(first.x() + jump, first.y() + road->width()/2) :
                           Point(second.x() - jump, second.y() + road->width()/2);
    }
    detectorItem->setPos(position);
    detectorItem->setZValue(Utils::LANE_LAYER);
    detectorItem->setScale(0.006);

    QTransform t = detectorItem->transform();
    t.translate(-detectorItem->pixmap().width()/2, -detectorItem->pixmap().height()/2);
    detectorItem->setTransform(t);
    scene->addItem(detectorItem);
    return position;
}

void Detector::log()
{

}
