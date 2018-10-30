//
// Vehicle (physical) entity
//

#include <qmath.h>
#include <iostream>
#include "vehicle.h"
#include "simulation.h"
#include "utils.h"
#include "config.h"
#include "debug.h"

using namespace Utils;
using namespace std;

Vehicule::Vehicule(const QString &name, TypeVehicule *type)
{
  _Name        = name;
  _Type        = type;
  _Wheelbase   = _WHEELBASE_;
  _Driver      = NULL;
  _Break       = false;
  _horizontal  = true;

//  setVelocity(Point(.9, .9));
  setDirection(0.1f);
  setMass(1000.0f);
  setMotorForce(9800);
}

Point Vehicule::acceleration()
{
  return _Acceleration;
}

void Vehicule::setAcceleration(const Point & value)
{
  _Acceleration = value;
}

void Vehicule::setDirection(const qreal angle)
{
  qreal newAngle = angle;
  Point::reinit0and180(newAngle);
  newAngle *= (M_PI/180.0);
  _Direction.setX(qCos(newAngle));
  _Direction.setY(qSin(newAngle));
}

void Vehicule::setDirection(const Point & value)
{
  Point normalise = value;
  normalise.normalise();
  _Direction = normalise;
}

void Vehicule::set_path(QPainterPath path)
{
    _Path = path;
}

QPainterPath Vehicule::get_path()
{
    return _Path;
}

Point Vehicule::direction()
{
  return _Direction;
}

void Vehicule::setWheelbase(const qreal value)
{
  _Wheelbase = value;
}

qreal Vehicule::wheelbase()
{
  return _Wheelbase;
}

void Vehicule::setLocation(const Point &value)
{
  _Location = value;
}

Point Vehicule::location()
{
  return _Location;
}

void Vehicule::setMotorForce(const qreal value)
{
  _AccelerationMax = value;
}

void Vehicule::setMass(const qreal value)
{
  _Mass = value;
}

QString Vehicule::name()
{
  return _Name;
}

void Vehicule::set_scene(QGraphicsScene* scene)
{
    _Scene = scene;
}

void Vehicule::setType(TypeVehicule *type)
{
  _Type = type;
}

TypeVehicule* Vehicule::type()
{
  return _Type;
}

void Vehicule::setVelocity(const Point & value)
{
  if(value.norm() < .001)
    _Velocity = Point(0,0);
  else
    _Velocity = value;
}

Point Vehicule::velocity()
{
  return _Velocity;
}

qreal Vehicule::speed(bool kmh)
{
  if(kmh)
    return (_Velocity.norm() * MPS_A_KMH);
  else
    return _Velocity.norm();
}

void Vehicule::forces(Point &moteur, Point &frictions)
{
  moteur = _MotorForce;
  frictions = _FrictionsForce;
}

void Vehicule::setDriver(Driver *driver)
{
  _Driver = driver;
}

Driver* Vehicule::driver()
{
  return _Driver;
}

qreal Vehicule::timeToSpeed(const qreal speedKmh)
{
  qreal diffSpeed = (speed(true)-speedKmh) * KMH_A_MPS;
  return ( (diffSpeed / (_AccelerationMax)) * _Mass );
}


qreal Vehicule::distanceToSpeed(const qreal speedKmh)
{
  qreal secs = timeToSpeed(speedKmh);
  return ( 0.50 * _Velocity.norm() * (secs-(Simulation::_simulationStep*3)) ) * UNIT_PER_METRE;
}

//  -1 = left, +1 = right.
void Vehicule::turnsSteeringWheel(qreal value)
{
  if ( value < -1.0 )
    value = -1.0;
  else if ( value > 1.0 )
    value = 1.0;
  _AngleSteeringwheel = value;
}

// 0 = no break, 1 = break fully pressed.
void Vehicule::break_(qreal value)
{
  if ( value < 0.0f )
    value = 0.0f;
  else if ( value > 1.0f )
    value = 1.0f;

  _Break = (value > 0.0f);
}

// 0 = no accelaration, 1 = accelerator fully pressed.
void Vehicule::accelerate(qreal value)
{
  if (value<0)
      value = 0;
  else if (value>1.0)
      value = 1.0;
  _MotorForce = _Direction * (value * _AccelerationMax);
}

void Vehicule::move()
{
  updateForces();
  updateAcceleration();
  updateVelocity();
  updateDirection();
  updateLocation();
}

void Vehicule::updateAcceleration()
{
   // Newton's second law: sum f = ma
  Point force = _MotorForce + _FrictionsForce;
  setAcceleration((force / _Mass));
}

void Vehicule::updateDirection()
{
  float turnRadius = _Wheelbase / sin(_AngleSteeringwheel * M_PI / 3);
  float angularSpeed = (_Velocity.norm() * UNIT_PER_METRE) / turnRadius;
  setDirection(_Direction.angle() + angularSpeed * Simulation::_simulationStep * 180 / M_PI);
}

void Vehicule::updateForces()
{
    _FrictionsForce = (_Break) ? _Direction * (- _AccelerationMax) : Point();
}

void Vehicule::updateVelocity()
{
    cout.precision(std::numeric_limits< double >::max_digits10);
    setVelocity((_Direction * _Velocity.norm()) + _Acceleration * Simulation::_simulationStep);
  //  | velocity_angle - direction_angle | > pi/2
  if (qAbs(_Velocity.angle(true) - _Direction.angle(true)) >= 90.0f)
    setVelocity(Point());
}

void Vehicule::updateLocation()
{
# if 0
    cout << "__________________________________________________________________________________________" << endl;
    cout << "   _Location   +=   "<< _Velocity.norm() << " * "<< UNIT_PER_METRE <<" * " << Simulation::_simulationStep << endl;
    cout << "   _Accelerat   =  " << fixed << _Acceleration.x() << ", " << _Acceleration.y() << endl;
    cout << "   _Direction   =  " << fixed << _Direction.x() << ", " << _Direction.y() << endl;
    cout << "   _ForceFrott  =  " << fixed << _FrictionsForce.x() << ", " << _FrictionsForce.y() << endl;
    cout << "   _MotorForce =  " << fixed << _MotorForce.x() << ", " << _MotorForce.y() << endl;
    cout << "   _Mass       =  " << fixed << _Mass << endl;
#endif

    setLocation(_Location + _Velocity * UNIT_PER_METRE * Simulation::_simulationStep);
}

void Vehicule::set_OSMvehicle(VehicleImageObject* vehicle, bool alignment ) // alignment: walker use.
{
    _OSMvehicle = vehicle;
    _horizontal = alignment;
}

void Vehicule::update_OSMvehicle_Location(Point pos)
{
     _OSMvehicle->setLatitude(pos.x());
     _OSMvehicle->setLongitude(pos.y());
    if (_horizontal)
        _OSMvehicle->setRotation(_Direction.angle(false) - 90); // TODOstat -90
}
