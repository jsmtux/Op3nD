/**
 * @(#) Light.h
 */

#ifndef LIGHT_H_H
#define LIGHT_H_H
#include "Resource.h"

class Light: public Resource
{
public:
      void draw(Shader* shader);
      string getDefaultShader(){return "normal.sfx";}
};

#endif
