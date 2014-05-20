#include "Font.h"
#include <fstream>
#include "../Math/Vector2.h"
#include "../Graphics/Shading.h"
#include "Model3d.h"
#include "../../ProjectManagement/Project.h"

map<string, Font*> Font::list;
GLuint Font::IBO=0;
Shading* Font::textShader;

template<class T, class S>
void readObject(T& to_read, S& in)
{
  in.read(reinterpret_cast<char*>(&to_read), sizeof(T));
}

void Font::init()
{
  textShader= new Shading;
  textShader->initShader(Project::common()->getDir("text.sfx",Project::SHADER));
}

Font::Font(string dir)
{
  init_VBO();
  name=dir;
  std::ifstream input(dir, std::ios::binary);
  if (input.fail() || input.get() != 'F' || input.get() != '0')
    throw std::runtime_error("Not a valid font file.");
  cout << "Loading valid font file\n";
  // Get the texture size, the number of glyphs and the line height.
  size_t width, height, n_chars;
  readObject(width, input);
  readObject(height, input);
  readObject(line_height, input);
  readObject(n_chars, input);
  tex_line_height = static_cast<float>(line_height) / height;
 
  // Read every glyph, store it in the glyph array and set the right
  // pointer in the table.
  
  struct Glyph_Buffer
  {
    unsigned char ascii, width;
    unsigned short x, y;
  };

  Glyph_Buffer buffer;
  for (size_t i = 0; i < n_chars; ++i){
    readObject(buffer, input);
    Glyph tmp;
    float tex_x1 = static_cast<float>(buffer.x) / width;
    float tex_x2 = static_cast<float>(buffer.x + buffer.width) / width;
    float tex_y1 = static_cast<float>(buffer.y) / height;
    float tex_y2= tex_y1+tex_line_height;
    
    Vertex Vertices[4] = {
    Vertex(Vector3(.5f, -0.5f, 0.0f), Vector2(tex_x2, tex_y2), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(.5f, 0.5f, 0.0f), Vector2(tex_x2, tex_y1), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector2(tex_x1, tex_y1), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(-.5f, -0.5f, 0.0f), Vector2(tex_x1, tex_y2), Vector3(0.0f,1.0f,0.0f)) };
    
    glGenBuffers(1, &tmp.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, tmp.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    tmp.advance = buffer.width;
 
    table.insert(pair<char,Glyph>(buffer.ascii,tmp));
  }
  cout << "loaded: " << table.size() << " fonts" << endl;
  // We must have the default character (stored under '\xFF')
  if (table.find((char)'\xFF') == table.end())
    throw std::runtime_error("Font file contains no default glyph");
 
  // Store the actual texture in an array.
  unsigned char* tex_data = new unsigned char[width * height];
  input.read(reinterpret_cast<char*>(tex_data), width * height);
 
  // Generate an alpha texture with it.
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, width, height, 0, GL_ALPHA,
               GL_UNSIGNED_BYTE, tex_data);
  // And delete the texture memory block
  delete[] tex_data;
}

Resource::ResourceType Font::getType()
{
  glDeleteTextures(1, &texture);
  return FONT;
}

Font* Font::loadFont(string dir)
{
    map<string,Font*>::iterator tmp;
    if((tmp=list.find(dir))==list.end()){
        Font* ret=new Font(dir);
        list.insert(pair<string, Font*>(dir,ret));
        return ret;
    }
    tmp->second->copies++;
    return tmp->second;
}

void Font::del()
{
  list.erase(name);
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

