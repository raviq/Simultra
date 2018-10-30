#include "pedestriangui.h"
#include "utils.h"
#include "rarrow.h"

#include <QPen>
#include <iostream>

using namespace std;

PedestrianGUI::PedestrianGUI(const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap)
{
    setZValue(Utils::VEHICLE_LAYER);
    _Scale = 0.008f;

   // Scaling
   setScale(_Scale);

  // Translate
  QTransform t = this->transform();
  t.translate(-pixmap.width()/2.0, -pixmap.height()/2.0);
  this->setTransform(t);

   setTransformOriginPoint(pixmap.width()/2.0, pixmap.height()/2.0);

  // Velocity vector (red)
  _VelocityVector = new QGraphicsLineItem(this);
  _VelocityVector->setPen(QPen(Qt::red, _Scale));
  _VelocityVector->setZValue(Utils::ANNOTATION_LAYER+1);
  // Arrow
  _VelocityVectorArrow = new QGraphicsPolygonItem(this);
  _VelocityVectorArrow->setPen(QPen(Qt::red, _Scale));
  _VelocityVectorArrow->setZValue(Utils::ANNOTATION_LAYER+1);

  // "force friction/frot?" (blue)
  _FrictionsVector = new QGraphicsLineItem(this);
  _FrictionsVector->setPen(QPen(Qt::blue, _Scale));
  _FrictionsVector->setZValue(Utils::ANNOTATION_LAYER+2);
  // Arrow
  _FrictionsVectorArrow = new QGraphicsPolygonItem(this);
  _FrictionsVectorArrow->setPen(QPen(Qt::blue, _Scale));
  _FrictionsVectorArrow->setZValue(Utils::ANNOTATION_LAYER+2);

  // "motive force" vector (green)
  _MotorForceVector = new QGraphicsLineItem(this);
  _MotorForceVector->setPen(QPen(Qt::green, _Scale));
  _MotorForceVector->setZValue(Utils::ANNOTATION_LAYER+3);
  // Arrow
  _MotorForceVectorArrow = new QGraphicsPolygonItem(this);
  _MotorForceVectorArrow->setPen(QPen(Qt::green, _Scale));
  _MotorForceVectorArrow->setZValue(Utils::ANNOTATION_LAYER+3);

}

void PedestrianGUI::renderVelocity(const bool print)
{
 if(_VelocityVector)
 {
     _VelocityVector->setVisible(print);
     _VelocityVectorArrow->setVisible(print);
 }
}

void PedestrianGUI::renderFrictions(const bool print)
{
  if(_FrictionsVector)
 {
     _FrictionsVector->setVisible(print);
     _FrictionsVectorArrow->setVisible(print);
 }
}

void PedestrianGUI::renderMotorForce(const bool print)
{
  if(_MotorForceVector)
  {
      _MotorForceVector->setVisible(print);
      _MotorForceVectorArrow->setVisible(print);
  }
}

// Set the vector and its arrow

void PedestrianGUI::setVector(  QPointF source,
                                QPointF destination,
                                QGraphicsLineItem** vect,
                                QGraphicsPolygonItem** arrow,
                                QColor colour)
{
    // Set the line
    QLineF line (source, destination);
    (*vect)->setPen(QPen(colour, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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

    qreal arrowSize = 10.0;
    QPointF sourceArrowP1 = source + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = source + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF destArrowP1 = destination + QPointF(sin(angle - M_PI / 3) * arrowSize, cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destination + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize, cos(angle - M_PI + M_PI / 3) * arrowSize);

    (*arrow)->setBrush(colour);
    (*arrow)->setPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    (*arrow)->setPolygon(QPolygonF() << line.p2() << destArrowP1   << destArrowP2);

}

// Add all the information contained in this object
// PedestrianGUI to ImageVehicle image

void PedestrianGUI::setImageVehicle(ImageVehicle* image)
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
    //setVector(sourcePoint, destPoint, &_MotorForceVector, &_MotorForceVectorArrow, Qt::green);
  }

  frictions *= 0.01;
  if(_FrictionsVector)
  {
    QPointF sourcePoint(mapFromScene(image->location()));
    QPointF destPoint(mapFromScene(image->location() + frictions));

    QLineF line (sourcePoint, destPoint);
    _FrictionsVector->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _FrictionsVector->setLine(line);

    //setVector(sourcePoint, destPoint, &_FrictionsVector, &_FrictionsVectorArrow, Qt::blue);
  }


}
// End.
