#include "point.h"
#include <cmath>
#include "utils.h"

using namespace std;

Utils::Point::Point()
{
}

Utils::Point::Point(qreal x, qreal y) : QPointF(x, y)
{
}

Utils::Point::Point(const QPoint &p) : QPointF(p)
{
}

Utils::Point::Point(const QPointF &p): QPointF(p)
{
}

string Utils::Point::str()
{
    return QString::number(x()).toStdString() + ", " + QString::number(y()).toStdString();
}

QString Utils::Point::qstr()
{
    return QString::number(x())+ ", " + QString::number(y());
}

Utils::Point Utils::Point::operator/(qreal miseAEchelle) const
{
    return Point(x() / miseAEchelle, y() / miseAEchelle);
}

Utils::Point Utils::Point::operator*(qreal miseAEchelle) const
{
    return Point(x() * miseAEchelle, y() * miseAEchelle);
}

Utils::Point Utils::Point::operator-(const Utils::Point& point) const
{
    return Point(x() - point.x(), y() - point.y());
}

Utils::Point Utils::Point::operator+(const Utils::Point& point) const
{
    return Point(x() + point.x(), y() + point.y());
}

const Utils::Point& Utils::Point::operator+=(const Utils::Point& point)
{
    setX(x() + point.x());
    setY(y() + point.y());
    return *this;
}

const Utils::Point& Utils::Point::operator-=(const Utils::Point& point)
{
    setX(x() - point.x());
    setY(y() - point.y());
    return *this;
}

const Utils::Point& Utils::Point::operator/=(qreal miseAEchelle)
{
    if (!nearlyEqual(miseAEchelle, 0, 0.001)) {
        setX(x() / miseAEchelle);
        setY(y() / miseAEchelle);
    }
    return *this;
}

const Utils::Point& Utils::Point::operator*=(qreal miseAEchelle)
{
    setX(x() * miseAEchelle);
    setY(y() * miseAEchelle);
    return *this;
}

qreal Utils::Point::operator*(const Utils::Point &point) const
{
    return mult(point);
}

bool Utils::Point::operator<(const Utils::Point & p) const
{
    return norm() < p.norm();
}

bool Utils::Point::operator ==(const Utils::Point& p) const
{
    return Utils::nearlyEqual(x(), p.x()) && Utils::nearlyEqual(y(), p.y());
}

qreal Utils::Point::norm() const
{
    return sqrt(x() * x() + y() * y());
}

qreal Utils::Point::mult(const Utils::Point &point) const
{
    return x() * point.x() + y() * point.y();
}

qreal Utils::Point::multcroisee(const Point &point) const
{
    return x() * point.y() - y() * point.x();
}

// TODO angle (deg) wrt origin in
// @param setbetween0and360 if true angle in [0, 360] else in [0, 180]

qreal Utils::Point::angle(const bool _setbetween0and360) const
{
    qreal ret = (qreal) (atan2(y(), x())*(180.0 / M_PI));
    if (_setbetween0and360)
        Point::setbetween0and360(ret);
    else
        Point::reinit0and180(ret);
    return ret;
}

// Set angle between 0 et 360
// @param angle in deg

qreal Utils::Point::setbetween0and360(const qreal value)
{
    qreal ret = value;
    while (ret >= 360.0f) ret -= 360.0f;
    while (ret < 0.0f) ret += 360.0f;
    return ret;
}


// set angle between 0 et 360
// @param angle in deg

qreal Utils::Point::reinit0and180(const qreal value)
{
    qreal ret = value;
    while (ret >= 180.0f) ret -= 360.0f;
    while (ret < -180.0f) ret += 360.0f;
    return ret;
}

qreal Utils::Point::angle(const Point &point1, const Point &point2)
{
    return acos((point1 * point2) / (point1.norm() * point2.norm()));
}

qreal Utils::Point::angleSign(const Point &point1, const Point &point2)
{
    return atan2(point1.multcroisee(point2), point1.mult(point2));
}

qreal Utils::Point::angleBetweenTwoPoints(const Point &point1, const Point &point2)
{
    Utils::Point p = point2 - point1;
    return p.angle();
}

// set point norm to 1.
void Utils::Point::normalise()
{
    qreal mag = norm();
    if (mag == 0)
        return;
    setX(x() / mag);
    setY(y() / mag);
}
