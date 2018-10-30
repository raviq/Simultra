//
// Traffic lights signalisation
//

#include "signalisation.h"
#include "simulation.h"

Signalisation::Connection::Connection(const Plan::Road *start, const unsigned int startLane, const Plan::Road *end, const unsigned int endLane)
{
  this->_Start = start;
  this->_End = end;
  this->_LaneStart = startLane;
  this->_LaneEnd= endLane;
}

Signalisation::State::State(const QString &name)
{
  _Name = name;
  _Connections.clear();
  _GreenDuration = 0.0;
  _OrangeDuration = 0.0;
  _RedDuration = 0.0;
}

Signalisation::State::~State()
{
    _Connections.clear();
}

QString Signalisation::State::name() const
{
  return _Name;
}

Signalisation::ConnectionsList Signalisation::State::getConnections()
{
  return _Connections;
}

void Signalisation::State::setGreenDuration(const qreal duration)
{
  _GreenDuration = duration;
}

qreal Signalisation::State::greenDuration() const
{
  return _GreenDuration;
}

void Signalisation::State::setOrangeDuration(const qreal duration)
{
  _OrangeDuration = duration;
}

qreal Signalisation::State::orangeDuration() const
{
  return _OrangeDuration;
}

void Signalisation::State::setRedDuration(const qreal duration)
{
  _RedDuration = duration;
}

qreal Signalisation::State::redDuration() const
{
  return _RedDuration;
}

void Signalisation::State::setLightColour(Signalisation::LIGHT_COLOUR name)
{
  _CurrentColour = name;
}

Signalisation::LIGHT_COLOUR Signalisation::State::lightColor() const
{
  return _CurrentColour;
}

Signalisation::LIGHT_COLOUR Signalisation::State::lightColor(
    const Plan::Road *start, const unsigned int startLane,
    const Plan::Road *end, const unsigned int endLane) const
{
  for (int i = 0 ; i < _Connections.size() ; i++)
  {
    const Signalisation::Connection *c = _Connections.at(i);
    if (c && c->_Start == start && c->_LaneStart == startLane && c->_End == end && c->_LaneEnd == endLane)
    {
      return _CurrentColour;
    }
  }
  return RED;
}

void Signalisation::State::addConnection(Connection *connection)
{
  _Connections.push_back(connection);
}

bool Signalisation::State::operator==(const State &state) const
{
  if (name() == state.name())
    return true;
  else
    return false;
}

Signalisation::Signalisation()
{
  _Intersection = NULL;
  _States.clear();
  _IdStateActuel = 0;
  _TpsDepuisDernierChangtState = 0.0;
}

Signalisation::~Signalisation()
{
  _States.clear();
}

void Signalisation::setIntersection(const Plan::Intersection *intersection)
{
  _Intersection = intersection;
}

const Plan::Intersection* Signalisation::intersection() const
{
  return _Intersection;
}

void Signalisation::addState(Signalisation::State *state)
{
  if (state)
  {
    foreach (Signalisation::State *e, _States)
    {
      if (e && e->name() == state->name())
      {
        return;
      }
    }
    _States.push_back(state);
  }
}

Signalisation::State* Signalisation::currentState() const
{
  return _States.at(_IdStateActuel);
}

void Signalisation::avancer()
{
    _TpsDepuisDernierChangtState += Simulation::_simulationStep;
    State *actuel = currentState();

    if (_TpsDepuisDernierChangtState > actuel->greenDuration())
    {
      if (_TpsDepuisDernierChangtState > (actuel->greenDuration() + actuel->orangeDuration()))
      {
        actuel->setLightColour(RED);
        if (_TpsDepuisDernierChangtState > (actuel->greenDuration() + actuel->orangeDuration()+ actuel->redDuration()))
        {
          _TpsDepuisDernierChangtState = 0.0;
          _IdStateActuel++;
          if (_IdStateActuel >= _States.size())
            _IdStateActuel = 0;
        }
      }
      else
      {
        actuel->setLightColour(ORANGE);
      }
    }
    else
    {
      actuel->setLightColour(GREEN);
    }
}
