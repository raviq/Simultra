#ifndef INTERSECTIONGUI_H
#define INTERSECTIONGUI_H

#include <QGraphicsPathItem>
#include "plan.h"
#include "utils.h"

class IntersectionGUI : public QGraphicsPathItem
{
public:
  IntersectionGUI(QGraphicsItem* parent = 0);
  void ajouterRoads(const Plan::RoadsList& roads);
  void setNomIntersection(const QString& name);
  void setCentreIntersection(const Utils::Point& centre);

private:
  void drawIntersection();
  QString _Name;
  Utils::Point _Centre;
  QList<Utils::Segment> _Segments;
  QList<Utils::Point> _Points;
};

#endif
