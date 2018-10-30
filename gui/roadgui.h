#ifndef ROUTEGUI_H
#define ROUTEGUI_H

#include <QGraphicsPathItem>
#include "point.h"
#include "plan.h"

class RoadGUI : public QGraphicsPathItem
{
    public:
    RoadGUI(Plan::Road *road, QGraphicsItem* parent = 0);
      void setFirst(const Utils::Point& first);
      void setSecond(const Utils::Point& second);
      void setLanesFirstToSecond(unsigned int nb);
      void setLanesSecondToFirst(unsigned int nb);
      void setRoadWidth(unsigned int width);

    protected:
      void drawRoad();
      Utils::Point _First, _Second;
      unsigned int _LanesFirstToSecond, _LanesSecondToFirst;
      int _RoadWidth;
      Plan::Road *_Road;

      char* _NameFirst;
      char* _NameSecond;
};

#endif
