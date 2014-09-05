#include "PickingTexture.h"
#include <iostream>
using namespace std;

PickingTexture::PickingTexture(unsigned int w, unsigned int h){
    height=h;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    glGenTextures(1, &picking);
    glBindTexture(GL_TEXTURE_2D, picking);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, w, h,
                0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                picking, 0);

    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h,
                0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                depth, 0);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        cout << "FB error, status: "<< Status;
        fbo=0;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, 0);//por defecto
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PickingTexture::~PickingTexture()
{
  glDeleteFramebuffers(1,&fbo);
  glDeleteTextures(1,&picking);
  glDeleteTextures(1,&depth);
}

unsigned int* PickingTexture::ReadId(unsigned int x, unsigned int y)
{
    static unsigned int ret[2]={0};
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    unsigned int Pixel[4];
    glReadPixels(x, height-y-1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &Pixel);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    ret[0]=Pixel[2];
    ret[1]=Pixel[1];
    return ret;
}

void PickingTexture::bind(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}
void PickingTexture::unBind(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
