#include "intersectiongui.h"
#include <QFont>
#include <QPen>
#include <qmath.h>
#include "utils.h"
#include <ctype.h>

#include <iostream>
using namespace std;

// Constructor, black background for intersection

IntersectionGUI::IntersectionGUI(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
  setZValue(Utils::INTERSECTION_LAYER);
  setPen(QPen(Qt::black));
  setBrush(Qt::black);
}

// Populate _Points and _Segments with the road content, normalize
// the postion of the points w.r.t to the intersection center, then draw the intersection
// @param roads RoadsList data used to draw the intersection

void IntersectionGUI::ajouterRoads(const Plan::RoadsList &roads)
{
  // set _Points and _Segments with roads content
  foreach(Plan::Road* r, roads)
  {
    if (r->getFirstIntersection() && r->getSecondIntersection())
    {
      Utils::Segment segment;
      segment.start() = r->first();
      segment.end() = r->second();
      if (r->getFirstIntersection()->name() == _Name)
      {
        Utils::Segment segmentP(segment.towardsPerpendicularFromStart(0, r->width()));
        _Segments.push_back(segmentP);
        _Points.push_back(segmentP.start());
        _Points.push_back(segmentP.end());
      }
      else
      {
        Utils::Segment segmentP(segment.towardsPerpendicularFromEnd(0, r->width()));
        _Segments.push_back(segmentP);
        _Points.push_back(segmentP.start());
        _Points.push_back(segmentP.end());
      }
    }
  }

  // Find the center of the intersection
  Utils::Point centre(0.0f, 0.0f), dir(0.0f, -1.0f);
  foreach(Utils::Point p, _Points)
    centre += p;
  centre /= _Points.count();

  // Find the positions of the points viz the center of the intersection
  for (int i=0; i<_Points.count(); ++i)
  {
    for (int j=0; j<_Points.count(); ++j)
    {
      Utils::Point pointI = _Points.at(i), pointJ = _Points.at(j);
      pointI -= centre;
      pointI.normalise();
      pointJ -= centre;
      pointJ.normalise();
      qreal angleI = Utils::Point::angleSign(pointI, dir) * 180.0/M_PI,
            angleJ = Utils::Point::angleSign(pointJ, dir) * 180.0/M_PI;
      if (angleI > 0) angleI -= 360;
      if (angleJ > 0) angleJ -= 360;
      if (angleI < angleJ)
      {
        Utils::Point temp(_Points.at(j));
        _Points.replace(j, _Points.at(i));
        _Points.replace(i, temp);
      }
    }
  }
  drawIntersection();
}

// Draw the borders and the name of the intersection
// @param renderNom draw or not the name of the intersection

void IntersectionGUI::drawIntersection()
{
  if (_Segments.count() > 1)
  {
    // Find intersection center
    Utils::Point centre(0, 0);
    foreach(Utils::Point p, _Points)
      centre += p;
    centre /= _Points.count();

    // Determine the QPainterPath in which the following iterms will be drawn
    QPainterPath path;
    path.moveTo(_Points.front().x(), _Points.front().y());
    path.lineTo(_Points.front().x(), _Points.front().y());
    for(int i=1; i<_Points.count(); ++i)
    {
      path.lineTo(_Points.at(i).x(), _Points.at(i).y());
    }
    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);
    setPath(path);

    // Walker path?
   if (islower(_Name.toStdString()[0]))
   {
        cout << "_Name --> " << _Name.toStdString() << endl;
        // Don't draw anything
        return;
   }

    // Intersection borders
    QPen colour(Qt::green);
    colour.setWidthF(0.01);

    // Draw using a polygon, with rounded corners
    foreach(Utils::Segment segment, _Segments)
    {
      QGraphicsLineItem* line(new QGraphicsLineItem(this));
      line->setPen(colour);
      line->setLine(segment.toQLineF());
    }

  }
}

void IntersectionGUI::setNomIntersection(const QString &name)
{
  _Name = name;
}

void IntersectionGUI::setCentreIntersection(const Utils::Point &centre)
{
  _Centre = centre;
}
