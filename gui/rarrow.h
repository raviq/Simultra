#ifndef RARROW_H
#define RARROW_H

#include <math.h>

#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QColor>

class RArrow
{
public:
    RArrow( QPointF _raStartPoint,
            QPointF _raEndPoint,
            double  _raLineWidth,
            double  _raHeadHeight,
            double  _raHeadWidth,
            QColor  _raBorderColor,
            QColor  _raFillColor,
            double _raBorderThickness );

    void raDrawArrow(QPainter *);
    void raSetArrowHead();
    void raSetArrowLine();

public:
    QPointF raArrowPoints[7];
    QPointF raStartPoint;
    QPointF raEndPoint;
    double raLineWidth;
    double raHeadWidth;
    double raHeadHeight;
    QColor raBorderColor;
    QColor raFillColor;
    double raBorderThickness;
};

#endif // RARROW_H
