/**
 * @(#) Images.h
 */

#ifndef IMAGES_H_H
#define IMAGES_H_H



#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <string>
#include <map>
using namespace std;
#include "Resource.h"
#include "../Math/Vector2.h"

class Image: public Resource
{	
public:
    void Draw(Vector2 offset, Vector2 size);
    void Draw(){Draw(Vector2(0,0), Vector2(1,1));}
    virtual ~Image();
    /**
     * Returns the name of an image
     * @return name of an image
     */
    ResourceType getType();
    void Bind(Vector2 offset=Vector2(0,0), Vector2 size=Vector2(1,1));
    void del();
    static void init();
    static void unBind();
    Image(int width, int height, int comp, unsigned char* data);
private:
#ifndef NODRAW
    static GLuint IBO,VBO;
    GLuint imageId;
#endif
};

#endif
