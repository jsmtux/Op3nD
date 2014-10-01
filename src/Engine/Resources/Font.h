#ifndef _FONT_H
#define _FONT_H
#include "Resource.h"
#include "Image.h"
#include "Graphics/Shader.h"

class Font: public Resource{
public:
  ResourceType getType();
  void Draw(Shader* shader, string text);
  void Draw(Shader* shader){Draw(shader, "Test");}
  static void init();
  struct Glyph
  {
    GLuint VBO;
    size_t advance;
  };
  Font(map<char,Glyph> glyphs, int line_height, float tex_line_height, GLuint texture);
  string defaultShader(){return "text.sfx";}
private:
  void del();
  void init_VBO();
  void draw(Shader* shader, Font::Glyph g);
  static GLuint IBO;
  map<char,Glyph> table;
  int line_height;
  float tex_line_height;
  GLuint texture;
};

#endif
