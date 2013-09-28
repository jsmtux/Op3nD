/**
 * @(#) Project.cpp
 */
#define PPATH "./proj/"

#include "Project.h"

Project::Project(string nome){
    portable=false;
    dir=nome;
    vector<string> files=listDirFiles(PPATH);
    for(vector<string>::const_iterator it=files.begin();it!=files.end();++it){
            if(dir.compare(*it)==0){
                return;
            }
    }
    string fullP=getDir();
    create_directory(PPATH);
    create_directory(fullP);
    create_directory(fullP+toString(PHYSICAL));
    create_directory(fullP+toString(FONT));
    create_directory(fullP+toString(IMAGE));
    create_directory(fullP+toString(MAP));
    create_directory(fullP+toString(SOUND));
    create_directory(fullP+toString(SKELETON));
    create_directory(fullP+toString(OBJECT));
    create_directory(fullP+toString(SHADER));
    create_directory(fullP+toString(MESH));
}

vector<string> Project::listProjs(){
    return listDirFiles(PPATH);
}

string Project::getDir(){
        return PPATH+dir+"/"; 
}

string Project::getDir(string file, FileType type){
    return PPATH+dir+toString(type)+"/"+file;
}

string Project::getDir(string file){
    return getDir(file,getType(file));
}

string Project::toString(FileType type){
    switch(type){
        case PHYSICAL:
            return("/meshes");
        case FONT:
            return("/fonts");
        case IMAGE:
            return("/images");
        case OBJECT:
            return("/objects");
        case MAP:
            return("/maps");
        case SOUND:
            return("/sounds");
        case SKELETON:    
            return("/skeletons");
        case SHADER:
            return("/shaders");
        case MESH:
            return("/meshes");
    }
}


void Project::read( ){
    MXML::Tag root;
    MXML::XMLFile file(getDir()+"project.xml",root,"projets.dtd");
    file.read();
    name = root["name"].getAttrib().getString();
    desc = root["description"].getAttrib().getString();
    vers = root["version"].getAttrib().getString();
    portable = root["portable"].getAttrib().getInt();
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

bool Project::getPort(){
    return portable;
}

void Project::setRemoteProjs(vector<string> p){
    remoteProjs=p;
}

FileType Project::getType(string dir){
    std::string::size_type dot=dir.find_last_of(".");
    if(dot==string::npos){
        return BOX;
    }
    string extension=dir.substr(dot,dir.size()-dot);
    if(extension.compare(".png")==0||extension.compare(".jpg")==0
            ||extension.compare(".pcx")==0||extension.compare(".tga")==0)
        return IMAGE;
    if(extension.compare(".md2")==0||extension.compare(".ase")==0
            ||extension.compare(".pk3")==0||extension.compare(".3ds")==0
            ||extension.compare(".obj")==0||extension.compare(".x")==0
            ||extension.compare(".md5mesh")==0||extension.compare(".blend")==0
            ||extension.compare(".dae")==0||extension.compare(".ms3d")==0)
        return MESH;
    if(extension.compare(".o3s")==0)
        return OBJECT;
    if(extension.compare(".xml")==0){
        return PHYSICAL;
    }
}

vector<string> Project::listFiles(FileType file){
    return listDirFiles(getDir("",file),false);
}