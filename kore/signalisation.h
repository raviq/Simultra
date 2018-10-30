#ifndef SIGNALISATION_H
#define SIGNALISATION_H

#include <QList>
#include "plan.h"

class Signalisation
{
  public:

    enum LIGHT_COLOUR
    {
      RED,
      ORANGE,
      GREEN
    };

    class Connection
    {
    public:
      Connection(const Plan::Road *_Start, const unsigned int _LaneStart, const Plan::Road *_End, const unsigned int _LaneEnd);
      const Plan::Road *_Start;
      unsigned int _LaneStart;
      const Plan::Road *_End;
      unsigned int _LaneEnd;
    };

    typedef QList<Connection*> ConnectionsList;

    class State
    {
        public:
          State(const QString &name);
          ~State();
          QString name() const;
          ConnectionsList getConnections();
          void addConnection(Connection *connection);
          void setGreenDuration(const qreal duration);
          qreal greenDuration() const;
          void setOrangeDuration(const qreal duration);
          qreal orangeDuration() const;
          void setRedDuration(const qreal duration);
          qreal redDuration() const;
          LIGHT_COLOUR lightColor(const Plan::Road *start, const unsigned int startLane,
                                 const Plan::Road *end, const unsigned int endLane) const;
          LIGHT_COLOUR lightColor() const;
          void setLightColour(const LIGHT_COLOUR name);
          bool operator==(const State & state) const;

        protected:
          QString _Name;
          ConnectionsList _Connections;
          qreal _GreenDuration;
          qreal _OrangeDuration;
          qreal _RedDuration;
          LIGHT_COLOUR _CurrentColour;
    };

    Signalisation();
    ~Signalisation();
    void setIntersection(const Plan::Intersection *intersection);
    const Plan::Intersection* intersection() const;
    void addState(State *state);
    State* currentState() const;
    void avancer();

  protected:
    QList<State*> _States;
    const Plan::Intersection *_Intersection;
    int _IdStateActuel;
    qreal _TpsDepuisDernierChangtState;
};

#endif // SIGNALISATION_H
