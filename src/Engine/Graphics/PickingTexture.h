/* 
 * File:   PickingTexture.h
 * Author: jsmtux
 *
 * Created on October 15, 2012, 12:43 PM
 */

#ifndef PICKINGTEXTURE_H
#define	PICKINGTEXTURE_H

#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

class PickingTexture{
public:
    PickingTexture(unsigned int w, unsigned int h);
    ~PickingTexture();
    unsigned int* ReadId(unsigned int x,unsigned int y);
    void bind();
    void unBind();
private:
    GLuint fbo, picking, depth;
    unsigned int height;
};

#endif	/* PICKINGTEXTURE_H */

