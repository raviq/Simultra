#include "mapobject.h"

#include <QGraphicsPixmapItem>


//mapObject::mapObject(const QPixmap &pix) : QObject(0), QGraphicsPixmapItem(pix)


mapObject::mapObject(QGraphicsItem *parent, QString name, QGraphicsScene *scene) :
    QObject(0), QGraphicsPixmapItem(0, scene)
{
    _name = name;
    _scene = scene;
}

