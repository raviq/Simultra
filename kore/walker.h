#ifndef WALKER_H
#define WALKER_H

#include <QGraphicsItem>
#include <QGraphicsView>

class Walker : public QGraphicsItem
{
public:
    Walker(QColor col, qreal scale);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getName();
    void setName(QString _name);
    void setView(QGraphicsView* _view);
    void set_walk(QPointF* array, int size);

protected:
    void advance_(int step);

private:
    int index   = 0;
    int _nwalk;
    qreal _scale;
    qreal angle;
    qreal speed;
    qreal eyeDirection;
    qreal epsilon;
    qreal alpha   = 0.0;
    QColor colour;
    QString name;
    QPointF position;
    QPointF* _walk;
    QGraphicsView* view;

    // Simulation* model;
};


#endif // WALKER_H


