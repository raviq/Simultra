#ifndef SIMULATIONGUI_H
#define SIMULATIONGUI_H

#include <QGraphicsView>
#include "plan.h"
#include "signalisationgui.h"

#include "vehiclegui.h"

class SimulationGUI : public QGraphicsView
{
    public:
      SimulationGUI(QWidget* parent = 0);
      void refreshImages();
      void redrawPlan();
      void drawMap();
      void deleteImages(const bool deleteScene=false);
      void zoom(bool frontZoom);
      void renderVelocity(bool render=true);
      void renderFrictions(bool render=true);
      void renderMotorForce(bool render=true);
      void centrer();
      void addMargins(const float marge=0.0f);

    protected:
      virtual void wheelEvent(QWheelEvent *event);
      QGraphicsScene _Scene;
      QMap<QString, VehicleGUI*> _GUIsVehicule;
      QMap<QString, SignalizationGUI*> _GUIsSignalisation;

    private:
      QGraphicsPathItem* drawRoad(Plan::Road* r);
      QGraphicsPathItem* drawIntersection(Plan::Intersection* i, const Plan::RoadsList& roads);
      bool _RenderVelocity;
      bool _renderFrictions;
      bool _renderMotorForce;
      float _Zoom;
};

#endif
