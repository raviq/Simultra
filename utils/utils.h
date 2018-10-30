#ifndef UTILS_H
#define UTILS_H

#include <QRect>
#include <QString>
#include "point.h"
#include "line.h"
#include "vector.h"
#include "segment.h"

// helpers {{
#define CONCAT(A,B)         A ## B
#define EXPAND_CONCAT(A,B)  CONCAT(A, B)
#define ARGN(N, LIST)       EXPAND_CONCAT(ARG_, N) LIST
#define ARG_0(A0, ...)      A0
#define ARG_1(A0, A1, ...)  A1
#define ARG_2(A0, A1, A2, ...)      A2
#define ARG_3(A0, A1, A2, A3, ...)  A3
#define ARG_4(A0, A1, A2, A3, A4, ...)      A4
#define ARG_5(A0, A1, A2, A3, A4, A5, ...)  A5
#define ARG_6(A0, A1, A2, A3, A4, A5, A6, ...)      A6
#define ARG_7(A0, A1, A2, A3, A4, A5, A6, A7, ...)  A7
#define ARG_8(A0, A1, A2, A3, A4, A5, A6, A7, A8, ...)      A8
#define ARG_9(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, ...)  A9
#define ARG_10(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, ...)    A10
//}}

#define n_s(x) QString::number(x)
#define abs_(x)  (x<0)? -x: x
#define p_s(point) ("("+n_s(point.x())+", "+n_s(point.y())+")")
#define UNUSED(expr) do { (void)(expr); } while (0) // :Do nothing


bool file_exists(QString path);

// random double
double qrandouble(double min, double max);

namespace Utils
{
    enum Layer
    {
       PLAN_LAYER,
       ROAD_LAYER,
       INTERSECTION_LAYER,
       LANE_LAYER,
       VEHICLE_LAYER,
       ANNOTATION_LAYER
    };

    bool nearlyEqual(qreal float1, qreal float2, qreal epsilon=0.1);
    enum IntersectionInfo {NO_INTERSECTION, INTERSECTION, PARALLEL, COINCIDENTAL};
    Point InterLineLine(const Line& l1, const Line& l2, IntersectionInfo& info);
    Point InterVectorVector(const Vector& v1, const Vector& v2, IntersectionInfo& info);
    Point InterVectorSegment(const Vector& v, const Segment& s, IntersectionInfo& info);
    Point InterSegmentSegment(const Segment& s1, const Segment& s2, IntersectionInfo& info);
    void InterVectorRectangle(const Vector &v, const QRect &rect, Point(&p)[2], IntersectionInfo& info);
}

#endif // UTILS_H
