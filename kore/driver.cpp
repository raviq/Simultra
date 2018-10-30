//
// Driver agent and behavioural model routines
//

#include "driver.h"
#include "simulation.h"
#include "plan.h"
#include "signalisation.h"
#include "utils.h"
#include "config.h"

#include <qmath.h>
#include <iostream>

using namespace Utils;
using namespace std;

DriverType::DriverType()
{
}

Driver::Coordinates::Coordinates(const Plan::Intersection *intersection, const Point &location):
    intersection(intersection), location(location)
{
}

Driver::Driver(const QString & name)
{
  _stopAtLight = false;
  _ModifiedCoords = false;
  _lastIntersection = NULL;
  _DestinationReached = true;
  _distanceToObjective = 0;
  _IdCurrentLane = 0;
  _Driver = NULL;
  _name = name;
  _directionObj = 0;
  _ObjSpeed = 0;
  _turnSpeedObj = 25;
  _previousSteeringWheelPosition =0;
  _currentRoad = NULL;
  _turnSoon = false;
  _defaultSpeed = 35.0;
  _PreviousTurnSpeed = 0.0;
}

QString Driver::name()
{
  return _name;
}

void Driver::use(Vehicule *vehicle)
{
  _Vehicle = vehicle;
  if (_Vehicle)
    _Vehicle->setDriver(this);
}

Point Driver::destination()
{
  return (_Path.isEmpty()) ? Point() : _Path.last().location;
}

void Driver::addCoords(const Point &p)
{
  _Path.push_back(Coordinates(NULL, p));
  _Destination = p;
}

void Driver::addCoords(const QString & nameIntersection)
{
  Simulation *simulation = Simulation::instance();
  const Plan::Intersection *i = simulation->_Plan->getIntersection(nameIntersection);
  if (i)
  {
    _Path.push_back(Coordinates(i, i->location()));
    _Destination = i->location();
  }
}

