#ifndef DETECTOR_H
#define DETECTOR_H

#include "point.h"
#include <qgraphicsscene.h>
#include "plan.h"
#include <QTime>

using namespace Utils;

class Detector
{
public:
    Detector(QString name_, Plan::Road* r, bool head_, qreal where_, qreal proximity);
    ~Detector();

    bool detected(Point v);
    Point draw(QGraphicsScene* scene);
    void log();
    Point getPosition();
    QTime getTime();
    void unset();

    QString name;
    QTime time;

private:
    QPixmap detectorIcon;
    Point position;
    Plan::Road* road;
    bool head;    // head or tail of the road
    qreal where;  // proportion relatively to the overall road
    bool status;
    bool horizontal; // direction: horizontal/vertical
    qreal proximiy;
};

#endif // DETECTOR_H
