#ifndef DRIVER_H
#define DRIVER_H

#include <QString>
#include <QList>
#include "vehicle.h"
#include "plan.h"

using namespace Utils;

class DriverType
{
public:
  DriverType();
  QString _Name;
  qreal _TurnSpeed;
  qreal _StraightSpeed;
};

class Driver
{
public:
  enum TypeObjectif
  {
    COORDINATES,
    OBSTACLE,
    LANE
  };

  Driver(const QString &name);
  QString name();
  Point destination();

  void addCoords(const Point &p);
  void addCoords(const QString &intersectionName);
  void move();
  bool goTo(const Point &objectif, const TypeObjectif TypeObjectif=COORDINATES);
  void changeCoordinates();
  bool predictCollisions(Point &collision);
  void use(Vehicule *vehicle);
  bool verifyLight(Point &lightPoint);
  bool verifyLane(Point &laneObjective);

  bool _stopAtLight;
  Point _CurrentCoords;
  Driver *_Driver;
  bool _ModifiedCoords;
  qreal _distanceToObjective;
  const Plan::Intersection *_lastIntersection;
  unsigned int _IdCurrentLane;
  Point _ObjectifActuel;
  qreal _directionObj;
  qreal _ObjSpeed;
  qreal _turnSpeedObj;
  qreal _previousSteeringWheelPosition;
  const Plan::Road *_currentRoad;
  Vehicule *_Vehicle;
  qreal _defaultSpeed;
  Plan::Road::Lane _currentLane;

  class Coordinates
  {
    public:
        Coordinates(const Plan::Intersection *intersection, const Point &location);
        const Plan::Intersection *intersection;
        Point location;
  };

  QList<Coordinates> get_Path();

private:
  void drive(bool urgent);
  Point _Destination;
  bool _DestinationReached;
  QString _name;
  bool _turnSoon;
  qreal _PreviousTurnSpeed;

  QList<Coordinates> _Path;

};

#endif // DRIVER_H
