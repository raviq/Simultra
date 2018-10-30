#ifndef SIMULATION_H
#define SIMULATION_H

#include <QThread>
#include <QList>
#include <QMap>
#include <QTime>
#include "imagesmanager.h"
#include "plan.h"
#include "vehicle.h"
#include "driver.h"
#include "detector.h"
#include "ui_simulator.h"
#include "atypes.h"
#include "world.h"

class QGraphicsItem;
class Signalisation;

class Simulation : public QThread
{
public:

  // to send a payload to the external manager.
 // void* send_payload (int round, char* name, double lat, double lon, double delay, char* ip, int port);
   static void* send_payload (void* destination_node);


  static Simulation* instance();
  static bool deleteInstance();

  virtual void execute();
  void openXMLFilePlan(const QString &fileName);
  void erasePlan();
  void openXMLFileTypes(const QString &fileName);
  void openXMLFileSimulation(const QString &fileName);
  QString openXMLFileSignalization(const QString &fileName);
  void refreshSimulation();

  void setWorld(World *w);
  World* getWorld();

  void setUI(Ui::TrafficSimulator *ui);

  void setFinSimulation(bool fin);
  bool endSimulation();
  void move();
  void addVehicle(Vehicule *v);
  Vehicule* addVehicle(const QString &name, const QString &name_TypeVehicule);
  Vehicule* findVehicle(const QString &name);
  void addDriver(Driver *c);
  Driver* addDriver(const QString &name, const QString &name_TypeDriver);
  Driver* findDriver(const QString &name);
  static qreal _simulationStep;
  static QTime _TpsSimulation;
  QTime _tx;

  QList<TypeVehicule*> _VehicleTypes;
  QList<DriverType*> _DriverTypes;
  QList<Vehicule*> _Vehicles;
  QList<Driver*> _Drivers;
  QMap<const Plan::Intersection*, Signalisation*> _Signalisations;
  Plan *_Plan;
  ImagesManager _ImagesManager;

  QList<Detector*> _Detectors;

private:
  Simulation();
  ~Simulation();

  static Simulation *_Instance;
  static Ui::TrafficSimulator *_UI;

  bool _EndSimulation;
  QString _simulationFile;
  QString _planFile;
  QString _SignalisationFile;
  unsigned int _NbTotalPas;

  QTime _t0;

  int round;

  World* _world;

};

#endif
