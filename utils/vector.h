#ifndef VECTEUR_H
#define VECTEUR_H
#include "point.h"

namespace Utils
{

    class Vector
    {
        public:
            Vector();
            Vector(const Point& start, const Point& dir);
            const Point& start() const;
            Point& start();
            const Point& direction() const;
            Point& direction();
            virtual bool isPrecise(const Point &p) const;
            virtual bool operator==(const Vector& v) const;

        protected:
            Point _Start;
            Point _Direction;
    };
}

#endif // VECTEUR_H
