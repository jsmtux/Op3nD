#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_
#include <Base.h>
#include "BaseResourceLoader.h"
#include "Project.h"

class Base;

class ResourceManager {
public:
  ResourceManager(Base* base);
  Resource* loadResource(State* state, string dir);
  void addResourceLoader(BaseResourceLoader *loader);
protected:
  Base* base;
private:
  map<Project::FileType, BaseResourceLoader*> loaders;
};

#endif