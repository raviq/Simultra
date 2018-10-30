#ifndef IMAGESIGNALISATION_H
#define IMAGESIGNALISATION_H
#include "point.h"
#include "plan.h"
#include "signalisation.h"

class ImageSignalisation
{
    public:
      ImageSignalisation(const QString & name,
                          const Plan::Intersection * intersection,
                          const Signalisation::State & state);

      QString name() const;
      const Plan::Intersection* intersection() const;
      Signalisation::State state() const;

    protected:
      QString _Name;
      const Plan::Intersection *_Intersection;
      Signalisation::State _State;
};

#endif
