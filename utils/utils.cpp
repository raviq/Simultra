#include "utils.h"
#include <qmath.h>
#include <QFileInfo>

bool file_exists(QString path)
{
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}

// random double
double qrandouble(double min, double max)
{
    return min + ((double) rand() / (RAND_MAX)) * max;
}

bool Utils::nearlyEqual(qreal float1, qreal float2, qreal epsilon)
{
  qreal result;
  if (float1 < float2) result = float2 - float1;
  else result = float1 - float2;
  bool res = result < epsilon;
  return res;
}

Utils::Point Utils::InterLineLine(const Utils::Line &l1, const Utils::Line &l2, Utils::IntersectionInfo &info)
{
    if (!l1.estVertical() && !l2.estVertical())
    {
        qreal x1 = 0, x2 = 1, x3 = 0, x4 = 1;
        qreal y1 = l1.slope() * x1 + l1.yIntercept(),
              y2 = l1.slope() * x2 + l1.yIntercept(),
              y3 = l2.slope() * x3 + l2.yIntercept(),
              y4 = l2.slope() * x4 + l2.yIntercept();

        qreal denom = (y4 - y3)*(x2 - x1)-(x4 - x3)*(y2 - y1),
                numa = (x4 - x3)*(y1 - y3)-(y4 - y3)*(x1 - x3),
                numb = (x2 - x1)*(y1 - y3)-(y2 - y1)*(x1 - x3);
        qreal ua = numa / denom;

        Utils::Point p(x1 + ua * (x2 - x1), y1 + ua * (y2 - y1));
        if (Utils::nearlyEqual(denom, 0) && Utils::nearlyEqual(numa, 0) && Utils::nearlyEqual(numb, 0))
        {
            info = COINCIDENTAL;
        }
        else if (Utils::nearlyEqual(denom, 0))
        {
            info = PARALLEL;
        }
        else
        {
            info = INTERSECTION;
        }
        return p;
    }
    else if (l1.estVertical() && !l2.estVertical())
    {
        Utils::Point p(l1.xIntercept(), l2.slope() * l1.xIntercept() + l2.yIntercept());
        info = INTERSECTION;
        return p;
    }
    else if (!l1.estVertical() && l2.estVertical())
    {
        Utils::Point p(l2.xIntercept(), l1.slope() * l2.xIntercept() + l1.yIntercept());
        info = INTERSECTION;
        return p;
    }
    else
    {
        if (Utils::nearlyEqual(l1.xIntercept(), l2.xIntercept()))
        {
            info = COINCIDENTAL;
        }
        else
        {
            info = NO_INTERSECTION;
        }
        return Utils::Point(0.0, 0.0);
    }
}

Utils::Point Utils::InterVectorVector(const Utils::Vector &r1, const Utils::Vector &r2, Utils::IntersectionInfo &info)
{
    Point p1b = r1.start(),
          p1e = r1.start() + (r1.direction() * 10),
          p2b = r2.start(),
          p2e = r2.start() + (r2.direction() * 10);

    Line l1(p1b, p1e), l2(p2b, p2e);
    Point lli = InterLineLine(l1, l2, info);

    if (info == INTERSECTION && r1.isPrecise(lli) && r2.isPrecise(lli))
        return lli;
    else
    {
        info = NO_INTERSECTION;
        return Point();
    }
}

Utils::Point Utils::InterVectorSegment (const Utils::Vector &r, const Utils::Segment &ls, Utils::IntersectionInfo &info)
{
    Point p1 = r.start(), p2 = r.start() + (r.direction() * 10);
    Line l1(p1, p2), l2(ls.start(), ls.end());
    Point lli = InterLineLine(l1, l2, info);

    if (info == INTERSECTION && r.isPrecise(lli) && ls.isPrecise(lli))
    {
        return lli;
    }
    else
    {
        info = NO_INTERSECTION;
        return Point();
    }
}

Utils::Point Utils::InterSegmentSegment(const Utils::Segment &s1, const Utils::Segment &s2, Utils::IntersectionInfo &info)
{
    Line l1(s1.start(), s1.end()), l2(s2.start(), s2.end());
    Point p = InterLineLine(l1, l2, info);

    if (info == INTERSECTION && s1.isPrecise(p) && s2.isPrecise(p))
    {
        return p;
    }
    else if (info == INTERSECTION)
    {
        info = NO_INTERSECTION;
    }
    return Point();
}

//TODO optimise the following algorithm:

void Utils::InterVectorRectangle(const Vector &v, const QRect &rect, Point(&pts)[2], IntersectionInfo& info) {

    bool firstFound = false, secondFound = false;
    Point p1, p2, p;
    Segment s1(Point(rect.topLeft()), Point(rect.bottomLeft()));
    IntersectionInfo inf;
    p = InterVectorSegment(v, s1, inf);

    if (inf != NO_INTERSECTION)
    {
        firstFound = true;
        info = INTERSECTION;
        p1 = p;
    }

    Segment s2(Point(rect.bottomLeft()), Point(rect.bottomRight()));
    p = InterVectorSegment(v, s2, inf);
    if (inf != NO_INTERSECTION)
    {
        if (!firstFound)
        {
            firstFound = true;
            p1 = p;
            info = INTERSECTION;
        }
        else
        {
            secondFound = true;
            p2 = p;
        }
    }

    if (firstFound && secondFound)
    {
        qreal d1 = (v.start() - p1).norm();
        qreal d2 = (v.start() - p2).norm();
        if (d2 > d1) {
            pts[0] = p1;
            pts[1] = p2;
            return;
        }
        else
        {
            pts[1] = p1;
            pts[0] = p2;
            return;
        }
    }

    Segment s3(Point(rect.bottomRight()), Point(rect.topRight()));
    p = InterVectorSegment(v, s3, inf);
    if (inf != NO_INTERSECTION)
    {
        if (!firstFound)
        {
            firstFound = true;
            p1 = p;
            info = INTERSECTION;
        }
        else
        {
            secondFound = true;
            p2 = p;
        }
    }

    if (firstFound && secondFound)
    {
        qreal d1 = (v.start() - p1).norm();
        qreal d2 = (v.start() - p2).norm();
        if (d2 > d1)
        {
            pts[0] = p1;
            pts[1] = p2;
            return;
        }
        else
        {
            pts[1] = p1;
            pts[0] = p2;
            return;
        }
    }

    Segment s4(Point(rect.bottomRight()), Point(rect.topRight()));
    p = InterVectorSegment(v, s4, inf);
    if (inf != NO_INTERSECTION)
    {
        if (!firstFound)
        {
            firstFound = true;
            p1 = p;
            info = INTERSECTION;
        }
        else
        {
            secondFound = true;
            p2 = p;
        }
    }

    if (firstFound && secondFound)
    {
        qreal d1 = (v.start() - p1).norm();
        qreal d2 = (v.start() - p2).norm();
        if (d2 > d1)
        {
            pts[0] = p1;
            pts[1] = p2;
            return;
        }
        else
        {
            pts[1] = p1;
            pts[0] = p2;
            return;
        }
    }
    else
    {
        pts[0] = p1;
        pts[1] = p2;
    }

}
