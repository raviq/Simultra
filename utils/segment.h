#ifndef SEGMENT_H
#define SEGMENT_H
#include <QLine>
#include "point.h"

namespace Utils
{
    class Segment
    {
        public:
            Segment();
            Segment(const Point& start, const Point& end);
            QLineF toQLineF() const;
            const Point& start() const;
            Point& start();
            const Point& end() const;
            Point& end();
            Segment towardsPerpendicularFromStart(qreal shift, qreal norm) const;
            Segment towardsPerpendicularFromEnd(qreal shift, qreal norm) const;
            Segment towardsShift(qreal decalage) const;
            virtual Point nearestPoint(const Point& p) const;
            virtual bool isPrecise(const Point& p) const;
            virtual bool operator==(const Segment& s) const;

        protected:
            Point _Start;
            Point _End;
    };
}

#endif // SEGMENT_H
