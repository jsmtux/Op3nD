#include "MeshState.h"
#include "../Math/Vector3.h"
#include "../ObjectTypes/Camera.h"
#include "../Base.h"
#include "../PhysicsWorld.h"

MeshState::MeshState(string name):State(name){
    initPhysicsWorld();
    
    cameras.push_back(new Camera());
    currCam=cameras[0];
    currCam->setPosition(Vector3(0,0,-10));
}

void MeshState::iteration(){
    currCam->view();
    currCam->orientate(Vector3(0,0,0));
    State::iteration();
    State::draw();
    if(pWorld)pWorld->draw();
}
