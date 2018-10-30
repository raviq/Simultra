//
// Routing node
//

#include "edge.h"
#include "node.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include "utils.h"

Node::Node()
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(Utils::VEHICLE_LAYER);
}

QPointF Node::getPos()
{
    return pos();
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

bool Node::advance_()
{
    if (newPos == pos())
        return false;
    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 10, 10);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
//  painter->setBrush(Qt::darkBlue);
    painter->drawEllipse(-7, -7, 10, 10);
    painter->setOpacity(0.7);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken)
    {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::red).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    }
    else
    {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
        case ItemPositionHasChanged:
            foreach (Edge *edge, edgeList)
                edge->adjust();

            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}
