#ifndef VEHIMOB_H
#define VEHIMOB_H

#include "MapGraphics/MapGraphicsObject.h"

#include <QImage>

class VehicleImageObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit VehicleImageObject(const QImage& img, MapGraphicsObject *parent = 0);

    // Pure-virtual from MapGraphicsObject
    virtual QRectF boundingRect() const;

    // Pure-virtual from MapGraphicsObject
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget=0);

    int get_Type();
    void set_Type(int type);
    void set_Scale(qreal scaling);
    qreal get_Scale();

    // Imported
    void renderVelocity(const bool render);
    void renderMotForce(const bool render);
    void renderFriction(const bool render);
    void setImageVehicle(QImage image);
    void setVector(QPointF source,
                   QPointF destination,
                   QGraphicsLineItem** vect,
                   QGraphicsPolygonItem** arrow, QColor colour);
private:
    QImage _img;
    QRectF _sizeRect;
    qreal _scale;
    int _type;

};

#endif // VEHIMOB_H
