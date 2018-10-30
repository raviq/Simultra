//
// Simulation heart
//

#include <QFile>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomElement>
#include <QStringList>
#include <iostream>
#include <QtDebug>
#include <QGraphicsLineItem>
#include <qmath.h>
#include <limits>
#include <cmath>
#include <iomanip>

#include "simulation.h"
#include "plan.h"
#include "utils.h"
#include "log.h"
#include "signalisation.h"
#include "math.h"
#include "detector.h"
#include "conversions.h"
#include "config.h"

#if _DISTRIBUTED_
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "atypes.h"
#include "payload.h"
#include "debug.h"
#include "json.h"
#endif

using namespace Utils;
using namespace std;

Ui::TrafficSimulator* Simulation::_UI = 0; // pointer to the UI
Simulation* Simulation::_Instance = 0;
qreal Simulation::_simulationStep = 0.4f;
QTime Simulation::_TpsSimulation;

#if _DISTRIBUTED_
void* Simulation::send_payload (void* destination_node)

{
    int len, sock = -1;
    struct sockaddr_in server;
    char server_reply[2000];

    int round = ((node_t*) destination_node)->round;
    double lat = ((node_t*) destination_node)->lat;
    double lon = ((node_t*) destination_node)->lon;
    double delay = ((node_t*) destination_node)->delay; // TODO

    UNUSED(delay);

    srand((unsigned int)time(NULL));

    // create socket
    if ((sock = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("error: cannot create socket\n");
        ::exit(-1);
    }
    else
    {
        debug("Socket created.");
    }

    server.sin_addr.s_addr = inet_addr( ((node_t*) destination_node)->ip );
    server.sin_family = AF_INET;
    server.sin_port = htons( ((node_t*) destination_node)->port );

    // connect to remote node
    if (::connect(sock , (struct sockaddr *) & server , sizeof(server)) < 0) // use ::connect for standard C++ socket instead of QT QSocket object.
    {
        perror("Connect failed");
        ::exit(-1);
    }

    debug("Connected.\n");

    // keep communicating with server

    while((len = recv(sock, server_reply, sizeof(server_reply), 0)) > 0)
    {
        command_t command;
        sscanf(server_reply, "%d", &command);

        switch (command)
        {
            case DONE:
            {
                debug("Server reply: DONE.\n");
                break;
            }

            case ACK:
            {
              debug("Server reply: ACK.\n");
              break;
            }

            default:
            {
                debug("Server reply: <%.*s>.\n", len, server_reply);
                break;
            }
        }

        // Setting payload ---<<<
        char* payload_string = NULL;
        cJSON* root, *fld;
        char* out;
        const char* data = "127.0.0.1:8058,127.0.0.1:8019,127.0.0.1:8003,127.0.0.1:8065"; // TODO define as JSON and add to the other JSON data

         root = cJSON_CreateObject();
         cJSON_AddItemToObject(root, "Mobility", fld=cJSON_CreateObject());
         cJSON_AddStringToObject(fld, "Precision", "precType");
         cJSON_AddNumberToObject(fld, "Round", round);
         cJSON_AddNumberToObject(fld, "Latitude", lat);
         cJSON_AddNumberToObject(fld, "Longitude", lon);
         cJSON_AddStringToObject(fld, "Name", ((node_t*) destination_node)->name );
         cJSON_AddNumberToObject(fld, "Command", MOVING);
         cJSON_AddStringToObject(fld, "Data", data);

         out = cJSON_Print(root);
         cJSON_Delete(root);

         if ( (payload_string = (char*) malloc(sizeof(char) * (strlen(out) + 1))) != NULL )
            strcpy(payload_string, out);
         else
            debug ("Cannot allocate *result !\n");

         free(out);

        // --->>>

        debug("Sending: %s", payload_string);

        // send payload
        if( send(sock , payload_string , strlen(payload_string) , 0) < 0)
        {
            debug("Send failed");
        }
        free(payload_string);
    }

    close(sock);

    return 0;
}

#endif

// Constructor, reset clock & steps

Simulation::Simulation()
{
  _TpsSimulation.setHMS(0, 0, 0, 0);
  _EndSimulation = false;
  _Plan = 0;
  _NbTotalPas = 0;
  _UI = 0;
  _t0 = QTime::currentTime(); // stimulatin starting at _t0
  _tx.setHMS(0, 0, 0, 0);
  round = 0;
}

Simulation::~Simulation()
{
  _EndSimulation = true;
  _TpsSimulation.setHMS(0,0,0);
}

Simulation* Simulation::instance()
{
  if (!_Instance)
  {
    _Instance = new Simulation;
  }
  return _Instance;
}

void Simulation::setWorld(World* w)
{
    _world = w;
}

World* Simulation::getWorld()
{
    return _world;
}

void Simulation::setUI(Ui::TrafficSimulator *ui)
{
   _UI = ui;
   _UI->debugBrowser->setText(
               QString("UI Loaded.\n Loading") +
               "\t Simulation description in '" + _simulationFile + "'\n" +
               "\t Map description in '" + _planFile + "'\n" +
               "\t Signalization description in '" + _SignalisationFile + "'\n\n" +
               "\t Storing vehicles mobility in 'vehicles.csv'\n" +
               "\t Storing flow data in 'flow.csv'\n" +
               "\t Storing detectors data in 'detections.csv'\n" +
               " Scenario with " + QString::number(_Vehicles.size()) + " vehicle(s)"
               " and " + QString::number(_Signalisations.size()) + " signalisation area(s).");

   // set the simulation view to a black background.
   _UI->simulationView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

#if _DETECTORS_

   QPen pen;
   pen.setStyle(Qt::DotLine);
   pen.setWidth(10);
   pen.setBrush(Qt::blue);

   QGraphicsLineItem* dline1 = new QGraphicsLineItem(0, ui->simulationView->scene() );
   dline1->setZValue(Utils::LANE_LAYER);
   dline1->setLine(QLine(-100, 125, -100, 75));  // AJ
   dline1->setPen(pen);

   QGraphicsLineItem* dline2 = new QGraphicsLineItem(0, ui->simulationView->scene() );
   dline2->setZValue(Utils::LANE_LAYER);
   dline2->setLine(QLine(425, 250, 425, 200));  // FN
   dline2->setPen(pen);

   QGraphicsLineItem* dline3 = new QGraphicsLineItem(0, ui->simulationView->scene() );
   dline3->setZValue(Utils::LANE_LAYER);
   dline3->setLine(QLine(-100, 325, -100, 275));  // GL
   dline3->setPen(pen);

   QGraphicsLineItem* dline4 = new QGraphicsLineItem(0, ui->simulationView->scene() );
   dline4->setZValue(Utils::LANE_LAYER);
   dline4->setLine(QLine(425, 425, 425, 375));  // UW
   dline4->setPen(pen);


   // adding detectors.
   QList<QString> toMark;           // TODO load from xml
     toMark.append(QString("AJ"));
     toMark.append(QString("UW"));
     toMark.append(QString("GL"));
     toMark.append(QString("FN"));

   bool head = true;
   qreal where = 0.0;
   qreal proximity = 10.0;

   foreach(Plan::Road* road, _Plan->getRoads())
   {
       if (toMark.contains(road->name()))
       {
               QString dname = "detector@" + road->name();
               Detector* driver = new Detector(dname, road, head, where, proximity);
               _Detectors.append(driver);
               Point pos = driver->draw(_UI->simulationView->scene());
               cout << road->name().toStdString() << " at " << n_s(pos.x()).toStdString() << ", " << n_s(pos.y()).toStdString() << endl;
       }
    }

    _UI->detectBrowser->setText("\n  Init (" + n_s(_Detectors.size()) + ") detector(s).");
#endif
}

bool Simulation::deleteInstance()
{
  if (_Instance)
  {
    delete _Instance;
    _Instance = 0;
    return true;
  }
  return false;
}

// Parse the map (XML) and assign to _Plan
void Simulation::openXMLFilePlan(const QString &fileName)
{
  erasePlan();
  _Plan = new Plan(fileName);
  _planFile = fileName;
}

void Simulation::erasePlan()
{
  if (_Plan)
  {
    delete _Plan;
    _planFile = "";
  }
  _Plan = 0;
}

QString Simulation::openXMLFileSignalization(const QString &fileName)
{
    QString filenameSimulation;
    QDomDocument doc("Map");
    QFile file(fileName);
    if( !file.open( QIODevice::ReadOnly ) )
        return filenameSimulation;

    if( ! doc.setContent(&file) )
      file.close();

    file.close();
    _Signalisations.clear();
    _SignalisationFile = fileName;

    QDomNodeList elementsPlan = doc.elementsByTagName("Topology");
    if (elementsPlan.isEmpty())
        return filenameSimulation;
    else
        openXMLFilePlan(fileName);

  QDomNodeList elementsSimulation = doc.elementsByTagName("Simulation");
  filenameSimulation = elementsSimulation.at(0).toElement().text();

  QDomNodeList lightElements = doc.elementsByTagName("Light");
  for (int i = 0; i < lightElements.size(); i++)
  {
    Signalisation *s = new Signalisation();
    QDomElement lightElement = lightElements.at(i).toElement();
    const Plan::Intersection *inter = NULL;

    QDomNodeList childsLight = lightElement.childNodes();
    for (int j=0; j < childsLight.size(); j++)
    {
      QDomElement childLight = childsLight.at(j).toElement();
      if (childLight.tagName()=="Intersection")
      {
        inter = _Plan->getIntersection(childLight.text());
        s->setIntersection(inter);
      }
      else if (childLight.tagName()=="State")
      {
        QDomElement stateElement = childLight.toElement();
        Signalisation::State *state = new Signalisation::State(stateElement.attribute("name"));

        QDomNodeList enfantsState = stateElement.childNodes();
        for (int k=0; k<enfantsState.size(); k++)
        {
          QDomElement childState = enfantsState.at(k).toElement();

          if (childState.tagName()=="GreenDuration")
            state->setGreenDuration(childState.text().toFloat());
          else if (childState.tagName()=="OrangeDuration")
            state->setOrangeDuration(childState.text().toFloat());
          else if (childState.tagName()=="RedDuration")
            state->setRedDuration(childState.text().toFloat());
          else if (childState.tagName()=="Connection")
          {
            QStringList separateList= childState.text().split(",", QString::SkipEmptyParts);
            const Plan::Road *start = _Plan->getRoad(separateList.at(0));
            unsigned int startLane = separateList.at(1).toInt();
            const Plan::Road *end = _Plan->getRoad(separateList.at(2));
            unsigned int endLane = separateList.at(3).toInt();
            Signalisation::Connection *conn = new Signalisation::Connection(start, startLane, end, endLane);
            state->addConnection(conn);
          }
        }
        s->addState(state);
      }
    }
    if (inter)
    {
      _Signalisations[inter] = s;
    }
  }
  return filenameSimulation;
}

void Simulation::refreshSimulation()
{
    _TpsSimulation.setHMS(0,0,0);
  if (!_SignalisationFile.isEmpty())
    openXMLFileSignalization(_SignalisationFile);
}

void Simulation::setFinSimulation(bool end)
{
  _EndSimulation = end;
}

bool Simulation::endSimulation()
{
  return _EndSimulation;
}

void Simulation::move()
{

  round += 1;

  _UI->lcdTimer->display( _tx.toString(QString("hh:mm:ss"))  );
  _UI->logsBrowser->setText("");
  _UI->agentBrowser->setText("");
  _UI->lanesBrowser->setText(" t\tVehicle\tLane\tRoad\tLength\tStart\tEnd\tSpeed\tx\ty\tin/out\n");
  _UI->paramBrowser->setText("");
  //_UI->detectBrowser->setText("");

  foreach(Signalisation *sign, _Signalisations)
  {
      sign->avancer();
      const Plan::Intersection *intersection = sign->intersection();
      _UI->logsBrowser->setText(
                          QTime::currentTime().toString() + "   " +
                          intersection->name() + "  " +
                          sign->currentState()->name() + "  " +
                          QString::number(sign->currentState()->getConnections().size()) + "  " +
                          QString::number(sign->currentState()->lightColor()) + "  " +
                          QString::number(sign->currentState()->orangeDuration()) + "  " +
                          QString::number(sign->currentState()->redDuration()) + "  " +
                          QString::number(sign->currentState()->greenDuration()) + "\n" +
                          _UI->logsBrowser->document()->toPlainText());
  }

  qreal mean_speed = 0.0;
  qreal space_mean_speed = 0.0;
  QMap<QString, int> nbVehiclesPerLane;
  QMap<QString, qreal> laneLengths;

  // Vehicle density on the lane/edge (#veh/km)

  // adding drivers
  foreach(Driver * driver, _Drivers)
  {
      driver->move();

      Point position( driver->_Vehicle->location() + driver->_Vehicle->velocity() * UNIT_PER_METRE *  0.1f );

      _UI->agentBrowser->setText(QTime::currentTime().toString() + "  " +
                                  QString::number(position.x()) + ", " +
                                  QString::number(position.y()) + "\n" +
                                  _UI->agentBrowser->document()->toPlainText());

      Point start = driver->_currentLane._Segment.start();
      Point end = driver->_currentLane._Segment.end();
      QString vName = driver->_Vehicle->name();
      QString roadName = driver->_currentRoad->name();
      qreal roadLength = driver->_currentRoad->height();
      qreal speed = driver->_Vehicle->speed();
      uint currentLane = driver->_IdCurrentLane;

      qWarning() << qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz")) << ", "
                 << qPrintable(vName) << ", "
                 << currentLane << ", "
                 << qPrintable(roadName) << ", "
                 << roadLength << ", "
                 << qPrintable(start.qstr()) << ", "
                 << qPrintable(end.qstr()) << ", "
                 << speed
                 << position.x()
                 << position.y()
                 << endl;

      _UI->lanesBrowser->setText(
                 _UI->lanesBrowser->document()->toPlainText() +
                 QTime::currentTime().toString("hh:mm:ss.zzz") + "\t" +
                 vName + "\t" +
                 n_s(currentLane) + "\t" +
                 roadName + "\t" +
                 n_s(roadLength)   + "\t" +
                 start.qstr() + " \t " + end.qstr() + "\t" +
                 n_s(speed) + "\t" +
                 QString::number(position.x()) + "\t" +
                 QString::number(position.y()) + "\t" +
                ((driver->_distanceToObjective < 10.0) ? "out " : "in ") + roadName +
                " <- " + ( (driver->_lastIntersection) ? driver->_lastIntersection->name() : "x" ) + "\n"
             );

      // TODOstat
      qreal proximity = 5.0;
      QLine* d1line = new QLine(-100, 125, -100, 75);  // AJ
      QLine* d2line = new QLine(425, 250, 425, 200);   // FN
      QLine* d3line = new QLine(-100, 325, -100, 275); // GL
      QLine* d4line = new QLine(425, 425, 425, 375);   // UW

      // Similar code for 4 detectors TODO combine

      if ( abs( position.x() - d1line->x1() ) < proximity && ( position.y() >= d1line->y2() && position.y() <= d1line->y1()) )
      {
          qCritical()  << "d1" << ", "
                       << qPrintable(vName) << ", "
                       << d1line->x1() << ", " << d1line->y1() << ", "
                       << d1line->x2() << ", " << d1line->y2() << ","
                       << qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz") ) << endl;

          _UI->detectBrowser->setText(
                        vName + " detected in (" + n_s(d1line->x1()) + ", " + n_s(d1line->x2()) + ") at " +
                     QTime::currentTime().toString("hh:mm:ss.zzz") +
                      "\n" + _UI->detectBrowser->document()->toPlainText()
                      );
      }

      if ( abs( position.x() - d2line->x1() ) < proximity && ( position.y() >= d2line->y2() && position.y() <= d2line->y1()) )
      {
          qCritical()  << "d2" << ", "
                       << qPrintable(vName) << ", "
                       << d2line->x1() << ", " << d2line->y1() << ", "
                       << d2line->x2() << ", " << d2line->y2() << ","
                       << qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz") ) << endl;

          _UI->detectBrowser->setText(
                        vName + " detected in (" + n_s(d2line->x1()) + ", " + n_s(d2line->x2()) + ") at " +
                      QTime::currentTime().toString("hh:mm:ss.zzz") +
                      "\n" + _UI->detectBrowser->document()->toPlainText()
                      );
      }


      if ( abs( position.x() - d3line->x1() ) < proximity && ( position.y() >= d3line->y2() && position.y() <= d3line->y1()) )
      {
          qCritical()  << "d3" << ", "
                       << qPrintable(vName) << ", "
                       << d3line->x1() << ", " << d3line->y1() << ", "
                       << d3line->x2() << ", " << d3line->y2() << ","
                       << qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz") ) << endl;

          _UI->detectBrowser->setText(
                        vName + " detected in (" + n_s(d3line->x1()) + ", " + n_s(d3line->x2()) + ") at " +
                      QTime::currentTime().toString("hh:mm:ss.zzz") +
                      "\n" + _UI->detectBrowser->document()->toPlainText()
                      );
      }

      if ( abs( position.x() - d4line->x1() ) < proximity && ( position.y() >= d4line->y2() && position.y() <= d4line->y1()) )
      {
          qCritical()  << "d4" << ", "
                       << qPrintable(vName) << ", "
                       << d4line->x1() << ", " << d4line->y1() << ", "
                       << d4line->x2() << ", " << d4line->y2() << ","
                       << qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz") ) << endl;

          _UI->detectBrowser->setText(
                        vName + " detected in (" + n_s(d4line->x1()) + ", " + n_s(d4line->x2()) + ") at " +
                      QTime::currentTime().toString("hh:mm:ss.zzz") +
                      "\n" + _UI->detectBrowser->document()->toPlainText()
                      );
      }

      if (nbVehiclesPerLane.contains(roadName))
      {
          nbVehiclesPerLane[roadName]++;
      }
      else
      {
          nbVehiclesPerLane[roadName] = 1;
          laneLengths[roadName] = roadLength;
      }
      mean_speed += driver->_Vehicle->speed();

      if (driver->_Vehicle->speed() != 0)
          space_mean_speed += (1./driver->_Vehicle->speed());
  }

  QString data("");
  for (QMap<QString, int>::iterator i = nbVehiclesPerLane.begin(); i != nbVehiclesPerLane.end(); ++i)
      data += (i.key() + "=" + n_s(i.value()) + "   ");

  data += "\n Lengths:    ";
  for (QMap<QString, qreal>::iterator i = laneLengths.begin(); i != laneLengths.end(); ++i)
      data += (i.key() + "=" + n_s(i.value()) + "   ");

  data += "\n Densities:  ";
  for (QMap<QString, qreal>::iterator i = laneLengths.begin(); i != laneLengths.end(); ++i)
      data += (i.key() + "=" + n_s(nbVehiclesPerLane[i.key()] / laneLengths[i.key()]) + "   ");


  data += "\n rho:  ";
  for (QMap<QString, qreal>::iterator i = laneLengths.begin(); i != laneLengths.end(); ++i)
      data += ("p_" + i.key() + "( " + _tx.toString() + " )" + " = " + n_s(nbVehiclesPerLane[i.key()] / laneLengths[i.key()]) + "   ");

  _UI->paramBrowser->setText(
              _UI->paramBrowser->document()->toPlainText() +
              " Time mean speed / spot speed: v_t = " + n_s(mean_speed/_Drivers.size()) + "  at t=" + _tx.toString() + "\n" +
                " Space mean speed / harmonic mean of spot speed: v_s = " + n_s(_Drivers.size()/space_mean_speed) + "\n" +
              "\n nbVperL:   "+ data
              );

#if _DISTRIBUTED_
  int i = 0;
  pthread_t* threads = (pthread_t*) malloc( _Vehicles.size() * sizeof(pthread_t) );
#endif

  foreach(Vehicule * vehicle, _Vehicles)
  {
      vehicle->move();

      Point current (vehicle->location()); // current position
      cout.precision(std::numeric_limits< double >::max_digits10);

//       World* w = this->getWorld();
//       minLat = w->get_minLat();
//       minLon = w->get_minLon();
//       maxLat = w->get_maxLat();
//       maxLon = w->get_maxLon();

      QPointF cp(rev_convert(current.x(), current.y(), _MINLAT_, _MINLON_, _MAXLAT_, _MAXLON_, HEIGHT, WIDTH));
      vehicle->update_OSMvehicle_Location(cp);

#if _DISTRIBUTED_

          // distributed agents {{
               cout << vehicle->name().toStdString()
                    << " --> "
                    << "   QPointF("
                    << vehicle->_OSMvehicle->pos().y()
                    << ", "
                    << vehicle->_OSMvehicle->pos().x()
                    << ")," << endl;

               char* vehname = (char*) malloc( ( vehicle->name().length() + 1 ) * sizeof(char));
               memcpy( vehname, vehicle->name().toStdString().c_str() , vehicle->name().length() + 1);

               node_t node = {  vehicle->_OSMvehicle->pos().y(), // lat
                                vehicle->_OSMvehicle->pos().x(), // lon
                                (double) round, // round
                                MANAGER_IP,     // IP
                                MANAGER_PORT,   // 8033: port we are connecting to.
                                1,              // Delay
                                vehname         // Name
                              };

               if( pthread_create( & threads[i] , NULL , send_payload,  (void *) & node  ) < 0)
               {
                   perror("Error : pthread_create() could not create thread.\n");
               }
               i++;

           // }}
#endif

          qreal speed     = vehicle->speed(true);
          Point velocity  = vehicle->velocity();
          Point destination (vehicle->driver()->destination());

          _UI->logsBrowser->setText(
                  QTime::currentTime().toString() + "   " +
                  vehicle->name().toLatin1() + "  s@" +
                  QString::number(speed, 'f', 10).toLatin1() + " km/h  v@ x=" +
                  QString::number(velocity.x()).toLatin1() + ", y=" +
                  QString::number(velocity.y()).toLatin1() +
                  " angle=" + QString::number(velocity.angle(), 'f', 10).toLatin1() +
                  " dest: x=" + QString::number(destination.x()).toLatin1() + ", y=" + QString::number(destination.y()).toLatin1() +
                  " @ x=" + QString::number(current.x(), 'f', 10).toLatin1() +
                   ", y=" + QString::number(current.y(), 'f', 10).toLatin1() +
                  "\n" +
                  _UI->logsBrowser->document()->toPlainText() // old content
                );

  }

#if _DISTRIBUTED_
  // join threads
  for ( i = 0 ; i < _Vehicles.size() ; i++  )
      pthread_join( threads[i], NULL);
#endif

  _ImagesManager.addImages();
  _TpsSimulation = _TpsSimulation.addMSecs( _MOVE_STEP_ * _simulationStep );
  _tx = _tx.addMSecs( _MOVE_STEP_ * _simulationStep);

  QPoint p = _UI->widget->mapFromGlobal(QCursor::pos());
  _UI->mouseCoords->setText(" (" + n_s(p.x()) + ", " + n_s(p.y()) + ")" );

}

void Simulation::execute()
{
  QTime tpsMax(0, _MAX_SIMULATION_DURATION_);

  while (!_EndSimulation && (_TpsSimulation < tpsMax))
  {
    move();
  }
}

void Simulation::addVehicle(Vehicule *v)
{
  _Vehicles.push_back(v);
}

Vehicule* Simulation::addVehicle(const QString &name, const QString &nameType)
{
  int nbVehicules = _Vehicles.size();
  QString startName = name;
  QString nameFin = startName;
  if (name.isEmpty())
  {
    startName = "Vehicle";
    nameFin = startName+QString::number(nbVehicules);
  }
  Vehicule *vehicleExiste = findVehicle(nameFin);
  int n = nbVehicules+1;
  while (vehicleExiste != NULL)
  {
    vehicleExiste = findVehicle(startName+QString::number(n));
    n++;
  }

  int index = -1;
  for (int i = 0; i < _VehicleTypes.size() ; i++)
  {
    if (_VehicleTypes.at(i)->_Name == nameType)
    {
      index = i;
      break;
    }
  }
  if (index < 0)
  {
    return 0;
  }

  TypeVehicule *tv = _VehicleTypes.at(index);
  Vehicule *v = new Vehicule(nameFin, tv);
  v->setMass(tv->_Mass);
  v->setWheelbase(tv->_Wheelbase);
  v->setMotorForce(tv->_MotorForce);
  addVehicle(v);
  return v;
}

Vehicule* Simulation::findVehicle(const QString & name)
{
  foreach(Vehicule *v, _Vehicles)
  {
    if (v && v->name() == name)
      return v;
  }
  return NULL;
}

void Simulation::addDriver(Driver *c)
{
  _Drivers.push_back(c);
}

Driver* Simulation::addDriver(const QString &name, const QString &nameType)
{
      int nbDrivers = _Drivers.size();
      QString startName = name;
      QString endName = startName;
      if (name.isEmpty())
      {
        startName = "Driver";
        endName = startName + QString::number(nbDrivers);
      }

      Driver *drv = findDriver(endName); // TODO
      int n = nbDrivers + 1;

      while (drv != NULL)
      {
        drv = findDriver(startName + QString::number(n));
        n++;
      }

      int index = -1;
      for (int i=0 ; i < _DriverTypes.size(); i++)
      {
        if (_DriverTypes.at(i)->_Name == nameType)
        {
          index = i;
          break;
        }
      }

      if (index < 0)
        return 0;

        // Apart from the two velocity values asssigned to straight and turn angles,
        // you can developed driver types here using elaborate behavioural models.
        // ...

      DriverType *driverType = _DriverTypes.at(index);
      Driver * driver = new Driver(endName);
      driver->_turnSpeedObj =  driverType->_TurnSpeed;
      driver->_defaultSpeed =  driverType->_StraightSpeed;
      addDriver(driver);

      return driver;
}

Driver* Simulation::findDriver(const QString &name)
{
  foreach(Driver *driver, _Drivers)
  {
    if (driver && driver->name() == name)
      return driver;
  }
  return NULL;
}


void Simulation::openXMLFileTypes(const QString &fileName)
{
  QDomDocument doc("Configuration");
  QFile file(fileName);
  if( !file.open( QIODevice::ReadOnly ))
  {
    return;
  }
  if( !doc.setContent(&file))
  {
    file.close();
    return;
  }
  file.close();
  _VehicleTypes.clear();
  _DriverTypes.clear();

  QDomNodeList vTypes = doc.elementsByTagName("VehicleType");
  for (int n1=0; n1<vTypes.size(); ++n1)
  {
    QDomElement elem1 = vTypes.at(n1).toElement();
    TypeVehicule *vt = new TypeVehicule();
    vt->_Name = elem1.attribute("name");

    QDomNodeList children = elem1.childNodes();
    for (int i = 0 ; i < children.size() ; ++i)
    {
      QDomElement elem2 = children.at(i).toElement();
      if (elem2.tagName() == "Image")
          vt->_NameImage = elem2.text();
      else if (elem2.tagName() == "Wheelbase")
        vt->_Wheelbase = (elem2.text().toFloat());
      else if (elem2.tagName() == "Mass")
        vt->_Mass = (elem2.text().toFloat());
      else if (elem2.tagName() == "MotForce")
        vt->_MotorForce = (elem2.text().toFloat());
    }
    _VehicleTypes.push_back(vt);
  }

  QDomNodeList cTypes = doc.elementsByTagName("DriverType");
  for (int n2 = 0 ; n2 < cTypes.size() ; ++n2)
  {
    QDomElement elem1 = cTypes.at(n2).toElement();
    DriverType *dt = new DriverType();
    dt->_Name = elem1.attribute("name");

    QDomNodeList children = elem1.childNodes();
    for (int i = 0 ; i < children.size() ; ++i)
    {
      QDomElement elem2 = children.at(i).toElement();
      if (elem2.tagName() == "TurnVelocity")
        dt->_TurnSpeed = elem2.text().toFloat();
      else if (elem2.tagName() == "StraightVelocity")
        dt->_StraightSpeed = elem2.text().toFloat();
    }
    _DriverTypes.push_back(dt);

  }

  return;
}

void Simulation::openXMLFileSimulation(const QString &fileName)
{

  _TpsSimulation.setHMS(0,0,0);

  QDomDocument doc("Simulation");
  QFile file(fileName);
  if( !file.open( QIODevice::ReadOnly ) )
      return;

  if( !doc.setContent( &file ) )
  {
    file.close();
    return;
  }
  file.close();

  QDomNodeList vehicles = doc.elementsByTagName("Vehicle");

  for (int i = 0 ; i < vehicles.size(); ++i)
  {
    QDomElement elementVehicule = vehicles.at(i).toElement();
    QString vehicleName = elementVehicule.attribute("name");
    QString typeVehicle = elementVehicule.attribute("type");
    Vehicule *newVehicule = addVehicle(vehicleName, typeVehicle);

    if (!newVehicule)
      return;

    QDomNodeList children = elementVehicule.childNodes();
    for (int i=0; i < children.size(); ++i)
    {
          QDomElement elem2 = children.at(i).toElement();
          if (elem2.tagName() == "Location")
          {
            QString valStr = elem2.text();
            double x = valStr.section(" ", 0, 0).toDouble();
            //cout.precision(std::numeric_limits< double >::max_digits10);
            //cout << "--> before:" << fixed << x << endl;
            qreal y = valStr.section(" ", 1, 1).toFloat();
            newVehicule->setLocation(Point(x,y));
          }
          else if (elem2.tagName() == "Direction")
          {
            newVehicule->setDirection(elem2.text().toFloat());
          }
    }

    QDomNodeList drivers = elementVehicule.elementsByTagName("Driver");

    for (int n4 = 0 ; n4 < drivers.size(); ++n4)
    {
              QDomElement elem2 = drivers.at(n4).toElement();
              QString typeDriver = elem2.attribute("type");

              Driver *newDriver = addDriver("Driver" + QString::number(n4), typeDriver);
              if (!newDriver)
                return;

              newDriver->use(newVehicule);

              QDomNodeList children = elem2.childNodes();
              for (int i = 0 ; i < children.size() ; ++i)
              {
                        QDomElement elem3 = children.at(i).toElement();
                        if (elem3.tagName() == "Lane")
                        {
                          QDomNodeList children2 = elem3.childNodes();
                          for (int j = 0 ; j < children2.size() ; ++j)
                          {
                                    QDomElement elem4 = children2.at(j).toElement();
                                    if (elem4.tagName().compare("Intersections",Qt::CaseInsensitive) == 0)
                                    {
                                      QString valStr = elem4.text();
                                      QStringList strList = valStr.split(" ", QString::SkipEmptyParts);
                                      for (int si = 0 ; si < strList.size() ; si++)
                                          newDriver->addCoords(strList.at(si));
                                    }
                                    else
                                    {
                                      QString valStr = elem4.text();
                                      qreal x = valStr.section(" ", 0, 0).toFloat();
                                      qreal y = valStr.section(" ", 1, 1).toFloat();
                                      newDriver->addCoords(Point(x,y));
                                    }
                          }
                        }
              }
    }
  }
  _simulationFile = fileName;
}

// End.
