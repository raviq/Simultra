#include "imagesmanager.h"
#include "simulation.h"
#include "signalisation.h"

// Constructor, initialises the image ID to 0
ImagesManager::ImagesManager() : _IdCurrentImage(0)
{

}

// Dest
ImagesManager::~ImagesManager()
{
    deleteImages();
}

// Gets the instance of the running simulation as well as the elements listed in _Vehicles
// and _Signalisations and adds the corresponding images to the variable _Images
// of the simulation.
void ImagesManager::addImages()
{
  Simulation* simu(Simulation::instance());
  if (simu)
  {
    Image* image(new Image());
    image->_Tps = simu->_TpsSimulation;

    foreach(Vehicule* v, simu->_Vehicles)
    {
      Point moteur, frottement, objectif;
      v->forces(moteur, frottement);
      if (v->driver())
        objectif = v->driver()->_ObjectifActuel;
      image->addImageVehicle(new ImageVehicle(
                                      v->name(),
                                      v->type()->_NameImage,
                                      v->location(),
                                      v->direction().angle(),
                                      v->acceleration(),
                                      v->velocity(),
                                      moteur,
                                      frottement));
    }

    foreach(Signalisation *s, simu->_Signalisations)
    {
      image->addImageSignalisation(new ImageSignalisation(s->intersection()->name(),
                                                              s->intersection(),
                                                              *s->currentState()));
    }
    _Images.append(image);
  }
}

// Deletess the images and sets the current image (ID) to null.
void ImagesManager::deleteImages()
{
  _IdCurrentImage = 0;
  _Images.clear();
}

// Sets the current image (ID) to the highest ID of the available images
// if no image is available , sets to null the ID of the current image.
void ImagesManager::lastImage()
{
  if (_Images.empty())
    _IdCurrentImage = 0;
  else
  {
    unsigned int nbImages = static_cast<unsigned int>(_Images.size());
    _IdCurrentImage = nbImages-1;
  }
}

// @return Image corresponding to the ID of the current image
Image* ImagesManager::currentImage()
{
  if (!_Images.empty() && _IdCurrentImage < (unsigned int)_Images.size())
    return _Images.at(_IdCurrentImage);
  return NULL;
}


// increments the ID of the current image.
void ImagesManager::nextImage(unsigned int nbImages)
{
  setCurrentImage(_IdCurrentImage+nbImages);
}

void ImagesManager::setCurrentImage(unsigned int nb)
{
  unsigned int nbImages = static_cast<unsigned int>(_Images.size());
  if (nb >= nbImages)
    _IdCurrentImage = nbImages-1;
  else
    _IdCurrentImage =  nb;
}
