/**
 * @(#) RenderingContext.cpp
 */

#include "RenderingContext.h"
#include "../Base.h"

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
}

void RenderingContext::update(){
    ;
}
