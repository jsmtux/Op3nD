#include "BaseResourceLoader.h"
#include "Image.h"
#include <States/State.h>

Resource* BaseResourceLoader::loadResource(State* state, string dir)
{
  Resource* ret;
  map<string,Resource*>::iterator tmp;

  if((tmp=resourceList.find(dir))==resourceList.end()){
    ret = loadInternal(state, dir);
    resourceList.insert(pair<string, Resource* >(dir, ret));
  }
  else{
    ret = tmp->second;
  }
  return ret;
}
