/**
 * @(#) Image.h
 */

#ifndef TILE_H_H
#define TILE_H_H

#include <string>

#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
using namespace std;

#include "Editable.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
class Resource;

class Tile:public Editable{	
public:
    Tile(Tile &tile);
    Tile(MXML::Tag &code);
    Tile(Vector3 p, Vector3 s, Quaternion r, string dirResource="");
    ~Tile();
    void draw( );
    MXML::Tag toXML();
    MXML::Tag difference(Networkable &n);
    void fromXML(MXML::Tag &code);
    Vector3 getPos();
    Vector3 getSize();
    Quaternion getRot();
    Vector3 getLinVel();
    Vector3 getAngVel();
    void setPos(Vector3 p);
    void setRot(Quaternion s);
    void setSize(Vector3 r);
    void setLinVel(Vector3 v);
    void setAngVel(Vector3 v);
    virtual edType getType();
    void setPhysical(btRigidBody* p);
    void setPhysical(MXML::Tag &code);
    btRigidBody* getPhysical();
    void setResource(string dirResource);
    Resource* getResource();
    vector<Tile*> getColliding();
protected:
    Resource* resource;
    Vector3 position,size;
    Quaternion rotation;
private:
    Matrix getTrans();
    void updateMotionState();
#ifndef NODRAW
    static GLuint VBO, IBO;
#endif
    bool colored;
    void load();
    btRigidBody* physInfo;
};

#endif
