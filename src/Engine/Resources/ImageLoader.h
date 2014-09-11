#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include "BaseResourceLoader.h"

class ImageLoader: public BaseResourceLoader{
public:
  Project::FileType getFileType() {return Project::IMAGE;}
private:
  Resource* loadInternal(State* state, string dir);
};

#endif