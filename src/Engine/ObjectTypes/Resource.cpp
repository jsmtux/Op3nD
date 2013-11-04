#include "Resource.h"
#include "../../ProjectManagement/Project.h"
#include "Image.h"
#include "Model3d.h"
#include "Box.h"
#include "Line.h"
#include "Physical.h"

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
	default:
	  cerr << "Cannot load " << dir << endl;
    }
    if(ret){
      ret->copies++;
    }
    return ret;
}

void Resource::Draw(){
    ;
}

string Resource::getName(){
    return name;
}

Matrix Resource::getTransform(){
    Matrix id;
    id.setIdentity();
    return id;
}
