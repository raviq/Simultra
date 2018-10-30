#ifndef LINE_H
#define LINE_H
#include "point.h"

namespace Utils
{
    class Line
    {
    public:
        Line();
        Line(const Utils::Point& p1, const Utils::Point& p2);
        Line(qreal yIntercept, qreal slope);
        qreal yIntercept() const;
        qreal xIntercept() const;
        qreal slope() const;
        bool estVertical() const;
        bool estHorizontal() const;
        virtual bool isPrecise(const Point& p) const;
        virtual bool operator==(const Line& l) const;

    protected:
        qreal _YIntercept, _XIntercept;
        qreal _Slope;
        bool _IsVertical, _IsHorizontal;
    };
}

#endif // LIGNE_H
