#include "Font.h"
#include <fstream>
#include "Math/Vector2.h"
#include "Graphics/Shader.h"
#include "Model3d.h"
#include "Project.h"

GLuint Font::IBO=0;
Shader* Font::textShader;

void Font::init()
{
  textShader= new Shader;
  textShader->initShader(Project::common()->getDir("text.sfx",Project::SHADER));
}

Font::Font(string name, map<char,Glyph> glyphs, int _line_height, float _tex_line_height, GLuint _texture)
  :Resource(name)
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

void Font::Draw(Shader* shader, string text)
{
  if(text.empty()){
    text="ABCDEFGHIJKLMNOPQRSTVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789";
  }
  float x=0,y=0;
  Matrix baseMat=shader->getMatrix("gObjMat");
  for(char c:text){
    if(c=='\n'){
      y+=line_height;
      continue;
    }
    Glyph g= table[c];
    Matrix trans;
    trans.setTranslationTransform(x,y,0);
    shader->setObjMat(baseMat * trans );
    draw(shader, g);
    x+=g.advance*0.1;
  }
  shader->setObjMat(baseMat);
}

void Font::draw(Shader* shader, Glyph g)
{
#ifndef NODRAW
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
  glEnableVertexAttribArray(shader->getVarLocation("Position"));
  glEnableVertexAttribArray(shader->getVarLocation("TexCoord"));
  
  glBindBuffer(GL_ARRAY_BUFFER, g.VBO);
  glVertexAttribPointer(shader->getVarLocation("Position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(shader->getVarLocation("TexCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
  
  glDisableVertexAttribArray(shader->getVarLocation("Position"));
  glDisableVertexAttribArray(shader->getVarLocation("TexCoord"));
  glDisable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);
  glEnable(GL_CULL_FACE);
#endif
}

