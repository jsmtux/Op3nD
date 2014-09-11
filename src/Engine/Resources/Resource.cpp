#include "Resource.h"
#include <Project.h>
#include "Image.h"
#include "Model3d.h"
#include "Box.h"
#include "Line.h"
#include "ObjectTypes/Physical.h"
#include "Font.h"

#include "../Base.h"

#ifdef ANDROID
#include <android/log.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

Resource::Resource(){
}

string Resource::getName(){
    return name;
}

Matrix Resource::getTransform(){
    Matrix id;
    id.setIdentity();
    return id;
}
