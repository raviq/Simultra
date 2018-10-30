#ifndef POINT_H
#define POINT_H

#include <QPoint>
#include <QString>

using namespace std;

namespace Utils
{

    class Point : public QPointF
    {
    public:
        Point();
        Point(qreal x, qreal y);
        Point(const QPoint& point);
        Point(const QPointF& point);

        Point operator/(qreal miseAEchelle) const;
        Point operator*(qreal miseAEchelle) const;
        Point operator-(const Point& point) const;
        Point operator+(const Point& point) const;
        const Point& operator+=(const Point& point);
        const Point& operator-=(const Point& point);
        const Point& operator/=(qreal miseAEchelle);
        const Point& operator*=(qreal miseAEchelle);
        qreal operator*(const Point &point) const;
        bool operator<(const Point& point) const;
        bool operator==(const Point& point) const;

        string str();
        QString qstr();

        // angle w.r.t. the origin
        qreal angle(const bool _setbetween0and360 = false) const;
        static qreal setbetween0and360(const qreal value);
        static qreal reinit0and180(const qreal value);

        qreal norm() const;
        void normalise();

        qreal mult(const Point &point) const;
        qreal multcroisee(const Point& point) const;

        // angle between two vectors
        static qreal angle(const Point &point1, const Point &point2);
        static qreal angleSign(const Point& point1, const Point& point2);
        static qreal angleBetweenTwoPoints(const Point &point1, const Point &point2);
    };
}

#endif // POINT_H
