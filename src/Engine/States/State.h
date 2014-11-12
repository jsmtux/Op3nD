/**
 * @(#) State.h
 */

#ifndef STATE_H_H
#define STATE_H_H
#include <vector>
#include <string>
#include <thread>
using namespace std;
#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#endif
#include <btBulletDynamicsCommon.h>

#include "MXML.h"
using namespace MXML;

#include "Timer.h"
#include "Controller.h"
#include "ObjectTypes/Scripted.h"
#include "../../Network/NetNode.h"
#include "Graphics/Renderer.h"

class Editable;
class Object;
class Shader;
class Camera;
class PhysicsWorld;
class Tile;
class StateUpdate;
class ResourceManager;
class ScriptManager;

typedef enum {EMPTYST, MESHST, EDITORST, GAMEST}StateType;

class State: public Networkable
{
public:
    /**
     * Creates a new state
     * @param resourceManager  handles the resouce loading
     * @param n name of the state
     */
    State(ResourceManager* resourceManger, string n);
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
     * Performs a graphics iteration on the state
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
    void deleteElement(Editable *ed);
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
    
    unsigned int* selection( const int x, const int y );
    
    /**
     * Returns the time elapsed since last update
     * @return time since last update
     */
    tMillis getDiffTime();
    
    void updateResolution();
    
    void setDebug(bool debug=true);
    
    PhysicsWorld* getPhysicsWorld();
    
    Resource* loadResource(string dir);
    
    void* addDrawElement(Editable* editable, string shaderName);

    unique_ptr<ScriptManager> scriptManager;
protected:
    StateUpdate* stateUpdate;
    PhysicsWorld *pWorld;
    bool deleteOnEnd;
    bool debug;
    string name;
    
    NetNode *netNode;
    ResourceManager *resourceManager;
    
    Renderer forwardRenderer;
    PickingRenderer pickingRenderer;

    vector<Editable*> list;
    vector<Camera*> cameras;
    Camera* currCam;
    vector<Tile*> tiles;
    vector<Scripted*> objects;
};

#endif