bool Driver::goTo(const Point &objectif, const TypeObjectif goalType)
{
  if (!_Vehicle || _DestinationReached)
   return false;

  bool ret = false;
  _ObjectifActuel = objectif;
  _distanceToObjective = (_ObjectifActuel - _Vehicle->location()).norm() * METRE_PER_UNIT;
  _directionObj = Point::angleBetweenTwoPoints(_Vehicle->location(), _ObjectifActuel);
  qreal urgent = false;

  if (goalType == COORDINATES)
  {
    if (_PreviousTurnSpeed > 0.0)
    {
      _ObjSpeed = _PreviousTurnSpeed;
    }
    else
    {
      // Recompute the speed
      Point next;
      if (_Path.size()==1)
      {
        _turnSoon = true;
      }
      else
      {
        next = _Path.at(1).location;
        qreal angle = Point::angleBetweenTwoPoints(_ObjectifActuel, next);
        qreal diffAngle = qAbs(Point::reinit0and180(angle - _Vehicle->direction().angle()));
        _turnSoon = (diffAngle > 20.0);
      }

      qreal distanceToSpeedVirageVoulue = _Vehicle->distanceToSpeed(_turnSpeedObj);
      if (_distanceToObjective < (30.0 * METRE_PER_UNIT))
      {
        _ObjSpeed = (_turnSoon) ? _turnSpeedObj : _defaultSpeed;
        _PreviousTurnSpeed = (_turnSoon) ? _turnSpeedObj : _defaultSpeed;
      }
      else if (_distanceToObjective <= distanceToSpeedVirageVoulue)
      {
        _ObjSpeed = (_turnSoon) ? _turnSpeedObj : _defaultSpeed;
        _PreviousTurnSpeed = (_turnSoon) ? _turnSpeedObj : _defaultSpeed;
      }
      else
      {
        _ObjSpeed = _defaultSpeed;
      }
    }
  }
  else if (goalType == OBSTACLE)
  {
    qreal distanceJusquaArret = _Vehicle->distanceToSpeed(0.0);
    if (_distanceToObjective < (15.0 * METRE_PER_UNIT))
    {
      _ObjSpeed = 0.0;
      urgent = true;
    }
    else if (_distanceToObjective <= distanceJusquaArret)
    {
      // Slow down
      _ObjSpeed = 5.0;
    }
    else
    {
      _ObjSpeed = _defaultSpeed;
    }
  }
  else if (goalType == LANE)
  {
    qreal diffAngle = qAbs(Point::reinit0and180(_directionObj - _Vehicle->direction().angle()));
    _ObjSpeed = (diffAngle > 20.0) ? _turnSpeedObj : _defaultSpeed;
  }
  drive(urgent);
  return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Behavior model of the driver
// Conditional statements, imperatives, rules extending the XML constraints.
//////////////////////////////////////////////////////////////////////////////////////////////////////

void Driver::drive(bool urgent)
{
      if (_Vehicle)
      {
            float speed = _Vehicle->speed(true),
                  AccelerationValue = 0.0,
                  BreakingValue = 0.0,
                  diffSpeed = 0.0f;

            diffSpeed = _ObjSpeed - speed;

            if (diffSpeed < -3.0f)
            {
              AccelerationValue = 0.0;
              BreakingValue = qAbs(diffSpeed * (urgent ? 100 : 3) / 100.0f);
            }
            else if (diffSpeed > 3.0f)
            {
              AccelerationValue = qAbs(diffSpeed * (urgent ? 100 : 3) / 100.0f);
              BreakingValue = 0.0;
            }
            else if (_ObjSpeed==0.0)
            {
              AccelerationValue = 0.0;
              BreakingValue = 1.0;
            }

            float diffAngle = _directionObj - _Vehicle->direction().angle();
            float valSteeringWheel = 0.0f;

            diffAngle = Point::reinit0and180(diffAngle);
            valSteeringWheel = 2 * (diffAngle) / 180.0;
            qreal diffSTRW=0;

            diffSTRW = valSteeringWheel - _previousSteeringWheelPosition;

            if(diffSTRW > .2)
              diffSTRW = .2;
            else if (diffSTRW < -.2)
              diffSTRW = -.2;

            valSteeringWheel = _previousSteeringWheelPosition + diffSTRW;

            if(valSteeringWheel > 1)
              valSteeringWheel = 1;
            else if (valSteeringWheel < -1)
              valSteeringWheel = -1;

            _previousSteeringWheelPosition = valSteeringWheel;
            _Vehicle->accelerate(AccelerationValue);
            _Vehicle->break_(BreakingValue);
            _Vehicle->turnsSteeringWheel(valSteeringWheel);
      }
}

bool Driver::predictCollisions(Point &collision)
{
  Simulation *simu = Simulation::instance();
  Point collisionPoint;
  bool certainCollision = false;
  qreal shortestDistance = 10000.0f;

  Point previousVehicle = _Vehicle->location() + (_Vehicle->direction()*_Vehicle->wheelbase());
  Vector v1(_Vehicle->location(), _Vehicle->direction());
  Vector v2(previousVehicle, _CurrentCoords-previousVehicle);
  _Driver = NULL;

  foreach (Driver *driver, simu->_Drivers)
  {
    if (driver && driver != this)
    {
      Vehicule *v = driver->_Vehicle;
      IntersectionInfo info = NO_INTERSECTION;

      QRect rect(0, 0, 20, 20);

      rect.moveCenter(v->location().toPoint());
      Point pts[2];

      InterVectorRectangle(v2, rect, pts, info);
      if (info == INTERSECTION)
      {
        qreal distanceToCollision = (previousVehicle-pts[0]).norm();
        if (distanceToCollision <= _MAX_DISTANCE_TO_COLLISION_)
        {
          if (distanceToCollision < shortestDistance)
          {
            if (driver->_Driver != this)
            {
              _Driver = driver;
              collision = pts[0];
              shortestDistance = distanceToCollision;
              certainCollision = true;
            }
          }
        }
      }

      InterVectorRectangle(v1, rect, pts, info);
      if (info == INTERSECTION)
      {
        qreal distanceToCollision = (previousVehicle-pts[0]).norm();
        if (distanceToCollision <= _MAX_DISTANCE_TO_COLLISION_)
        {
          if (distanceToCollision < shortestDistance)
          {
            if (driver->_Driver != this)
            {
              _Driver = driver;
              collision = pts[0];
              shortestDistance = distanceToCollision;
              certainCollision = true;
            }
          }
        }
      }

      Vector v3(v->location(), v->direction());
      collisionPoint = InterVectorVector(v2, v3, info);

      if (info == INTERSECTION)
      {
        qreal myCollisionDistance = (previousVehicle-collisionPoint).norm() * METRE_PER_UNIT;
        qreal theirdistancetoCollision = (v->location()-collisionPoint).norm()* METRE_PER_UNIT;
        qreal mySpeed = _Vehicle->speed(), leurVitesse = v->speed();
        qreal timeToCollision=0.0, theirDurationToCollision=0.0;
        qreal shortestDuration = _SHORTEST_DURATION_;

        timeToCollision = (mySpeed > 0.0) ? myCollisionDistance/mySpeed : _SHORTEST_DURATION_;
        theirDurationToCollision = (leurVitesse > 0.0) ? theirdistancetoCollision/leurVitesse : 10000.0;
        qreal diffTemps = qAbs(timeToCollision-theirDurationToCollision);

        if (timeToCollision <= 2 && diffTemps <= 2)
        {
          if (myCollisionDistance < shortestDistance)
          {
            if(diffTemps < shortestDuration)
            {
              if (driver->_Driver != this && theirdistancetoCollision < myCollisionDistance)
              {
                _Driver = driver;
                collision = collisionPoint;
                shortestDistance = myCollisionDistance;
                certainCollision = true;
                shortestDuration = diffTemps;
              }
            }
          }
        }
      }
    }
  }
  return certainCollision;
}

bool Driver::verifyLane(Point &laneObjective)
{
  laneObjective = _currentLane._Segment.nearestPoint(_Vehicle->location());
  qreal dist = (laneObjective-_Vehicle->location()).norm();
  if (dist >= LANE_WIDTH/2)
  {
    Point v = (_CurrentCoords-laneObjective);
    v.normalise();
    laneObjective = laneObjective + (v * 5.0);
    return true;
  }
  return false;
}

bool Driver::verifyLight(Point &lightPoint)
{
  if (!_currentRoad || _Path.size()<2)
    return false;

  Simulation *simu = Simulation::instance();
  Plan *plan = simu->_Plan;

  Signalisation *lights = simu->_Signalisations.value(_Path.at(0).intersection);
  if (lights)
  {
    Signalisation::LIGHT_COLOUR c = lights->currentState()->lightColor(
        _currentRoad, _IdCurrentLane,
        plan->getRoad(_Path.at(0).intersection, _Path.at(1).intersection), _IdCurrentLane);
    if (c==Signalisation::ORANGE || c==Signalisation::RED)
    {
      Point v = _currentLane._Segment.nearestPoint(_Vehicle->location())-_CurrentCoords;
      v.normalise();
      lightPoint = _CurrentCoords;
      return true;
    }
  }
  return false;
}

void Driver::changeCoordinates()
{
  Simulation *simu = Simulation::instance();
  Plan *plan = simu->_Plan;

  const Plan::Intersection *inter = _Path.at(0).intersection;

  if (!inter)
  {
    _CurrentCoords = _Path.at(0).location;
    _ModifiedCoords = true;
    _lastIntersection = NULL;
    return;
  }

  if (!_lastIntersection)
    _currentRoad = plan->getRoad(_Vehicle->location());
  else
    _currentRoad = plan->getRoad(_lastIntersection, inter);

  if (!_currentRoad)
    return;

  Plan::Road::Lane::Direction dir = _currentRoad->getDirection(inter);
  _IdCurrentLane = 0;

  bool resultOK = false;
  _currentLane = _currentRoad->lane(dir, _IdCurrentLane, resultOK);
  if (!resultOK)
    return;

  _CurrentCoords = ((dir == Plan::Road::Lane::FIRST_TO_SECOND)) ? _currentLane._Segment.start() : _currentLane._Segment.end();

  Point v = _currentLane._Segment.nearestPoint(_Vehicle->location())-_CurrentCoords;
  v.normalise();
  _CurrentCoords = _CurrentCoords + (v * 5.0f);
  _ModifiedCoords = true;
}


QList<Driver::Coordinates> Driver::get_Path()
{
    return _Path;
}

void Driver::move()
{
  _DestinationReached = _Path.isEmpty();
  if (!_DestinationReached)
  {
    Point collisionPoint, lightPoint, lanePoint;
    bool certainCollision = false,
         inLane           = false,
         signalisation    = false;
    qreal distancetoCollision = 999.0f,
          distancetoLane =      999.0f;

    if (!_ModifiedCoords)
      changeCoordinates();

    qreal distancetoPoint = (_CurrentCoords - _Vehicle->location()).norm() * METRE_PER_UNIT;

    if (distancetoPoint <= LIGHT_DISTANCE && !_stopAtLight)
    {
      _lastIntersection = _Path.at(0).intersection;
      _Path.pop_front();
      _ModifiedCoords = false;
      _PreviousTurnSpeed = 0.0;
      _turnSoon = false;
      return;
    }

    if (predictCollisions(collisionPoint))
    {
      certainCollision = true;
      Point versVehicule = _Vehicle->location() - collisionPoint;
      distancetoCollision = versVehicule.norm();
      versVehicule.normalise();
      if (distancetoCollision >= 10.0f)
        collisionPoint =  collisionPoint + (versVehicule * 10.0f);
      else
        collisionPoint = _Vehicle->location();
      distancetoCollision = (_Vehicle->location() - collisionPoint).norm();
    }

    if (_Path.at(0).intersection && verifyLane(lanePoint))
    {
      inLane = true;
      distancetoLane = (_Vehicle->location() - lanePoint).norm();
    }

    if (verifyLight(lightPoint))
    {
      signalisation= true;
      _stopAtLight = true;
    }
    else
      _stopAtLight = false;

    if (certainCollision && distancetoCollision < distancetoLane && distancetoCollision <= distancetoPoint + _COLLISION_DISTANCE_) // TODO
      goTo(collisionPoint, OBSTACLE);
    else if (inLane && distancetoLane < distancetoCollision)
      goTo(lanePoint, LANE);
    else if (signalisation)
      goTo(lightPoint, OBSTACLE);
    else
      goTo(_CurrentCoords, COORDINATES);
  }
  else
  {
    _ObjSpeed = 0.0;
    drive(false);
  }
}
