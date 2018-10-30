#include "imagevehicle.h"


ImageVehicle::ImageVehicle(const QString &name,
                           const QString &imageName,
                           const Utils::Point &location,
                           qreal direction,
                           const Point &acceleration,
                           const Point &velocity,
                           const Point &motorForce,
                           const Point &firctionsForce)
  : _Name(name),
    _NameImage(imageName),
    _Location(location),
    _Direction(direction),
    _Acceleration(acceleration),
    _Velocity(velocity),
    _MotorForce(motorForce),
    _FrictionsForce(firctionsForce)
{
}

QString ImageVehicle::name() const
{
  return _Name;
}

QString ImageVehicle::imageName() const
{
  return _NameImage;
}

Utils::Point ImageVehicle::location() const
{
  return _Location;
}

qreal ImageVehicle::direction() const
{
  return _Direction;
}

Point ImageVehicle::acceleration() const
{
  return _Acceleration;
}

Point ImageVehicle::velocity() const
{
  return _Velocity;
}

void ImageVehicle::forces(Point &motorForces, Point &frictions)
{
  motorForces = _MotorForce;
  frictions = _FrictionsForce;
}
