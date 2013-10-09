/**
 * @(#) RenderingContext.h
 */

#ifndef RENDERINGCONTEXT_H_H
#define RENDERINGCONTEXT_H_H


#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#include "../Math/Vector3.h"
#include <cmath>

class RenderingContext{
protected:
    Vector3 bgColor;
    string appName;
public:
    virtual void init();
    virtual void update();
};

#endif
