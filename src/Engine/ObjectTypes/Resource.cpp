#include "Resource.h"
#include <Project.h>
#include "Image.h"
#include "Model3d.h"
#include "Box.h"
#include "Line.h"
#include "Physical.h"
#include "Font.h"

#include "../Base.h"

#ifdef ANDROID
#include <android/log.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

Resource::Resource(){
    copies=0;
}

void Resource::free(){
    copies--;
    if(!copies){
      del();
    }
}

Resource* Resource::Load(string dir){
    Resource *ret=NULL;
    if(dir[0]=='*'){
      dir=Project::common()->getDir(dir.c_str()+1,Project::getType(dir));
    }if(dir.find('/')==string::npos){
      dir=Base::getInstance()->getProj()->getDir(dir,Project::getType(dir));
    }
    switch(Project::getType(dir)){
        case Project::IMAGE:
          ret=Image::loadIm(dir);
          break;
        case Project::MESH:
          ret=Model3d::loadM3d(dir);
          break;
        case Project::BOX:
          ret=Box::loadBox();
          break;
	case Project::FONT:
	  ret=Font::loadFont(dir);
	  break;
	default:
	  cerr << "Cannot load " << dir << endl;
    }
    if(ret){
      ret->copies++;
    }
    return ret;
}

string Resource::getName(){
    return name;
}

Matrix Resource::getTransform(){
    Matrix id;
    id.setIdentity();
    return id;
}
