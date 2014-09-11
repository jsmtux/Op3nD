#include "ResourceManager.h"
#include <Project.h>

ResourceManager::ResourceManager(Base* _base)
{
  base = _base;
}


void ResourceManager::addResourceLoader(BaseResourceLoader* loader)
{
  loaders.insert(pair<Project::FileType, BaseResourceLoader*>(loader->getFileType(), loader));
}

Resource* ResourceManager::loadResource(State* state, string dir)
{
  cout << "Resource loader for " << dir << endl;
  Project::FileType type = Project::getType(dir);
  cout << "load resource in " << dir << "found type in " << this << endl;
  Resource *res = nullptr;
  map<Project::FileType, BaseResourceLoader*>::iterator it= loaders.find(type);
  cout << "load resource in " << dir << "found loader" << endl;
  if (it != loaders.end())
  {
    BaseResourceLoader* loader = it->second;
    
    if(dir[0]=='*'){
      dir=Project::common()->getDir(dir.c_str()+1,type);
    } if(dir.find('/')==string::npos){
      dir=Base::getInstance()->getProj()->getDir(dir,type);
    }
    cout << "Calling loader " << endl;;
    res = loader->loadResource(state, dir);
  }
  else{
    cout << __func__ << ": Loader not found for file " << dir << endl;
  }
  
  return res;
}
