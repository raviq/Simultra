//
// Road rendereding
//
#include "roadgui.h"
#include <QPen>
#include <iostream>
#include "point.h"
#include "segment.h"
#include "utils.h"
#include "MapGraphics/guts/Conversions.h"

using namespace std;
using namespace Utils;

// Constructor, black road background

RoadGUI::RoadGUI(Plan::Road *road, QGraphicsItem* parent): QGraphicsPathItem(parent), _LanesFirstToSecond(0), _LanesSecondToFirst(0), _RoadWidth(0)
{
  _Road = road;
  setZValue(ROAD_LAYER);
  setPen(QPen(Qt::black));
  setBrush(Qt::black);
}

// Returns a segment |-----<alpha----------beta>------------------|

QLineF getLine(QPointF first, QPointF second, qreal alpha, qreal beta)
{
    QPointF begin( alpha * first.x() + (1-alpha) * second.x(), alpha * first.y() + (1-alpha) * second.y());
    QPointF end( beta * first.x() + (1-beta) * second.x(), beta * first.y() + (1-beta) * second.y());
    return QLineF(begin, end);
}

// Draw the roads (separation lines, borders, etc.)

bool ispedestrianRoad(QString name)
{
   if (!name.compare("PaPb"))
       return true;

   else if (!name.compare("PbPc"))
       return true;
   else if (!name.compare("PcPd"))
       return true;
   else if (!name.compare("PdPe"))
       return true;
   else if (!name.compare("PePx"))
       return true;
   else
     return false;
}

