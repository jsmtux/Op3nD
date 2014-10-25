/* 
 * File:   Line.h
 * Author: jsmtux
 *
 * Created on November 6, 2012, 9:32 PM
 */

#ifndef LINE_H
#define	LINE_H
#include "Resource.h"
class Shader;
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
    void Draw(Shader* shader); 
    ResourceType getType();
    void del();
    static void drawLine(Shader* shader, Vector3 b, Vector3 e, Vector3 c);
    static void init();
    string getDefaultShader(){return "color.sfx";}
private:
    Vector3 begin, end, color;
#ifndef NODRAW
    static GLuint VBO; 
    static Shader* colorShader;
#endif
};

#endif	/* LINE_H */

