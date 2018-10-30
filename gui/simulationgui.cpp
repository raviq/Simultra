//
// Rendering the elements of the simulation
//
#include <QWheelEvent>
#include <iostream>
#include <QTimer>

#include "simulationgui.h"
#include "roadgui.h"
#include "simulation.h"
#include "image.h"
#include "intersectiongui.h"

QColor Colors[10] = { QColor("cyan"), QColor("magenta"), QColor("red"),
                      QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                      QColor("green"), QColor("darkGreen"), QColor("yellow"),
                      QColor("blue") };

using namespace std;

// Constructor, zoom=1 et render the scene
SimulationGUI::SimulationGUI(QWidget *parent) : QGraphicsView(parent)
{
  setScene(&_Scene);
  _Zoom = 1.0f;
  show();
}

// Render all the velocity vectors for the vehicles

void SimulationGUI::renderVelocity(bool render)
{
  _RenderVelocity = render;
  foreach(VehicleGUI* vi, _GUIsVehicule)
  {
    if (vi)
        vi->renderVelocity(_RenderVelocity);
  }
}

// Render all the frictions vectors for the vehicles

void SimulationGUI::renderFrictions(bool render)
{
  _renderFrictions = render;
  foreach(VehicleGUI* vi, _GUIsVehicule)
  {
    if (vi)
        vi->renderFrictions(_renderFrictions);
  }
}

// Render all the force motor vectors for the vehicles

void SimulationGUI::renderMotorForce(bool render)
{
  _renderMotorForce = render;
  foreach(VehicleGUI* vi, _GUIsVehicule)
  {
    if (vi)
        vi->renderMotorForce(_renderMotorForce);
  }
}

// Add a margin on the 4 sides of the simulation gui

void SimulationGUI::addMargins(const float margin)
{
  QRectF rectangle = _Scene.itemsBoundingRect();
  rectangle.setLeft(rectangle.left() - margin);
  rectangle.setTop(rectangle.top() - margin);
  rectangle.setBottom(rectangle.bottom() + margin);
  rectangle.setRight(rectangle.right() + margin);
  setSceneRect(rectangle);
}

// Center the window on the middle of the simulation and adjust to zoom

void SimulationGUI::centrer()
{
  addMargins();
  centerOn(sceneRect().center());
  resetTransform();
  _Zoom = 1.0f;
}

// Get an intersection from an un objet intersection and a list of road objects

QGraphicsPathItem* SimulationGUI::drawIntersection(Plan::Intersection *i, const Plan::RoadsList& roads)
{
  IntersectionGUI* inter(new IntersectionGUI());

  if (i)
  {
    inter->setCentreIntersection(i->location());
    inter->setNomIntersection(i->name());
    inter->ajouterRoads(roads);
  }

  return inter;
}

// Get a GUI object road from actual road object

QGraphicsPathItem* SimulationGUI::drawRoad(Plan::Road *road)
{
  RoadGUI* r(new RoadGUI(road));
  r->setFirst(road->first());
  r->setSecond(road->second());
  r->setLanesFirstToSecond(road->nbLanes(Plan::Road::Lane::FIRST_TO_SECOND));
  r->setLanesSecondToFirst(road->nbLanes(Plan::Road::Lane::SECOND_TO_FIRST));
  r->setRoadWidth(road->width());
  return r;
}

// Erase all the img of signalisation & vehicles of the simulation

void SimulationGUI::deleteImages(const bool deleteScene)
{
  foreach(VehicleGUI *vi, _GUIsVehicule)
      _Scene.removeItem(vi);
    _GUIsVehicule.clear();

  foreach(SignalizationGUI *sGUI, _GUIsSignalisation)
    _Scene.removeItem(sGUI);

  _GUIsSignalisation.clear();

  if (deleteScene)
    _Scene.clear();
}

// Refresh the content of all vehicle & signalisation images

