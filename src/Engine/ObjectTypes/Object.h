/**
 * @(#) Object.h
 */

#ifndef OBJECT_H_H
#define OBJECT_H_H

#include <btBulletDynamicsCommon.h>
#include "Editable.h"
#include "../Math/Quaternion.h"

class Tile;

class Object: public Editable
{	
public:
    /**
     * Constructor. Initializes an empty object
     */
    Object();
    ~Object();
    /**
     * Constructor, creates an object from a tile
     * @param t tile included in the object
     */
    Object(Tile* t);
    Object(Object &o);
    Object(MXML::Tag &code);
    MXML::Tag difference(Networkable &n);
    void setId(unsigned int id);
    
    void notify( );
    /**
     * Called in each iteration updates the object
     */
    virtual void update( );
    /**
     * Draws the object
     */
    void draw();
    Vector3 getPos();
    /**
     * Returns the object's rotation at a given time
     * @return the object's rotation
     */
    Quaternion getRot();
    /**
     * Returns the object's size at a given time
     * @return the object's size
     */
    Vector3 getSize();
    /**
     * Changes the position of the object in the next frame
     * @param p new position
     */
    void setPos(Vector3 p);
    /**
     * Changes the rotation of the object in the next frame
     * @param r new rotation
     */
    void setRot(Quaternion r);
    /**
     * Changes the size of the object in the next frame
     * @param s new size
     */
    void setSize(Vector3 s);
    Vector3 getLinVel();
    Vector3 getAngVel();
    void setLinVel(Vector3 v);
    void setAngVel(Vector3 v);    
    
    void addResource(Tile *vis);
    edType getType();
    void setPhysical(btRigidBody* p);
    btRigidBody* getPhysical();
    virtual void fromXML(MXML::Tag &code);
    virtual MXML::Tag toXML();
protected:
    vector<Tile*> resources;
};

#endif
