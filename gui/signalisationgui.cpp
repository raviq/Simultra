#include <QFont>
#include <QPen>
#include <QGraphicsView>
#include "signalisationgui.h"
#include "utils.h"
#include "imagesignalisation.h"
#include "simulation.h"
#include "config.h"

SignalizationGUI::SignalizationGUI()
{
}

// Initialise _Intersection for this SignalizationGUI object and render a red light

void SignalizationGUI::setIntersection(const Plan::Intersection *intersection)
{
  if (_Intersection != intersection)
  {
    _LightImages.clear();

    Simulation *s = Simulation::instance();
    Plan::RoadsList roads  = s->_Plan->getRoads(intersection);

    foreach(Plan::Road *road, roads)
    {
          Plan::Road::Lane::Direction dir = road->getDirection(intersection);
          unsigned int nbLanes = road->nbLanes(dir);

          for (unsigned int i=0; i < nbLanes; i++)
          {
                bool resultOK = true;
                Plan::Road::Lane lane = road->lane(dir, i, resultOK);
                if (resultOK)
                {
                      QGraphicsPixmapItem *redLight = new QGraphicsPixmapItem;
                      redLight->setPixmap(QPixmap(_RED_LIGHT_ICON_));

                      if (lane._Direction == Plan::Road::Lane::FIRST_TO_SECOND)
                        redLight->setPos(lane._Segment.start());
                      else
                        redLight->setPos(lane._Segment.end());

                      redLight->setZValue(Utils::LANE_LAYER);
                      redLight->setScale(0.05);

                      // Translate
                      QTransform t = redLight->transform();
                      t.translate(-redLight->pixmap().width()/2, -redLight->pixmap().height()/2);
                      redLight->setTransform(t);

                      QString key = road->name() + QString::number(i);
                      _LightImages[key] = redLight;
                }
           }
    }
    _Intersection = intersection;
  }
}

// Render a green, orange, or red light according to image parameters

void SignalizationGUI::setImage(ImageSignalisation* image)
{
  setIntersection(image->intersection());

  Signalisation::ConnectionsList connections = image->state().getConnections();

  for (int i  =0 ; i < connections.size() ; i++)
  {
    Signalisation::Connection *connection = connections.at(i);
    if (connection->_Start)
    {
      QString cle = connection->_Start->name() + QString::number(connection->_LaneStart);
      QMap<QString, QGraphicsPixmapItem*>::iterator counter = _LightImages.find(cle);

      if (counter != _LightImages.end())
      {
        QGraphicsPixmapItem *pixmap = counter.value();

        if (pixmap)
        {
          Signalisation::LIGHT_COLOUR colour = image->state().lightColor();
          switch (colour)
          {
              case Signalisation::GREEN: pixmap->setPixmap(QPixmap(_GREEN_LIGHT_ICON_));
              break;
              case Signalisation::ORANGE: pixmap->setPixmap(QPixmap(_ORANGE_LIGHT_ICON_));
              break;
              case Signalisation::RED: pixmap->setPixmap(QPixmap(_RED_LIGHT_ICON_));
              break;
          }
        }
      }
    }
  }
}