void SimulationGUI::refreshImages()
{
  Simulation* simu(Simulation::instance());

  if (simu)
  {
    Image* image(simu->_ImagesManager.currentImage());

    if (image)
    {
        foreach(ImageVehicle* imageV, image->imagesVehicule())
        {
              VehicleGUI *vi(_GUIsVehicule.value(imageV->name(), 0));
              if (!vi)
              {
                vi = new VehicleGUI(QPixmap(imageV->imageName()));
                vi->setToolTip(imageV->name());
                _GUIsVehicule[imageV->name()] = vi;
                _Scene.addItem(vi);
              }
              vi->setImageVehicle(imageV);
        }

      foreach(ImageSignalisation *is, image->imagesSignalisation())
      {
        SignalizationGUI *sGUI(_GUIsSignalisation.value(is->name(), 0));
        if (!sGUI)
        {
          sGUI = new SignalizationGUI();
          _GUIsSignalisation[is->name()] = sGUI;
          _Scene.addItem(sGUI);
        }
        sGUI->setImage(is);
      }
      addMargins();

    }
  }
  else
  {
    deleteImages();
  }
}

// Render OSM

void SimulationGUI::drawMap()
{
  deleteImages();

  Simulation* simu(Simulation::instance());

  if (simu && simu->_Plan)
  {
          _Scene.addEllipse(100, 100, 20, 20, QPen(Qt::green));
  }
}

// Re-draw all the roads and intersections of the simulation.

void SimulationGUI::redrawPlan()
{
  deleteImages();
  Simulation* simu(Simulation::instance());
  if (simu && simu->_Plan)
  {
    foreach(Plan::Road* r, simu->_Plan->getRoads())
    {
      QGraphicsPathItem* road(drawRoad(r));
      _Scene.addItem(road);
    }

    foreach(Plan::Intersection* i, simu->_Plan->getIntersections())
    {
      Plan::RoadsList listeroads(simu->_Plan->getRoads(i));
      QGraphicsPathItem* intersection(drawIntersection(i, listeroads));
      _Scene.addItem(intersection);
    }
  }
}

//  Managing the "wheelEvent" of the simulation, and in particular,
//  limiting the speed at which the user could move around the window

void SimulationGUI::wheelEvent(QWheelEvent *wheel)
{
  qreal vitesseLimite = width() * 0.14;
  QPointF centreScene = mapToScene(rect().center());
  QPointF positionSceneWheel = mapToScene(wheel->pos());
  QPointF sceneCenterWindowG = mapToGlobal(rect().center());
  QPointF positionGlobaleWheel = mapToGlobal(wheel->pos());
  QPointF diffCentreFenetre = positionGlobaleWheel - sceneCenterWindowG;

  if (diffCentreFenetre.x() > vitesseLimite)
    diffCentreFenetre.setX(vitesseLimite);
  else if (diffCentreFenetre.x() < -vitesseLimite)
    diffCentreFenetre.setX(-vitesseLimite);
  if (diffCentreFenetre.y() > vitesseLimite)
    diffCentreFenetre.setY(vitesseLimite);
  else if (diffCentreFenetre.y() < -vitesseLimite)
    diffCentreFenetre.setY(-vitesseLimite);

  qreal echelleHorizontale = diffCentreFenetre.x() / (width()/2) * _Zoom;
  qreal echelleVerticale = diffCentreFenetre.y() / (height()/2) * _Zoom;
  qreal newX = (positionSceneWheel.x() - centreScene.x()) * qAbs(echelleHorizontale);
  qreal newY = (positionSceneWheel.y() - centreScene.y()) * qAbs(echelleVerticale);

  QPointF new_center((positionSceneWheel.x() + newX), (positionSceneWheel.y() + newY));

  if (wheel->delta() > 0)
  {
      zoom(1);
  }
  else
  {
      zoom(0);
  }

  centerOn(new_center);
  wheel->accept();

}

// Zoom (+/-) in the simulation window with ratio=zoom_ratio

qreal zoom_ratio = 1.08;

void SimulationGUI::zoom(bool frontZoom)
{
  if (frontZoom)
  {
    scale(zoom_ratio, zoom_ratio);
    _Zoom *= 0.97;
  }
  else
  {
    scale(1/zoom_ratio, 1/zoom_ratio);
    _Zoom = 1.03;
  }
}