void RoadGUI::drawRoad()
{
    // check if pedestrian or not
    // if ( ispedestrianRoad(_Road->name()) )
    //    cout << " --->  " <<  _Road->name().toStdString() << endl;

   // Create the 3 segments that will delimit the road (middle one, and two borders)

    // if road is walker path, dont draw it.
    if (_Road->name().length() > 0 && islower(_Road->name().toStdString() [0]))
    {
        //cout << "_NameFirst --->" << _Road->name().toStdString()
        //     << "  "
        //     << _Road->name().length() << endl;
        return;
    }

  Segment middleSegment(_First, _Second);
  Segment negativeGap(middleSegment.towardsShift(-_RoadWidth/2)),
          positiveGap(middleSegment.towardsShift(_RoadWidth/2));
// Walkway

  qreal L = middleSegment.toQLineF().length();
  qreal q = _RoadWidth/2;
  qreal a = 1.0; // q/L;
  qreal b = 0.0; // -a;
  Segment wSegment(Point(middleSegment.toQLineF().p1().x() * a + middleSegment.toQLineF().p2().x() * b,
                         middleSegment.toQLineF().p1().y() * a + middleSegment.toQLineF().p2().y() * b),
                   Point(middleSegment.toQLineF().p1().x() * b + middleSegment.toQLineF().p2().x() * a,
                         middleSegment.toQLineF().p1().y() * b + middleSegment.toQLineF().p2().y() * a));

  UNUSED(L);
  UNUSED(q);
  UNUSED(wSegment);

  // Definition of QPainterPath which will contain the following drawings

  QPainterPath path;
  path.moveTo(negativeGap.start().x(), negativeGap.start().y());
  path.lineTo(negativeGap.end().x(), negativeGap.end().y());
  path.lineTo(positiveGap.end().x(), positiveGap.end().y());
  path.lineTo(positiveGap.start().x(), positiveGap.start().y());
  path.closeSubpath();
  setPath(path);

  qDeleteAll(childItems());

  qreal opacity;
  QPen darkGrayLine(Qt::darkGray, 0.1);

  if ( ispedestrianRoad(_Road->name()) )
      opacity = 0.0;
  else
      opacity = 0.4;

  // draw the white lines for the two borders of the road
  QGraphicsLineItem* negative(new QGraphicsLineItem(this));
  negative->setZValue(LANE_LAYER);
  negative->setLine(negativeGap.toQLineF());
  negative->setOpacity(opacity);
  negative->setPen(darkGrayLine);

  QGraphicsLineItem* positive(new QGraphicsLineItem(this));
  positive->setZValue(LANE_LAYER);
  positive->setLine(positiveGap.toQLineF());
  positive->setOpacity(opacity);
  positive->setPen(darkGrayLine);

// walkway ----------------------------------------------------{{

  //TODO remove the crossing bits by removing 1.0 (the gap added in posPavGap/negPavGap )

#if _CROSSINGS_
  if ( !ispedestrianRoad(_Road->name()) )
  {
      QPen pavPen("orange");
      qreal pavOpacity = 1.0;

      QGraphicsLineItem* posPav(new QGraphicsLineItem(this));
      posPav->setZValue(LANE_LAYER);
      posPav->setLine(posWalkway.toQLineF());
      posPav->setOpacity(pavOpacity);
      posPav->setPen(pavPen);

      QGraphicsLineItem* negPav(new QGraphicsLineItem(this));
      negPav->setZValue(LANE_LAYER);
      negPav->setLine(negWalkway.toQLineF());
      negPav->setOpacity(pavOpacity);
      negPav->setPen(pavPen);
  }
#endif

  // add the crosswalks

  if ( !ispedestrianRoad(_Road->name()) )
  {

#if _CROSSINGS_
//      cout << " ====>  " <<  _Road->name().toStdString() << endl;

      qreal alpha = 0.02;
      qreal beta = 0.1;
      qreal width = 0.5;
      qreal alphas[] = {0.0, 0.25, 0.5, 0.75, 1.0};

      QPointF L1p1(positiveGap.toQLineF().p1());
      QPointF L1p2(positiveGap.toQLineF().p2());
      QPointF L2p1(negativeGap.toQLineF().p1());
      QPointF L2p2(negativeGap.toQLineF().p2());

      for (int k = 0 ; k < (sizeof(alphas)/sizeof(qreal)) ; k++ )
      {
          QGraphicsLineItem* _e(new QGraphicsLineItem(this));
          _e->setZValue(LANE_LAYER);
          _e->setLine(getLine(
                          QPointF(
                              L1p1.x() * alphas[k] + (1-alphas[k]) * L2p1.x(),
                              L1p1.y() * alphas[k] + (1-alphas[k]) * L2p1.y()),
                          QPointF(
                              L1p2.x() * alphas[k] + (1-alphas[k]) * L2p2.x(),
                              L1p2.y() * alphas[k] + (1-alphas[k]) * L2p2.y()),
                              alpha, beta));
          _e->setPen(QPen(Qt::lightGray , width));
      }
      /*
        QGraphicsLineItem* e4(new QGraphicsLineItem(this));
        e4->setZValue(LANE_LAYER);
        e4->setLine(getLine(posPavGap.toQLineF().p1(), posPavGap.toQLineF().p2(), alpha, beta));
        e4->setPen(QPen(Qt::blue, width));
        QGraphicsLineItem* e5(new QGraphicsLineItem(this));
        e5->setZValue(LANE_LAYER);
        e5->setLine(getLine(negPavGap.toQLineF().p1(), negPavGap.toQLineF().p2(), alpha, beta));
        e5->setPen(QPen(Qt::cyan, width));
      */
#endif

      // Render lane delimitors

      unsigned int nbTotalLanes(_LanesFirstToSecond + _LanesSecondToFirst);

      if (nbTotalLanes)
      {
          unsigned int laneWidth( _RoadWidth / nbTotalLanes );
          for (unsigned int i = 1; i < nbTotalLanes; ++i)
          {
              QGraphicsLineItem* division(new QGraphicsLineItem(this));
              division->setZValue(LANE_LAYER);
              Segment line(negativeGap.towardsShift( i * laneWidth));
              division->setLine(line.toQLineF());
              if (i == _LanesFirstToSecond)
              {
                  // Yellow line in the middle of the road
                  division->setOpacity(opacity);
                  division->setPen(QPen(Qt::yellow, 0.01));
              }
              else
              {
                  // Discontinuous white lines separating lanes
                  QPen whiteDiscontinuous(Qt::DotLine);
                  whiteDiscontinuous.setWidthF(0.01);
                  division->setOpacity(opacity);
                  whiteDiscontinuous.setBrush(Qt::white);
                  division->setPen(whiteDiscontinuous);
              }
          }
      }
  }
}

// Modify the first point of the road, then redraw
// @param first first point of the road

void RoadGUI::setFirst(const Point &first)
{
  _First = first;
  drawRoad();
}

// Modify the second point of the road, then redraw
// @param second second point of the road

void RoadGUI::setSecond(const Point &second)
{
  _Second = second;
  drawRoad();
}

// Modify the number of lanes from the first to the second, then redraw
// @param nb number of lanes from first to second.

void RoadGUI::setLanesFirstToSecond(unsigned int nb)
{
  _LanesFirstToSecond = nb;
  drawRoad();
}

// Modify the number of lanes from the second to the first, then redraw
// @param nb number of lanes from second to first.

void RoadGUI::setLanesSecondToFirst(unsigned int nb)
{
  _LanesSecondToFirst = nb;
  drawRoad();
}

// modify the width of the road, then redraw the road.
// @param width

void RoadGUI::setRoadWidth(unsigned int width)
{
  _RoadWidth = width;
  drawRoad();
}
