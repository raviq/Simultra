#include "line.h"
#include "utils.h"

Utils::Line::Line() : _YIntercept(0.0), _XIntercept(0.0), _Slope(0.0), _IsVertical(false), _IsHorizontal(false)
{
}

// Set horizontal/vertical parameters and slope.
// @param p1 first point
// @param p2 second point

Utils::Line::Line(const Utils::Point &p1, const Utils::Point &p2)
{
    _IsVertical   =  nearlyEqual(p1.x() - p2.x(), 0.01);
    _IsHorizontal =  nearlyEqual(p1.y() - p2.y(), 0.01);
    _Slope        =  (_IsVertical) ? 0 : (p2.y() - p1.y()) / (p2.x() - p1.x());
    _YIntercept   =  (!_IsHorizontal) ? p1.y() - _Slope * p1.x() : p1.y();
    _XIntercept   =  (!_IsVertical) ? -_YIntercept / _Slope : p1.x();
}

Utils::Line::Line(qreal yIntercept, qreal slope) : _YIntercept(yIntercept), _Slope(slope)
{
}

bool Utils::Line::estHorizontal() const
{
    return _IsHorizontal;
}

bool Utils::Line::estVertical() const
{
    return _IsVertical;
}

qreal Utils::Line::slope() const
{
    return _Slope;
}

qreal Utils::Line::yIntercept() const
{
    return _YIntercept;
}

qreal Utils::Line::xIntercept() const
{
    return _XIntercept;
}

bool Utils::Line::isPrecise(const Utils::Point &p) const
{
    return nearlyEqual(_Slope * p.x() + _YIntercept, p.y());
}

bool Utils::Line::operator==(const Utils::Line& l) const
{
    return nearlyEqual(_Slope, l.slope()) && nearlyEqual(_YIntercept, l.yIntercept());
}
