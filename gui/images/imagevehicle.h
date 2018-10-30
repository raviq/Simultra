#ifndef IMAGEVEHICULE_H
#define IMAGEVEHICULE_H
#include "point.h"
using namespace Utils;

class ImageVehicle
{
    public:
      ImageVehicle(const QString& name, const QString& imageName,
                   const Utils::Point& location, qreal direction,
                   const Point &acceleration, const Point &velocity,
                   const Point &motorForce, const Point &firctionsForce);

      QString name() const;
      QString imageName() const;
      Utils::Point location() const;
      qreal direction() const;
      Point acceleration() const;
      Point velocity() const;
      void forces(Point &motorForces, Point &frictions);

    protected:
      QString _Name, _NameImage;
      Utils::Point _Location;
      qreal _Direction;
      Point _Acceleration, _Velocity;
      Point _MotorForce, _FrictionsForce;
};

#endif
