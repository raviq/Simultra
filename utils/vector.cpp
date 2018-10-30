#include "vector.h"
#include "line.h"
#include "utils.h"

Utils::Vector::Vector() : _Start(0.0, 0.0), _Direction(0.0, 0.0)
{
}

Utils::Vector::Vector(const Utils::Point &start, const Utils::Point &dir) : _Start(start), _Direction(dir)
{
}

const Utils::Point& Utils::Vector::start() const
{
    return _Start;
}

Utils::Point& Utils::Vector::start()
{
    return _Start;
}

const Utils::Point& Utils::Vector::direction() const
{
    return _Direction;
}

Utils::Point& Utils::Vector::direction()
{
    return _Direction;
}

bool Utils::Vector::operator==(const Utils::Vector& v) const
{
    return _Direction == v.direction() && _Start == v.start();
}

bool Utils::Vector::isPrecise(const Utils::Point& p) const
{
    Utils::Line line(_Start, _Start + (_Direction * 10));
    if (line.estVertical())
    {
        return (nearlyEqual(p.x(), line.xIntercept()) &&
                 ((_Direction.y() > 0 && p.y() >= _Start.y()) ||
                    (_Direction.y() < 0 && p.y() <= _Start.y())));
    }
    else
    {
        return (nearlyEqual(p.y(), line.slope() * p.x() + line.yIntercept()) &&
                ((_Direction.x() > 0 && p.x() >= _Start.x()) ||
                (_Direction.x() < 0 && p.x() <= _Start.x())));
    }
}
