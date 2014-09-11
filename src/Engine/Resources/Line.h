/* 
 * File:   Line.h
 * Author: jsmtux
 *
 * Created on November 6, 2012, 9:32 PM
 */

#ifndef LINE_H
#define	LINE_H
#include "Resource.h"
class Shading;
#include "../Math/Vector3.h"
#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

class Line: public Resource{
public:
    void Draw(); 
    ResourceType getType();
    void del();
    static void drawLine(Vector3 b, Vector3 e, Vector3 c);
    static void init();
private:
    Vector3 begin, end, color;
#ifndef NODRAW
    static GLuint VBO; 
    static Shading* colorShader;
#endif
};

#endif	/* LINE_H */

