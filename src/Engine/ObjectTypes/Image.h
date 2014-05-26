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
    /**
     * Returns the id of an image, if it is not loaded loads it
     * @param dir directory of the image to be loaded
     * @return pointer to the image
     */
    static Image *loadIm(string dir);
    void Draw();
    /**
     * Returns the name of an image
     * @return name of an image
     */
    ResourceType getType();
    void Bind(Vector2 offset=Vector2(0,0), Vector2 size=Vector2(1,1));
    void del();
    static void init();
    static void unBind();
private:
#ifndef NODRAW
    static GLuint IBO,VBO;
    GLuint imageId;
#endif
    static map<string,Image*> list;
    Image(string dir);
};

#endif
