#include "MeshState.h"
#include "../Math/Vector3.h"
#include "../ObjectTypes/Camera.h"
#include "../Base.h"
#include "../PhysicsWorld.h"

MeshState::MeshState(ResourceManager* _resourceManager, string name):State(_resourceManager, name){
    initPhysicsWorld();
    
    cameras.push_back(new Camera(this));
    currCam=cameras[0];
    currCam->setPosition(Vector3(0,0,-10));
}

void MeshState::iteration(){
    currCam->view(&forwardRenderer);
    currCam->orientate(Vector3(0,0,0));
    State::iteration();
    State::draw();
    if(pWorld)pWorld->draw();
}
