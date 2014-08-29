/**
 * @(#) RenderingContext.cpp
 */

#include "RenderingContext.h"
#include "../Base.h"

RenderingContext::RenderingContext()
{
  setResolution(Vector3(640,480,32));
}

void RenderingContext::init(){
#ifndef ANDROID
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        cerr << "Error: " << glewGetErrorString(res)<< endl;
        throw "error on init glew";
    }
#endif
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

Vector3 RenderingContext::getResolution()
{
  return resolution;
}

void RenderingContext::setResolution(Vector3 res)
{
  resolution=res;
  glViewport(0,0,res.x,res.y);
  State* current=Base::getInstance()->getStateManager()->getCurState();
  if(current){
    current->updateResolution();
  }
}

float RenderingContext::getRatio()
{
  return resolution.x/resolution.y;
}

