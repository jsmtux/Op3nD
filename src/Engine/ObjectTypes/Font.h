#ifndef _FONT_H
#define _FONT_H
#include "Resource.h"
#include "Image.h"

class Font: public Resource{
public:
  ResourceType getType();
  static Font* loadFont(string dir);
  void Draw(string text);
private:
  Font(string dir);
  void del();
  void init_VBO();
  struct Glyph
  {
    GLuint VBO;
    size_t advance;
  };
  void draw(Glyph g);
  static GLuint IBO;
  map<char,Glyph> table;
  static map<string, Font*> list;
  int line_height;
  float tex_line_height;
  GLuint texture;
};

#endif