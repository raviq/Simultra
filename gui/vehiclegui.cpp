#include "vehiclegui.h"
#include "utils.h"
#include <QPen>
#include <iostream>
#include "rarrow.h"

using namespace std;

VehicleGUI::VehicleGUI(const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap)
{
    setZValue(Utils::VEHICLE_LAYER);
    _Scale = 0.006f;

   // scaling
   setScale(_Scale);

  //translate
   QTransform t = this->transform();
   t.translate(-pixmap.width()/2.0, -pixmap.height()/2.0);
   this->setTransform(t);

  setTransformOriginPoint(pixmap.width()/2.0, pixmap.height()/2.0);

  // Velocity vector (red)
  _VelocityVector = new QGraphicsLineItem(this);
  _VelocityVector->setPen(QPen(Qt::red, _Scale));
  _VelocityVector->setZValue(Utils::ANNOTATION_LAYER+1);

  // velocity arrow
  _VelocityVectorArrow = new QGraphicsPolygonItem(this);
  _VelocityVectorArrow->setPen(QPen(Qt::red, _Scale));
  _VelocityVectorArrow->setZValue(Utils::ANNOTATION_LAYER+1);

  // "force friction" (blue)
  _FrictionsVector = new QGraphicsLineItem(this);
  _FrictionsVector->setPen(QPen(Qt::blue, _Scale));
  _FrictionsVector->setZValue(Utils::ANNOTATION_LAYER+2);

  // arrow
  _FrictionsVectorArrow = new QGraphicsPolygonItem(this);
  _FrictionsVectorArrow->setPen(QPen(Qt::blue, _Scale));
  _FrictionsVectorArrow->setZValue(Utils::ANNOTATION_LAYER+2);

  // "motive force" vector (green)
  _MotorForceVector = new QGraphicsLineItem(this);
  _MotorForceVector->setPen(QPen(Qt::green, _Scale));
  _MotorForceVector->setZValue(Utils::ANNOTATION_LAYER+3);

  // arrow
  _MotorForceVectorArrow = new QGraphicsPolygonItem(this);
  _MotorForceVectorArrow->setPen(QPen(Qt::green, _Scale));
  _MotorForceVectorArrow->setZValue(Utils::ANNOTATION_LAYER+3);

}

void VehicleGUI::renderVelocity(const bool print)
{
     if(_VelocityVector)
     {
         _VelocityVector->setVisible(print);
         _VelocityVectorArrow->setVisible(print);
     }
}

void VehicleGUI::renderFrictions(const bool print)
{
      if(_FrictionsVector)
     {
         _FrictionsVector->setVisible(print);
         _FrictionsVectorArrow->setVisible(print);
     }
}

void VehicleGUI::renderMotorForce(const bool print)
{
      if(_MotorForceVector)
      {
          _MotorForceVector->setVisible(print);
          _MotorForceVectorArrow->setVisible(print);
      }
}

// set the vector and its arrow
void VehicleGUI::setVector(QPointF source,
                            QPointF destination,
                            QGraphicsLineItem** vect,
                            QGraphicsPolygonItem** arrow,
                            QColor colour)
{
    // Set the line
    qreal rescaling_line = 3.0;   // rescaling the arrow line
    QLineF line (source, destination.operator *=(rescaling_line));
    (*vect)->setPen(QPen(colour, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    (*vect)->setLine(line);


    // qFuzzyCompare: compares 2 floating points and returns true if they are considered equal, otherwise false.
    if (qFuzzyCompare(line.length(), qreal(0.)))
    {
        return;
    }

    // Set the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2*M_PI - angle;

    qreal arrowSize = 90.0;
    QPointF sourceArrowP1 = source + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = source + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF destArrowP1 = destination + QPointF(sin(angle - M_PI / 3) * arrowSize, cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destination + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize, cos(angle - M_PI + M_PI / 3) * arrowSize);

    (*arrow)->setBrush(colour);
    (*arrow)->setPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    (*arrow)->setPolygon(QPolygonF() << line.p2() << destArrowP1   << destArrowP2);

}

// add all the information contained in this object
// VehicleGUI to ImageVehicle image

void VehicleGUI::setImageVehicle(ImageVehicle* image)
{
      if (!image)
        return;

      setPos(image->location());
      setRotation(image->direction());

      if(_VelocityVector)
      {
          QPointF sourcePoint(mapFromScene(image->location()));
          QPointF destPoint(mapFromScene(image->location() + image->velocity()));
          setVector(sourcePoint, destPoint, &_VelocityVector, &_VelocityVectorArrow, Qt::red);
      }


      Point motor, frictions;
      image->forces(motor, frictions);

      motor *= 0.01;
      if(_MotorForceVector)
      {
        QPointF sourcePoint(mapFromScene(image->location()));
        QPointF destPoint(mapFromScene((image->location() + motor)));

        QLineF line (sourcePoint, destPoint);
        _MotorForceVector->setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _MotorForceVector->setLine(line);

      }

      frictions *= 0.01;
      if(_FrictionsVector)
      {
        QPointF sourcePoint(mapFromScene(image->location()));
        QPointF destPoint(mapFromScene(image->location() + frictions));

        qreal rescaling_line = 0.06;   // rescaling the arrow line
        QLineF line (sourcePoint, destPoint.operator *=(rescaling_line));
        _FrictionsVector->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _FrictionsVector->setLine(line);
      }
} // end
