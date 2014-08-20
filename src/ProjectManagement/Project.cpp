/**
 * @(#) Project.cpp
 */
#define PRO_CONF_FILE "project.xml"

#include <algorithm>

#define COMMON_PATH "/usr/local/share/O3D/"

#include "Project.h"

Project* Project::commonProject=new Project(COMMON_PATH);

Project::Project(string dir){
  this->dir=dir;
  
  if(dir.compare(COMMON_PATH)!=0){
    read();
    create_directories(dir+"/"+toString(IMAGE));
    create_directories(dir+"/"+toString(MAP));
    create_directories(dir+"/"+toString(SOUND));
    create_directories(dir+"/"+toString(OBJECT));
    create_directories(dir+"/"+toString(SHADER));
    create_directories(dir+"/"+toString(MESH));
    create_directories(dir+"/"+toString(FONT));
    create_directories(dir+"/"+toString(SPRITE));
  }
}

bool Project::createProject(string name, string dir, string vers, string desc)
{
  if(!create_directories(dir)){
    return false;
  }
  MXML::Tag root("project");
  root.addChildren(MXML::Tag("name"));
  root.addChildren(MXML::Tag("description"));
  root.addChildren(MXML::Tag("version"));
  root["name"].setAttrib(MXML::Attribute(name));
  root["version"].setAttrib(MXML::Attribute(vers));
  root["description"].setAttrib(MXML::Attribute(desc));
  MXML::XMLFile file(dir+"/"+PRO_CONF_FILE,root,"projects.dtd");
  file.write();
  
  return true;
}

string Project::getDir(){
  return dir+"/"; 
}

string Project::getDir(string file, FileType type){
  return dir+"/"+toString(type)+"/"+file;
}

string Project::getDir(string file){
  return getDir(file,getType(file));
}

string Project::toString(FileType type){
  switch(type){
    case IMAGE:
      return("images");
    case OBJECT:
      return("objects");
    case MAP:
      return("maps");
    case SOUND:
      return("sounds");
    case SHADER:
      return("shaders");
    case MESH:
      return("meshes");
    case PHYSICAL:
      return("meshes");
    case FONT:
      return("fonts");
    case SPRITE:
      return("sprites");
    default:
      return "";
  }
}


void Project::read( ){
  MXML::Tag root;
  MXML::XMLFile file(getDir()+"project.xml",root,"projects.dtd");
  file.read();
  name = root["name"].getAttrib().getString();
  desc = root["description"].getAttrib().getString();
  vers = root["version"].getAttrib().getString();
}


string Project::getName(){
  return name;
}

string Project::getVers(){
  return vers;
}

string Project::getDesc(){
  return desc;
}

void Project::setRemoteProjs(vector<string> p){
  remoteProjs=p;
}

Project::FileType Project::getType(string dir){
  std::string::size_type dot=dir.find_last_of(".");
  const vector<string> imageFiles={".png",".jpg",".pcx",".tga"};
  const vector<string> meshFiles={".md2",".ase",".pk3",".3ds",".obj",".x",".md5mesh",".blend",".dae",".ms3d"};
  if(dot==string::npos){
    return BOX;
  }
  string extension=dir.substr(dot,dir.size()-dot);
  if(std::find(imageFiles.begin(),imageFiles.end(),extension)!=imageFiles.end()){
    return IMAGE;
  }
  if(std::find(meshFiles.begin(),meshFiles.end(),extension)!=meshFiles.end()){
    return MESH;
  }
  if(extension.compare(".o3s")==0){
    return OBJECT;
  }
  if(extension.compare(".xml")==0){
    return PHYSICAL;
  }
  if(extension.compare(".o3f")==0){
    return FONT;
  }
  if(extension.compare(".o3sp")==0){
    return SPRITE;
  }
}

vector<string> Project::listFiles(FileType file){
  return listDirFiles(getDir("",file),false);
}

bool Project::isProject(string dir)
{
  vector<string> files = listDirFiles(dir,false);
  return std::find(files.begin(),files.end(),PRO_CONF_FILE)!=files.end();
}

Project* Project::common()
{
  return commonProject;
}