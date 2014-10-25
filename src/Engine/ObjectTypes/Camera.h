/**
 * @(#) Camera.h
 */

#ifndef CAMERA_H_H
#define CAMERA_H_H

#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <math.h>

#include "MXML.h"

#include "Tile.h"
#include "Editable.h"

#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"

class Renderer;

class Camera: public Editable{	
public:
    Camera(State* state, float ratio=-1, float fov=30.f, float zNear=0.1f, float zFar=1000.f);
    Camera(State* state, MXML::Tag &code);
    /**
     * Sets the camera position
     * @param e camera position
     * @param o camera direction
     * @param u up vector
     */
    void setPosition( Vector3 e, Vector3 o=Vector3(0,0,-1), Vector3 u=Vector3(0,1,0));
    /**
     * Updates the world position according tho the camera coordinates
     */
    void view(Renderer* renderer);    
    /**
     * Updates the world position according tho the camera coordinates
     * without perspective correction
     */
    void orthoView(Renderer* renderer);
    /**
     * Moves the camera
     * @param v amount of movement
     */
    void move(Vector3 v);
    /**
     * Rotates the camera
     * @param v amount of rotation
     */
    void orientate(Vector3 v);
    /**
     * Returns camera target
     * @return camera target
     */
    Vector3 getTarget();
    /**
     * Sets camera target
     */
    void setTarget(Vector3 t);
    /**
     * Returns camera up vector
     * @return up vector
     */
    Vector3 getU();
    Vector3 getV();
    Vector3 getN();
    void setId(unsigned int id);
    MXML::Tag toXML();
    MXML::Tag difference(Networkable &n);
    void fromXML(MXML::Tag &code);
    void notify();
    Vector3 getPos();
    Vector3 getSize();
    Quaternion getRot();
    void setPos(Vector3 p);
    void setRot(Quaternion s);
    void setSize(Vector3 r);
    void draw(Shader* shader);
    edType getType();
    void reloadMatrix();
private:
    Tile visible;
    Vector3 target,pos, up,V,U,N;
    Matrix perspMat;
    Matrix orthoMat;
    float step;
    float angleH, angleV;
    bool targetDefined;
    Vector3 direction;
};

#endif
