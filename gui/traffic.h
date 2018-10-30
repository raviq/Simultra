#ifndef TRAFFIC_H
#define TRAFFIC_H
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "simulation.h"
#include "graph.h"
#include "world.h"
#include "weather.h"
#include "vehimob.h"
#include "config.h"

#include "MapGraphics/MapGraphicsScene.h"
#include "MapGraphics/MapGraphicsView.h"
#include "MapGraphics/CircleObject.h"


namespace Ui
{
    class TrafficSimulator;
}

class TrafficSimulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrafficSimulator(QWidget *parent = 0);
    ~TrafficSimulator();
    void setWalkers(int numberOfWalkers, QPointF* walk, int swalk, QColor colour);
    void addOsmMapGraphics(QWidget* parent);
    void addComponents();


private:
  void updateGUI();

  QAction* _aboutAct;
  Ui::TrafficSimulator *ui;
  World* _world;
  Graph* graph;
  Simulation* _Simulation;
  QTimer* _Clock;
  bool _SimRealTime;
  bool _beingRead;
  QMap<QString, QGraphicsTextItem*> _Markers;

  MapGraphicsScene * _OsmScene;
  MapGraphicsView * _OsmView;
  VehicleImageObject * _vehicles[_MAX_VEHICLES_];

  CircleObject* _circle;
  CircleObject* _center;

  JWeatherManager * _weatherManager;

  qreal _alpha;
  int _zoomLevel;
  int _numVehicles;

  public slots:
  void OnClock();
  void OnReadStopSimulation();
  void OnOpenSimulation();
  void OnSettingSpeedSim(int speed);
  void OnZoomArriere();
  void OnZoomAvant();
  void onNewButtonpressed();
  void OnSelectDragMode();
  void OnOsmZooming(int zoom);
  void go();
  void disactivateWeather();
  void increasesizeVehicles();
  void decreasesizeVehicles();

private slots:
  void on_checkBoxIntersecNaming_toggled(bool checked);
  void on_checkBoxRenderVelocity_toggled(bool checked);
  void on_checkBoxRenderMotorForce_toggled(bool checked);
  void on_checkBoxRenderFrottements_toggled(bool checked);
  void On_about_Action();

};

#endif // TRAFFIC_H
