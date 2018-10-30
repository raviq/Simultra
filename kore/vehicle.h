#ifndef VEHICULE_H
#define VEHICULE_H

#include <QList>
#include <QTime>
#include <QString>
#include <QPainterPath>
#include <QGraphicsScene>
#include "point.h"
#include "vehimob.h"

class Driver;

using namespace Utils;

class TypeVehicule
{
    public:
      qreal _Wheelbase;
      qreal _MotorForce;
      qreal _Mass;
      QString _Name;
      QString _NameImage;
};


class Vehicule
{
    public:
      enum DriverActionType
      {
        ACCELERER,
        BREAKING,
        TOURNER
      };

      struct DriverAction
      {
        DriverActionType action;
        qreal value;
        QTime hour;
      };

      void set_scene(QGraphicsScene* scene);

      Vehicule(const QString &name, TypeVehicule *type=0);
      void setAcceleration(const Point &value);
      Point acceleration();
      void setDirection(const qreal angle);
      void setDirection(const Point &value);
      Point direction();
      qreal distanceToSpeed(const qreal speedKmh = 0.0);
      void setLocation(const Point &value);
      Point location();
      void setWheelbase(const qreal value);
      qreal wheelbase();
      void forces(Point & engine, Point &frictions);
      void setMotorForce(const qreal value);
      void setMass(const qreal value);
      QString name();
      TypeVehicule* type();
      void setType(TypeVehicule *type);
      void setVelocity(const Point &value);
      Point velocity();
      qreal speed( bool kmh = false );
      qreal timeToSpeed(const qreal speedKmh = 0.0);
      Driver* driver();
      void setDriver(Driver* c);
      void accelerate(qreal value);
      void move();
      void break_(qreal value);
      void turnsSteeringWheel(qreal value);

      void set_path(QPainterPath path);
      QPainterPath get_path();
//      void set_path_(QList<Driver::Coordinates> path);

      void set_OSMvehicle(VehicleImageObject* vehicle, bool alignment );
      void update_OSMvehicle_Location(Point pos);

      VehicleImageObject* _OSMvehicle;

    private:
      void updateAcceleration();
      void updateDirection();
      void updateLocation();
      void updateForces();
      void updateVelocity();

      QPainterPath _Path;
      Point _Acceleration;
      qreal _AccelerationMax;
      float _AngleSteeringwheel;
      Point _Direction;
      Point _Location;
      Point _FrictionsForce;
      Point _MotorForce;
      float _Wheelbase;
      Point _Velocity;
      bool  _Break;
      float _Mass;
      QString _Name;
      bool _horizontal;

      Driver *_Driver;
      TypeVehicule *_Type;
      QGraphicsScene* _Scene;

};

#endif // VEHICULE_H
