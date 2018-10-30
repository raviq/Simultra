#ifndef ImagesManager_H
#define ImagesManager_H
#include "image.h"

class ImagesManager
{
    public:
      typedef QList<Image*> ImagesList;

      ImagesManager();
      virtual ~ImagesManager();
      void addImages();
      void deleteImages();
      Image* currentImage();
      void setCurrentImage(const unsigned int id);
      void nextImage(unsigned int nbImages=1);
      void lastImage();

    protected:
      ImagesList _Images;
      unsigned int _IdCurrentImage;
};

#endif
