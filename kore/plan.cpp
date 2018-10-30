//
// Abstract representation of plan, roads, intersections, etc.
//

#include <QtGui/QPen>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomElement>
#include <QFile>
#include <qmath.h>
#include <fstream>
#include <string>

#include "plan.h"
#include "line.h"
#include "segment.h"
#include "utils.h"
#include "config.h"

// Road

Plan::Road::Road() : _FirstIntersection(NULL), _SecondIntersection(NULL)
{
}

// @param roadName   Name of the road.
// @param first      Intersection start.
// @param second     Intersection end.

Plan::Road::Road(const QString& roadName, Plan::Intersection* first, Plan::Intersection* second)
: _Name(roadName), _First(first->location()), _Second(second->location()),
  _FirstIntersection(first), _SecondIntersection(second)
{
}

const QString& Plan::Road::name() const
{
    return _Name;
}

qreal Plan::Road::height() const
{
    return (_First - _Second).norm();
}

const Utils::Point& Plan::Road::first() const
{
    return _First;
}

Utils::Point& Plan::Road::first()
{
    return _First;
}

const Utils::Point& Plan::Road::second() const
{
    return _Second;
}

Utils::Point& Plan::Road::second()
{
    return _Second;
}

const Plan::Intersection *Plan::Road::getFirstIntersection() const
{
    return _FirstIntersection;
}

Plan::Intersection *Plan::Road::getFirstIntersection()
{
    return _FirstIntersection;
}

const Plan::Intersection *Plan::Road::getSecondIntersection() const
{
    return _SecondIntersection;
}

Plan::Intersection *Plan::Road::getSecondIntersection()
{
    return _SecondIntersection;
}

unsigned int Plan::Road::width() const
{
    return (_LanesFirstToSecond.count() + _LanesSecondToFirst.count()) * LANE_WIDTH;
}

unsigned int Plan::Road::nbLanes(Plan::Road::Lane::Direction d) const
{
    return d == Lane::FIRST_TO_SECOND ? _LanesFirstToSecond.count() : _LanesSecondToFirst.count();
}

unsigned int Plan::Road::nbLanes() const
{
    return _LanesFirstToSecond.count() + _LanesSecondToFirst.count();
}


// @param d Direction of the prefered lane
// @param positionLane Position
// @param OKresult true if the lane exists, else false
// @return track in direction d, at the specified position or the first lane in direction d
// if the specifided direction does not correspond to a lane;

// TODO Lane
const Plan::Road::Lane& Plan::Road::lane(Plan::Road::Lane::Direction d, int positionLane, bool& OKresult) const
{
    const QList<Lane> &lanes = d == Lane::FIRST_TO_SECOND ? _LanesFirstToSecond : _LanesSecondToFirst;
    if (positionLane < 0 || positionLane > lanes.count())
    {
        OKresult = false;
        return lanes.first();
    }
    else
    {
        OKresult = true;
        return lanes.at(positionLane);
    }
}

void Plan::Road::addLane(int positionLane, Plan::Road::Lane::Direction dir)
{
    QList<Lane>& lanes = (dir == Lane::FIRST_TO_SECOND ? _LanesFirstToSecond : _LanesSecondToFirst);
    if (positionLane >= 0 && lanes.count() > positionLane)
    {
        Lane lane = {dir, Utils::Segment()};
        lanes.replace(positionLane, lane);
    }
    else if (positionLane >= 0)
    {
        Lane actual = {dir, Utils::Segment()},
             temp = {dir, Utils::Segment()};
        while (lanes.count() + 1 < positionLane)
            lanes.push_back(temp);
        lanes.push_back(actual);
    }
    computeSegmentsLane();
}

void Plan::Road::addLane(const Plan::Road::Lane& v)
{
    if (v._Direction == Lane::FIRST_TO_SECOND)
            _LanesFirstToSecond.push_back(v);
    else if (v._Direction == Lane::SECOND_TO_FIRST)
            _LanesSecondToFirst.push_back(v);
    computeSegmentsLane();
}

void Plan::Road::computeSegmentsLane() // TODO
{
    Utils::Segment directLine(_First, _Second);

    Utils::Segment sideLine = directLine.towardsShift(width() / 2);
    int nbLanes = 0;

    for (int i = _LanesSecondToFirst.size() - 1; i >= 0; i--)
    {
        _LanesSecondToFirst[i]._Segment = sideLine.towardsShift(nbLanes * -LANE_WIDTH - (LANE_WIDTH / 2));
        nbLanes++;
    }

    for (int i = 0; i < _LanesFirstToSecond.size(); i++)
    {
        _LanesFirstToSecond[i]._Segment = sideLine.towardsShift(nbLanes * -LANE_WIDTH - (LANE_WIDTH / 2));
        nbLanes++;
    }
}

Plan::Road::Lane::Direction Plan::Road::getDirection(const Intersection *nextIntersection) const
{
    if (nextIntersection != NULL)
    {
        if (nextIntersection == getFirstIntersection())
            return Lane::FIRST_TO_SECOND;
        else if (nextIntersection == getSecondIntersection())
            return Lane::SECOND_TO_FIRST;
    }
    return Lane::NONE;
}

