#include "PlayState.h"
#include "../Base.h"
#include "../ObjectTypes/Camera.h"
#include "../Math/Vector3.h"

PlayState::PlayState(string n):State(n){
    initPhysicsWorld();
    deleteOnEnd=true;
}

PlayState::PlayState(State* s):State(s->getName()){
    initPhysicsWorld();
}

void PlayState::iteration(){
    if(cameras.size()==0){
        cameras.push_back(new Camera());
        cameras[0]->setPos(Vector3(0,15,-60));
        cameras[0]->orientate(Vector3(0,0,10));
    }
    currCam= cameras[0];
    currCam->view();
    currCam->orientate(Vector3(0,0,0));
    State::iteration();
    stepPhysWorld();
    draw();
}

void PlayState::updateElements(){
    State::updateElements();
}

void PlayState::draw(){
    State::draw();
}
