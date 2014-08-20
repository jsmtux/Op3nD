/**
 * @(#) Images.cpp
 */

#include "Image.h"
#include "../Graphics/Shading.h"
#include "../../ProjectManagement/Project.h"
#include "Model3d.h"

#include "./stb_image/stb_image.h"

map<string, Image*> Image::list;
#ifndef NODRAW
GLuint Image::VBO;
GLuint Image::IBO;
#endif

Image::Image(string dir){
#ifndef NODRAW
    int width, height, comp;
    unsigned char* data = stbi_load(dir.c_str(),&width,&height,&comp,0);
    glGenTextures(1,&imageId);
    glBindTexture(GL_TEXTURE_2D,imageId);
#ifndef ANDROID    
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
#else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif
    if(comp==4){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else{
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
#endif
    name = dir;
}

Image* Image::loadIm(string dir){
    map<string,Image*>::iterator tmp;
    if((tmp=list.find(dir))==list.end()){
        Image* ret=new Image(dir);
        list.insert(pair<string, Image*>(dir,ret));
        return ret;
    }
    tmp->second->copies++;
    return tmp->second;
}

void Image::del(){
  glDeleteTextures(1,&imageId);
  list.erase(name);
}

void Image::Bind(Vector2 offset, Vector2 size){
#ifndef NODRAW
  Shading* current = Shading::getActive();
  GLuint pos;
  if((pos=current->getVarLocation("offset"))!=-1){
    glUniform2f(pos, offset.x, offset.y);
  }
  if((pos=current->getVarLocation("size"))!=-1){
    glUniform2f(pos, size.x, size.y);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, imageId);
#endif
}

Resource::ResourceType Image::getType(){
    return IMAGE;
}

void Image::Draw(Vector2 offset, Vector2 size){
    Bind(offset, size);
    cout << "Drawing!!!!!!!!!!1111" << endl;
#ifndef NODRAW
    Shading* current = Shading::getActive();
    glDisable(GL_CULL_FACE);
    glEnableVertexAttribArray(current->getPosLocation());
    glEnableVertexAttribArray(current->getTexLocation());
//    glEnableVertexAttribArray(Shading::getActive()->getNormLocation());
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(current->getPosLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(current->getTexLocation(), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
//    glVertexAttribPointer(getNormLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
    
    glDisableVertexAttribArray(current->getPosLocation());
    glDisableVertexAttribArray(current->getTexLocation());
//    glDisableVertexAttribArray(Shading::getActive()->getNormLocation());
    glEnable(GL_CULL_FACE);
#endif
}

void Image::init(){    
#ifndef NODRAW
    Vertex Vertices[4] = {
    Vertex(Vector3(.5f, -0.5f, 0.0f), Vector2(0.0f, 0.0f), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(.5f, 0.5f, 0.0f), Vector2(0.0f, 1.0f), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector2(1.0f, 1.0f), Vector3(0.0f,1.0f,0.0f)),
    Vertex(Vector3(-.5f, -0.5f, 0.0f), Vector2(1.0f, 0.0f), Vector3(0.0f,1.0f,0.0f)) };
    
    unsigned int Indices[] = { 2, 1, 0, 3, 2, 0  };
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

void Image::unBind()
{
  glBindTexture(GL_TEXTURE_2D,0);
}
