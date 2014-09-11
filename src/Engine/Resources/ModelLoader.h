#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include "BaseResourceLoader.h"

class ModelLoader: public BaseResourceLoader{
public:
  Project::FileType getFileType() {return Project::MESH;}
private:
  Resource* loadInternal(State* state, string dir);
  vector<Resource*> initMaterials(State* state, const aiScene* scene);
};

#endif