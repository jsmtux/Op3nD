/**
 * @(#) State.h
 */

#ifndef STATE_H_H
#define STATE_H_H
#include <vector>
#include <string>
#include <thread>
#include <mutex>
using namespace std;
#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#endif
#include <btBulletDynamicsCommon.h>

#include "../../ProjectManagement/MXML.h"
using namespace MXML;

#include "../Timer.h"
#include "../Controller.h"
#include "../ObjectTypes/Scripted.h"
#include "../../Network/NetNode.h"

class Editable;
class Object;
class Shading;
class Camera;
class PhysicsWorld;
class PickingTexture;
class Tile;

typedef enum {EMPTYST, MESHST, EDITORST, GAMEST}StateType;

class State: public Networkable
{	
public:
    /**
     * Creates a new state
     * @param n name of the state
     */
    State(string n);
    State(State& s);
    ~State();
    /**
     * Runs the new state
     */
    void beginUpdateLoop( );
    /**
     * Returns the name of the state
     * @return name of the state
     */
    string getName();
    /**
     * Returns the type of the state either empty, editor or game
     * @return enum that indicates the state type
     */
    virtual StateType getType();
    /**
     * Stops the state
     */
    void pause();
    /**
     * Selects the object in the positions x y
     * @param x mouse postition
     * @param y mouse position
     * @return id of the object
     */
    unsigned int* selection( const int x, const int y );
    /**
     * Performs an iteration on the state
     */
    virtual void iteration();
    /**
     * adds a new element
     * @param a object to be added
     */
    void addElement(Editable *a);
    /**
     * Adds a new element to elements' list, asigning it an id
     */
    void addToList(Editable *a,unsigned int i=0);
    /**
     * Deletes an ellement
     */
    void deleteElement(unsigned int id);
    /**
     * Loads an state saved in a file
     */    
    void loadFile();
    /**
     * Draws the scene
     */
    virtual void draw();
    /**
     * Updates the scene
     */
    virtual void updateElements();
    /**
     * Removes all elements from the state
     */
    void clear();
    /**
     * Adds a new camera to the scene
     * @return camera number in the scene
     */
    int addCam(Camera* cam=nullptr);
    /**
     * Sets the current camera
     */
    void setCam(int no);
    /**
     * Returns the current camera
     * @return 
     */
    Camera* getCam();
    /**
     * Returns any editable item by id
     */
    Editable* getByIndex(unsigned int ind);
    
    void initPhysicsWorld();
    /**
     * Iterates the physics world according to the time elapsed since last call
     * Does not affect the state, just interpolates
     */
    void stepPhysWorld();
    /**
     * Function called every time the physics world is going to be updated
     * @param world pointer to the physics world
     * @param timeStep amount of time elapsed since last call
     */
    static void physicsCallback(btDynamicsWorld *world, btScalar timeStep);
    /**
     * Adds a new physical rigid body object to the physics world
     * @param body body to be added
     */
    void addRigidBody(btRigidBody *body);
    /**
     * deletes a physical rigid body from the physics world
     * @param del
     */
    void deleteRigidBody(btRigidBody *del);
    
    /**
    void setNetNode(NetNode *n);
     * Networkable definitions
     */
    MXML::Tag toXML();
    void fromXML(MXML::Tag &code);
    MXML::Tag difference(Networkable &n);
    
    void setNetNode(NetNode *n);
    void lockUpdate();
    void unlockUpdate();
    
    /**
     * Returns the time elapsed since last update
     * @return time since last update
     */
    tMillis getDiffTime();
    
    void updateResolution();
    
    void setDebug(bool debug=true);
    
    PhysicsWorld* getPhysicsWorld();
private:
    bool isStopped;
    /**
     * Alternative to the physics callback, unused otherwise
     * TODO: update to use thread library
     * @param p
     * @return 
     */
    void updateLoop();
    thread *updateTh;
    Timer tUpdated;
    mutex *updateLock;

protected:
    
    PhysicsWorld *pWorld;
    bool deleteOnEnd;
    bool debug;
    string name;
    
    NetNode *netNode;

#ifndef NODRAW
    Shading* stest, *sselect;
    PickingTexture *ptext;
#endif    
    vector<Editable*> list;
    vector<Camera*> cameras;
    Camera* currCam;
    vector<Tile*> tiles;
    vector<Scripted*> objects;
    
    tMillis diffTime;
};

#endif
