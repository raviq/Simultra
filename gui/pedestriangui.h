#ifndef PEDESTRIANGUI_H
#define PEDESTRIANGUI_H

#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include "point.h"
#include "imagevehicle.h"

class PedestrianGUI : public QGraphicsPixmapItem
{

    public:
      PedestrianGUI(const QPixmap &pixmap);
      void renderVelocity(const bool render);
      void renderMotorForce(const bool render);
      void renderFrictions(const bool render);
      void setImageVehicle(ImageVehicle* image);
      void setVector(QPointF source, QPointF destination, QGraphicsLineItem** vect, QGraphicsPolygonItem** arrow, QColor colour);

    private:
      qreal _Scale;
      QGraphicsLineItem* _VelocityVector;
      QGraphicsPolygonItem* _VelocityVectorArrow;

      QGraphicsLineItem* _MotorForceVector;
      QGraphicsPolygonItem* _MotorForceVectorArrow;

      QGraphicsLineItem* _FrictionsVector;
      QGraphicsPolygonItem* _FrictionsVectorArrow;

};

#endif // PEDESTRIANGUI_H