// Intersection

Plan::Intersection::Intersection() : _Location(0.0, 0.0)
{
}

Plan::Intersection::Intersection(const QString& name, const Utils::Point& location) : _Name(name), _Location(location)
{
}

const Utils::Point& Plan::Intersection::location() const
{
    return _Location;
}

const QString& Plan::Intersection::name() const
{
    return _Name;
}

void Plan::Intersection::addNeighbour(Plan::Intersection* neighbour)
{
    _Neighbours.push_back(neighbour);
}

// Plan

Plan::Plan()
{
}

Plan::Plan(const QString & fileName)
{
    QDomDocument doc("Topology");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomNodeList intersections = doc.elementsByTagName("Intersection");
    for (int n = 0; n < intersections.size(); ++n)
    {
        QDomElement intersection = intersections.at(n).toElement();
        qreal x = intersection.attribute("x").toFloat(),
              y = intersection.attribute("y").toFloat();
        QString name = intersection.attribute("name");
        Intersection *i = new Intersection(name, Utils::Point(x, y));
        if (_Intersections.contains(name))
        {
            delete _Intersections[name];
        }
        _Intersections[name] = i;
    }

    QDomNodeList roads = doc.elementsByTagName("Road");
    for (int n = 0; n < roads.count(); ++n)
    {
        QDomElement road = roads.at(n).toElement();
        Road *r = new Road(road.attribute("name"),
                             _Intersections[road.attribute("start")],
                             _Intersections[road.attribute("end")]);
        for (uint j = 0; j < road.attribute("pvd").toUInt(); ++j)
        {
            Road::Lane v;
            v._Direction = Road::Lane::FIRST_TO_SECOND;
            r->addLane(v);
        }
        for (uint j = 0; j < road.attribute("dvp").toUInt(); ++j)
        {
            Road::Lane v;
            v._Direction = Road::Lane::SECOND_TO_FIRST;
            r->addLane(v);
        }
        _Roads.push_back(r);
    }
    creerTrousIntersection();
}

Plan::~Plan()
{
    foreach(Road *r, _Roads)
            delete r;
    foreach(Intersection *i, _Intersections)
            delete i;
    _Roads.clear();
    _Intersections.clear();
}

const Plan::Intersection* Plan::getIntersection(const Utils::Point& point) const
{
    Intersection *inter = NULL;
    foreach(Intersection *i, _Intersections)
    {
        // TODO
        if (inter == NULL)
        {
            inter = i;
        }
        else if ((i->location() - point).norm() < (inter->location() - point).norm())
        {
            inter = i;
        }
    }
    return inter;
}

const Plan::Intersection* Plan::getIntersection(const Plan::Intersection *previousIntersection, const Plan::Road *roadActuelle) const
{
    if (roadActuelle->getFirstIntersection() == previousIntersection)
    {
        return roadActuelle->getSecondIntersection();
    }
    else
    {
        return roadActuelle->getFirstIntersection();
    }
}

Plan::Intersection* Plan::getIntersection(const Utils::Point& point)
{
    Intersection *inter = NULL;

    foreach(Intersection *i, _Intersections)
    {
        if (inter == NULL)
        {
            inter = i;
        }
        else if ((i->location() - point).norm() < (inter->location() - point).norm())
        {
            inter = i;
        }
    }
    return inter;
}

const Plan::Intersection* Plan::getIntersection(const QString& name) const
{
    if (_Intersections.contains(name))
        return _Intersections[name];
    else
        return NULL;
}

Plan::IntersectionsList Plan::getIntersections() const
{
    return _Intersections.values();
}

// @param point point of the plan
// @return Road nearest road to the point

const Plan::Road* Plan::getRoad(const Utils::Point &point) const
{
    Road *road = NULL;

    foreach(Road* r, _Roads)
    {
        if (road == NULL)
        {
            road = r;
        }
        else
        {
            Utils::Point p1 = (r->second() + r->first()) / 2.0,
                         p2 = (road->second() + road->first()) / 2.0;
            if ((p1 - point).norm() < (p2 - point).norm())
                road = r;
        }
    }
    return road;
}

const Plan::Road* Plan::getRoad(const QString &roadName) const
{
    foreach(Road* r, _Roads)
    {
        if (r && r->name() == roadName)
            return r;
    }
    return NULL;
}

const Plan::Road* Plan::getRoad(const Plan::Intersection* intersection1, const Plan::Intersection* intersection2) const
{
    if (!intersection1 || !intersection2)
        return NULL;

    foreach(Road* r, _Roads)
    {
        if (   ((r->getFirstIntersection() && r->getFirstIntersection()->name() == intersection1->name())
                && (r->getSecondIntersection() && r->getSecondIntersection()->name() == intersection2->name()))
                ||
                ((r->getFirstIntersection() && r->getFirstIntersection()->name() == intersection2->name())
                && (r->getSecondIntersection() && r->getSecondIntersection()->name() == intersection1->name()))
                )
            return r;
    }
    return NULL;
}

