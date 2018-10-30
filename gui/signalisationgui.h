#ifndef SIGNALISATIONGUI_H
#define SIGNALISATIONGUI_H

#include <QGraphicsPixmapItem>
#include "plan.h"
#include "imagesignalisation.h"

class SignalizationGUI : public QGraphicsPixmapItem
{
    public:
      SignalizationGUI();
      void setImage(ImageSignalisation* image);
      void setIntersection(const Plan::Intersection *intersection);

    private:
      QString _Name;
      QMap<QString, QGraphicsPixmapItem*> _LightImages;
      const Plan::Intersection *_Intersection;
};

#endif
