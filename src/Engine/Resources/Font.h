#ifndef _FONT_H
#define _FONT_H
#include "Resource.h"
#include "Image.h"
#include "../Graphics/Shading.h"

class Font: public Resource{
public:
  ResourceType getType();
  void Draw(string text);
  void Draw(){Draw("Test");}
  static void init();
  struct Glyph
  {
    GLuint VBO;
    size_t advance;
  };
  Font(map<char,Glyph> glyphs, int line_height, float tex_line_height, GLuint texture);
private:
  void del();
  void init_VBO();
  void draw(Glyph g);
  static GLuint IBO;
  map<char,Glyph> table;
  int line_height;
  float tex_line_height;
  GLuint texture;
  static Shading* textShader;
};

#endif
