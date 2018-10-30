#ifndef NODE_H
#define NODE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QList>

#define Q_DECL_OVERRIDE override

class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Node : public QGraphicsItem
{
public:
    Node();

    QPointF getPos();
    QList<Edge *> edges() const;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    enum { Type = UserType + 1 };

    int type() const Q_DECL_OVERRIDE { return Type; }
    bool advance_();
    void addEdge(Edge *edge);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;


private:
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
};

#endif // NODE_H
