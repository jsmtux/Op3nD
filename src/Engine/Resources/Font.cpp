#include "Font.h"
#include <fstream>
#include "../Math/Vector2.h"
#include "../Graphics/Shading.h"
#include "Model3d.h"
#include "../../ProjectManagement/Project.h"

GLuint Font::IBO=0;
Shading* Font::textShader;

void Font::init()
{
  textShader= new Shading;
  textShader->initShader(Project::common()->getDir("text.sfx",Project::SHADER));
}

Font::Font(map<char,Glyph> glyphs, int _line_height, float _tex_line_height, GLuint _texture)
{
  init_VBO();
  table = glyphs;
  line_height = _line_height;
  tex_line_height = _tex_line_height;
  texture = _texture;
  
}

Resource::ResourceType Font::getType()
{
  glDeleteTextures(1, &texture);
  return FONT;
}

void Font::init_VBO()
{
  if(IBO!=0){
    return;
  }
  unsigned int Indices[] = { 2, 1, 0, 3, 2, 0  };
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Font::Draw(string text)
{
  if(text.empty()){
    text="ABCDEFGHIJKLMNOPQRSTVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789";
  }
  float x=0,y=0;
  Matrix baseMat=Shading::getActive()->getObjMat();
  for(char c:text){
    if(c=='\n'){
      y+=line_height;
      continue;
    }
    Glyph g= table[c];
    Matrix trans;
    trans.setTranslationTransform(x,y,0);
    Shading::getActive()->setObjMat(baseMat * trans );
    draw(g);
    x+=g.advance*0.1;
  }
  Shading::getActive()->setObjMat(baseMat);
}

void Font::draw(Glyph g)
{
#ifndef NODRAW
  Shading::push();
  textShader->useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    glEnableVertexAttribArray(Shading::getActive()->getPosLocation());
    glEnableVertexAttribArray(Shading::getActive()->getTexLocation());
    
    glBindBuffer(GL_ARRAY_BUFFER, g.VBO);
    glVertexAttribPointer(Shading::getActive()->getPosLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(Shading::getActive()->getTexLocation(), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
    
    glDisableVertexAttribArray(Shading::getActive()->getPosLocation());
    glDisableVertexAttribArray(Shading::getActive()->getTexLocation());
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBindTexture(GL_TEXTURE_2D, 0);
  Shading::pop();
#endif
}

