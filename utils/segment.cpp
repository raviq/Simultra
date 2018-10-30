#include "segment.h"
#include "line.h"
#include "utils.h"

Utils::Segment::Segment()
{
}

Utils::Segment::Segment(const Utils::Point& start, const Utils::Point& end) : _Start(start), _End(end)
{
}

QLineF Utils::Segment::toQLineF() const
{
    return QLineF(_Start, _End);
}

const Utils::Point& Utils::Segment::start() const
{
    return _Start;
}

Utils::Point& Utils::Segment::start()
{
    return _Start;
}

const Utils::Point& Utils::Segment::end() const
{
    return _End;
}

Utils::Point& Utils::Segment::end()
{
    return _End;
}

bool Utils::Segment::isPrecise(const Utils::Point& p) const
{
    Utils::Line l(_Start, _End);
    qreal start_x = _Start.x(), end_x = _End.x(), start_y = _Start.y(), end_y = _End.y();
    if (!l.estVertical())
    {
        if (nearlyEqual(p.y(), p.x() * l.slope() + l.yIntercept()))
            if ((start_x <= p.x() && p.x() <= end_x) || (end_x <= p.x() && p.x() <= start_x))
                if ((start_y <= p.y() && p.y() <= end_y) || (end_y <= p.y() && p.y() <= start_y))
                    return true;
        return false;
    }
    else
    {
        if ((start_y <= p.y() && p.y() <= end_y) || (end_y <= p.y() && p.y() <= start_y))
            if (nearlyEqual(_Start.x(), p.x()))
                return true;
        return false;
    }
}

Utils::Point Utils::Segment::nearestPoint(const Utils::Point &p) const
{
    Utils::Point diff = p - _Start;
    Utils::Point direction = _End - _Start;
    qreal dot1 = diff.mult(direction);
    if (dot1 <= 0.0f)
        return _Start;

    qreal dot2 = direction.mult(direction);
    if (dot2 <= dot1)
        return _End;

    qreal t = dot1 / dot2;
    return _Start + (direction * t);
}

bool Utils::Segment::operator==(const Utils::Segment& s) const
{
    return (_Start == s.start() && _End == s.start()) || (_Start == s.end() && _End == s.end());
}

Utils::Segment Utils::Segment::towardsPerpendicularFromStart(qreal shift, qreal norm) const
{
    Point dir(_End - _Start);
    dir.normalise();
    Point perpDir(-dir.y(), dir.x());
    perpDir.normalise();
    Point shiftPt(_Start + (dir * shift));
    return Segment(shiftPt - (perpDir * norm / 2.0), shiftPt + (perpDir * norm / 2.0));
}

Utils::Segment Utils::Segment::towardsPerpendicularFromEnd(qreal shift, qreal norm) const
{
    Point dir(_End - _Start);
    dir.normalise();
    Point perpDir(-dir.y(), dir.x());
    perpDir.normalise();
    Point shiftPt(_End + (dir * shift));
    return Segment(shiftPt - (perpDir * norm / 2.0), shiftPt + (perpDir * norm / 2.0));
}

Utils::Segment Utils::Segment::towardsShift(qreal shift) const
{
    return Segment(towardsPerpendicularFromStart(0, shift * 2).end(), towardsPerpendicularFromEnd(0, shift * 2).end());
}

