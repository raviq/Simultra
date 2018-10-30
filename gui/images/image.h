#ifndef IMAGE_H
#define IMAGE_H
#include <QTime>
#include "imagesignalisation.h"
#include "imagevehicle.h"

class Image
{
    public:
      typedef QList<ImageSignalisation*> SignalisationImagesList;
      typedef QList<ImageVehicle*> VehiclesList;
      Image();
      void addImageSignalisation(ImageSignalisation* image);
      SignalisationImagesList imagesSignalisation() const;
      void addImageVehicle(ImageVehicle* image);
      VehiclesList imagesVehicule() const;
      QTime _Tps;

    protected:
      SignalisationImagesList _ImagesSignalisation;
      VehiclesList _Vehicles;
    };

#endif
