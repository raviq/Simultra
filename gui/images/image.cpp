#include "image.h"

Image::Image()
{

}

void Image::addImageSignalisation(ImageSignalisation* image)
{
  _ImagesSignalisation.append(image);
}


Image::SignalisationImagesList Image::imagesSignalisation() const
{
  return _ImagesSignalisation;
}

void Image::addImageVehicle(ImageVehicle* image)
{
  _Vehicles.append(image);
}

Image::VehiclesList Image::imagesVehicule() const
{
  return _Vehicles;
}

