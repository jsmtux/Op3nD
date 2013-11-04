/**
 * @(#) Images.cpp
 */

#include "Image.h"
#include "../Base.h"
#include "../Graphics/Shading.h"
#include "../../ProjectManagement/Project.h"
#include "Model3d.h"

#ifdef ANDROID
#include "./stb_image/stb_image.h"
#endif

map<string, Image*> Image::list;
#ifndef NODRAW
GLuint Image::VBO;
GLuint Image::IBO;
#endif

Image::Image(string dir){
#ifndef NODRAW
#ifndef ANDROID
    image.loadFromFile(Base::getInstance()->getProj()->getDir(dir,Project::IMAGE));
    sf::Texture::bind(&image,sf::Texture::Pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
#else
    int width, height, depth;
    data = stbi_load(Base::getInstance()->getProj()->getDir(dir,IMAGE).c_str(),&width,&height,&depth,0);
    glGenTextures(1,&imageId);
    glBindTexture(GL_TEXTURE_2D,imageId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete data;
#endif
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
    return tmp->second;
}

void Image::del(){
    list.erase(name);
#ifndef NODRAW
#ifndef ANDROID
    image.~Texture();
#endif
#endif
}

void Image::Bind(){
#ifndef NODRAW
#ifndef ANDROID
    glActiveTexture(GL_TEXTURE0);
    sf::Texture::bind(&image,sf::Texture::Pixels);
#else
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageId);
#endif
#endif
}

Resource::ResourceType Image::getType(){
    return IMAGE;
}

void Image::Draw(){
    Bind();
#ifndef NODRAW
    glDisable(GL_CULL_FACE);
    glEnableVertexAttribArray(Shading::getActive()->getPosLocation());
    glEnableVertexAttribArray(Shading::getActive()->getTexLocation());
//    glEnableVertexAttribArray(Shading::getActive()->getNormLocation());
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(Shading::getActive()->getPosLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(Shading::getActive()->getTexLocation(), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
//    glVertexAttribPointer(getNormLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
    
    glDisableVertexAttribArray(Shading::getActive()->getPosLocation());
    glDisableVertexAttribArray(Shading::getActive()->getTexLocation());
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
