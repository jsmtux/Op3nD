#include "ImageLoader.h"
#include "Image.h"
#include "./stb_image/stb_image.h"

Resource* ImageLoader::loadInternal(State* state, string dir)
{
  Image* ret;
  int width, height, comp;
  unsigned char* data = stbi_load(dir.c_str(),&width,&height,&comp,0);
  ret = new Image(width, height, comp, data);
  stbi_image_free(data);
  
  return ret;
}
