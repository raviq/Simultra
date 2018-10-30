#include "imagesignalisation.h"

// Constructor
// @param name name of the itnesection
// @param intersection Object intersection
// @param state state of the signalisation

ImageSignalisation::ImageSignalisation(const QString & name,
                                       const Plan::Intersection * intersection,
                                       const Signalisation::State & state)
  : _Name(name), _Intersection(intersection), _State(state)
{

}

QString ImageSignalisation::name() const
{
  return _Name;
}

Signalisation::State ImageSignalisation::state() const
{
  return _State;
}

const Plan::Intersection* ImageSignalisation::intersection() const
{
  return _Intersection;
}