// @param intersection Objet intersection
// @return list of the roads of the plan connected to the intersection

Plan::RoadsList Plan::getRoads(const Plan::Intersection *intersection) const
{
    RoadsList roads;
    if (intersection)
    {
        foreach(Road* r, _Roads)
        {
            if (r->getFirstIntersection() && r->getFirstIntersection()->name() == intersection->name())
            {
                roads.append(r);
            }
            if (r->getSecondIntersection() && r->getSecondIntersection()->name() == intersection->name())
            {
                roads.append(r);
            }
        }
    }
    return roads;
}

Plan::RoadsList Plan::getRoads() const
{
    return _Roads;
}

void Plan::creerTrousIntersection()
{

    foreach(Intersection *i, _Intersections)
    {
        RoadsList roads = getRoads(i);
        RoadsList::iterator counterRoadExt;
        for (counterRoadExt = roads.begin(); counterRoadExt != roads.end(); ++counterRoadExt)
        {
            RoadsList::iterator counterRoadInt;
            for (counterRoadInt = roads.begin(); counterRoadInt != roads.end(); ++counterRoadInt)
            {
                Road *roadExterior = *counterRoadExt, *roadInterior = *counterRoadInt;
                qreal widthExt = roadExterior->width(), widthInt = roadInterior->width();
                Utils::Segment segmentExt(roadExterior->first(), roadExterior->second()),
                               segmentInt(roadInterior->first(), roadInterior->second());
                Utils::Segment segmentExtPos(segmentExt.towardsShift((widthExt / 2))), segmentExtNeg(segmentExt.towardsShift(-(widthExt / 2))),
                               segmentIntPos(segmentInt.towardsShift((widthInt / 2))), segmentIntNeg(segmentInt.towardsShift(-(widthInt / 2)));
                Utils::IntersectionInfo infsepsip, infsepsin, infsensip, infsensin;
                Utils::Point sepsip(Utils::InterSegmentSegment(segmentExtPos, segmentIntPos, infsepsip)),
                             sepsin(Utils::InterSegmentSegment(segmentExtPos, segmentIntNeg, infsepsin)),
                             sensip(Utils::InterSegmentSegment(segmentExtNeg, segmentIntPos, infsensip)),
                             sensin(Utils::InterSegmentSegment(segmentExtNeg, segmentIntNeg, infsensin));
                if (infsepsip == Utils::INTERSECTION)
                {
                  bool eb=roadExterior->getFirstIntersection()->name() == i->name(),
                       ib=roadInterior->getFirstIntersection()->name() == i->name();
                  Utils::Point eoff(eb?segmentExtPos.start()-sepsip:segmentExtPos.end()-sepsip),
                               ioff(ib?segmentIntPos.start()-sepsip:segmentIntPos.end()-sepsip);
                  (eb?roadExterior->first():roadExterior->second()) -= eoff;
                  (ib?roadInterior->first():roadInterior->second()) -= ioff;
                }
                if (infsepsin == Utils::INTERSECTION)
                {
                  bool eb=roadExterior->getFirstIntersection()->name() == i->name(),
                       ib=roadInterior->getFirstIntersection()->name() == i->name();
                  Utils::Point eoff(eb?segmentExtPos.start()-sepsin:segmentExtPos.end()-sepsin),
                               ioff(ib?segmentIntNeg.start()-sepsin:segmentIntNeg.end()-sepsin);
                  (eb?roadExterior->first():roadExterior->second()) -= eoff;
                  (ib?roadInterior->first():roadInterior->second()) -= ioff;
                }
                if (infsensip == Utils::INTERSECTION)
                {
                  bool eb=roadExterior->getFirstIntersection()->name() == i->name(),
                       ib=roadInterior->getFirstIntersection()->name() == i->name();
                  Utils::Point eoff(eb?segmentExtNeg.start()-sensip:segmentExtNeg.end()-sensip),
                               ioff(ib?segmentIntPos.start()-sensip:segmentIntPos.end()-sensip);
                  (eb?roadExterior->first():roadExterior->second()) -= eoff;
                  (ib?roadInterior->first():roadInterior->second()) -= ioff;
                }
                if (infsensin == Utils::INTERSECTION)
                {
                  bool eb=roadExterior->getFirstIntersection()->name() == i->name(),
                       ib=roadInterior->getFirstIntersection()->name() == i->name();
                  Utils::Point eoff(eb?segmentExtNeg.start()-sensin:segmentExtNeg.end()-sensin),
                               ioff(ib?segmentIntNeg.start()-sensin:segmentIntNeg.end()-sensin);
                  (eb?roadExterior->first():roadExterior->second()) -= eoff;
                  (ib?roadInterior->first():roadInterior->second()) -= ioff;
                }
                roadInterior->computeSegmentsLane();
                roadExterior->computeSegmentsLane();
            }
        }
    }
}
// End.
