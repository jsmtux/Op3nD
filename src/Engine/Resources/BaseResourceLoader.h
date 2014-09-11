#ifndef IRESOURCELOADER_H_
#define IRESOURCELOADER_H_
#include "Resource.h"
#include <Project.h>
#include <memory>

class State;

class BaseResourceLoader{
public:
  Resource* loadResource(State* state, string dir);
  virtual Project::FileType getFileType() = 0;
private:
  virtual Resource* loadInternal(State* state, string dir) = 0;
  map<string, Resource* > resourceList;
};

#endif