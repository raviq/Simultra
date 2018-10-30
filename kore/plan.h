#ifndef PLAN_H
#define PLAN_H

#include <QString>
#include <QSize>
#include <QList>
#include <QMap>

#include "point.h"
#include "segment.h"

class Plan
{
public:
    class Road;
    class Intersection;
    typedef QList<Road*> RoadsList;
    typedef QList<Intersection*> IntersectionsList;
    typedef QMap<QString, Intersection*> DictionnaireIntersections;

    Plan();
    Plan(const QString& fileName);
    ~Plan();

    const Intersection* getIntersection(const Utils::Point& point) const;
    Intersection* getIntersection(const Utils::Point& point);
    const Intersection* getIntersection(const Intersection* intersectionPrecedente, const Road* roadActuelle) const;
    const Intersection* getIntersection(const QString& name) const;
    IntersectionsList getIntersections() const;
    void creerTrousIntersection();

    const Road* getRoad(const Intersection* intersection1, const Intersection* intersection2) const;
    const Road* getRoad(const Utils::Point& point) const;
    const Road* getRoad(const QString &name) const;
    RoadsList getRoads() const;
    RoadsList getRoads(const Intersection* intersection) const;

protected:
    RoadsList _Roads;
    DictionnaireIntersections _Intersections;

};


//

class Plan::Road
{
public:

    class Lane
    {
        public:
            enum Direction { NONE, FIRST_TO_SECOND, SECOND_TO_FIRST };
            Direction _Direction;
            Utils::Segment _Segment;
    };

    typedef QList<Lane> LaneList;

    Road();
    Road(const QString& roadName, Intersection* first, Intersection* second);

    const QString& name() const;

    qreal height() const;

    unsigned int width() const;

    const Utils::Point& first() const;

    Utils::Point& first();

    const Utils::Point& second() const;

    Utils::Point& second();

    const Intersection *getFirstIntersection() const;

    Intersection *getFirstIntersection();
    const Intersection *getSecondIntersection() const;
    Intersection *getSecondIntersection();
    Lane::Direction getDirection(const Intersection *nextIntersection) const;

    // Accept result iff OKresult = true
    const Lane& lane(Lane::Direction d, int positionLane, bool& OKresult) const;
    unsigned int nbLanes(Lane::Direction d) const;
    unsigned int nbLanes() const;
    void addLane(int positionLane, Lane::Direction dir);
    void addLane(const Lane& v);
    void computeSegmentsLane();

protected:
    QString _Name;
    QList<Lane> _LanesFirstToSecond, _LanesSecondToFirst;
    Utils::Point _First, _Second;
    Intersection *_FirstIntersection, *_SecondIntersection;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Plan::Intersection
{
   public:
    Intersection();
    Intersection(const QString& name, const Utils::Point& location);
    const QString& name() const;
    const Utils::Point & location() const;
    void addNeighbour(Intersection* neighbour);

protected:
    QString _Name;
    QList<Intersection*> _Neighbours;
    Utils::Point _Location;
};

#endif // PLAN_H
