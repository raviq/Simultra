//
// Arrows utility
//
#include "rarrow.h"

RArrow::RArrow(QPointF _raStartPoint, QPointF _raEndPoint, double _raLineWidth, double _raHeadHeight, double _raHeadWidth, QColor _raBorderColor, QColor _raFillColor, double _raBorderThickness)
{
    raStartPoint = _raStartPoint;
    raEndPoint = _raEndPoint;
    raLineWidth = _raLineWidth;
    raHeadHeight = _raHeadHeight;
    raHeadWidth = _raHeadWidth;
    raBorderThickness = _raBorderThickness;
    raBorderColor = _raBorderColor;
    raFillColor = _raFillColor;
}

void RArrow::raSetArrowHead()
{
    double x1 = raStartPoint.x();
    double y1 = raStartPoint.y();
    double x2 = raEndPoint.x();
    double y2 = raEndPoint.y();

    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

    double dx = x2 + (x1 - x2) * raHeadHeight / distance;
    double dy = y2 + (y1 - y2) * raHeadHeight / distance;

    double k = raHeadWidth / raHeadHeight;

    double x2o = x2 - dx;
    double y2o = dy - y2;

    double x3 = y2o * k + dx;
    double y3 = x2o * k + dy;

    double x4 = dx - y2o * k;
    double y4 = dy - x2o * k;

    raArrowPoints[0] = QPointF(x4,y4);
    raArrowPoints[1] = QPointF(x2,y2);
    raArrowPoints[2] = QPointF(x3,y3);
}

void RArrow::raSetArrowLine()
{
    double x1 = raStartPoint.x();
    double y1 = raStartPoint.y();
    double x2 = (raArrowPoints[0].x()+raArrowPoints[2].x())/2.00000;
    double y2 = (raArrowPoints[0].y()+raArrowPoints[2].y())/2.00000;

    double k = raLineWidth / (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));

    double x2o = x2 - x1;
    double y2o = y1 - y2;

    double x3 = y2o * k + x1;
    double y3 = x2o * k + y1;

    double x4 = x1 - y2o * k;
    double y4 = y1 - x2o * k;

    double x1o = x1 - x2;
    double y1o = y2 - y1;

    double x5 = y1o * k + x2;
    double y5 = x1o * k + y2;

    double x6 = x2 - y1o * k;
    double y6 = y2 - x1o * k;

    raArrowPoints[4] = QPointF(x3,y3);
    raArrowPoints[5] = QPointF(x4,y4);
    raArrowPoints[6] = QPointF(x5,y5);
    raArrowPoints[3] = QPointF(x6,y6);
}

void RArrow::raDrawArrow(QPainter *p)
{
    p->setPen(QPen(QBrush(raBorderColor),raBorderThickness,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    p->setBrush(QBrush(raFillColor));

    raSetArrowHead();
    raSetArrowLine();

    p->setRenderHint(QPainter::Antialiasing, true);
    p->drawPolygon(raArrowPoints, 7);
}
