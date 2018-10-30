//
// vehicle image object
//
#include "vehimob.h"
#include "utils.h"
#include "config.h"

// TODO add the velocity vectors, etc.

VehicleImageObject::VehicleImageObject(const QImage &img, MapGraphicsObject *parent) : MapGraphicsObject(false, parent), _img(img)
{
    _scale =  _INITIAL_SCALE_;
    // QPointF(-width/2,-height/2) is the top left corner (ie. 0,0)
    _sizeRect  = QRectF(_scale * _img.width()  * -0.5,
                        _scale * _img.height() * -0.5,
                        _scale * _img.width(),
                        _scale * _img.height());
}

QRectF VehicleImageObject::boundingRect() const
{
    return _sizeRect;
}

qreal VehicleImageObject::get_Scale()
{
    return _scale;

}
int VehicleImageObject::get_Type()
{
    return _type;
}

void VehicleImageObject::set_Type(int type)
{
    _type = type;
}

void VehicleImageObject::set_Scale(qreal scale)
{
    _scale = scale;
    // update _sizeRect
    _sizeRect  = QRectF(_scale * _img.width()  * -0.5,
                        _scale * _img.height() * -0.5,
                        _scale * _img.width(),
                        _scale * _img.height());
}

void VehicleImageObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawImage(boundingRect(), _img);
}


void VehicleImageObject::renderVelocity(const bool render)
{
     UNUSED(render);
}

void VehicleImageObject::renderMotForce(const bool render)
{
    UNUSED(render);
}

void VehicleImageObject::renderFriction(const bool render)
{
    UNUSED(render);
}

void VehicleImageObject::setImageVehicle(QImage image)
{
    _img = image;
}

void VehicleImageObject::setVector(QPointF source, QPointF destination, QGraphicsLineItem** vect, QGraphicsPolygonItem** arrow, QColor colour)
{
    UNUSED(source);
    UNUSED(destination);
    UNUSED(vect);
    UNUSED(arrow);
    UNUSED(colour);
}

