/**
 * @(#) State.cpp
 */

#include "State.h"

#include "../Base.h"
#include "../Timer.h"
#include "../Controller.h"
#include "../PhysicsWorld.h"

#include "../ObjectTypes/Camera.h"
#include "../ObjectTypes/Editable.h"
#include "../ObjectTypes/Object.h"
#include "../ObjectTypes/Model3d.h"
#include "../ObjectTypes/Scripted.h"
#include "../ObjectTypes/Box.h"
#include "../ObjectTypes/Line.h"

#include "../Graphics/Shading.h"
#include "../Graphics/PickingTexture.h"
#include "../Graphics/RenderingContext.h"
#include "../../ProjectManagement/Project.h"
#include "../ObjectTypes/Image.h"

#include <chrono>
#include <thread>

State::State(string n){
    isStopped=true;
    name=n;
    pWorld =NULL;
    deleteOnEnd=false;
    updateTh = NULL;
    updateLock= new mutex;
#ifndef NODRAW
    stest=new Shading();
    stest->initShader("normal.sfx");
    sselect=new Shading();
    sselect->initShader("picking.sfx");

    Vector3 res=Base::getInstance()->getResolution();    
    
    ptext = new PickingTexture(res.x,res.y);
    
    stest->useProgram();
#endif
    netNode=NULL;
    diffTime= chrono::duration_cast<tMillis>(chrono::seconds(0));
;}

State::State(State& s){
    for(Camera *c:s.cameras){
        //cameras.push_back(new Camera(*c));
    }
    for(Tile *t:s.tiles){
        tiles.push_back(new Tile(*t));
        addToList(tiles.back(),tiles.back()->getId());
    }
    for(Object *o:s.objects){
        objects.push_back(new Object(*o));
        addToList(objects.back(),objects.back()->getId());
    }
    isStopped=true;
}

void State::addToList(Editable* a, unsigned int i){    
    if(i==0){
        list.push_back(a);
        a->setId(list.size());
    }else{
        while(list.size()<i){
            list.push_back(NULL);
        }
        list[i-1]=a;
    }
}

State::~State(){
    clear();
}

void State::loadFile(){
    Image::init();
    Line::init();
    Scripted::Init();
    if(getType()==EMPTYST)
        return;
    MXMLFile *file;
    Tag info;
    switch(name[name.size()-1]){
        case 'l':
            file= new XMLFile(Base::getInstance()->getProj()->getDir(name,Project::MAP).c_str(),info,"maps.dtd");
            break;
        case 'n':
            //file=new BINFile(new Stream(new std::ifstream(Base::getInstance()->getProj()->getDir(name,MAP).c_str())),info,"maps.dtd");
            break;
        default:
            throw "Unhandled type on call to load state\n";
    }
    //TODO: bintype
    //TODO wget file from remote node
    file->read();
    vector<Tag> listElements=info.getChildren();
    for(int i=0;i<listElements.size();i++){
        if(listElements[i].getName().compare("tile")==0){
            addElement(new Tile(listElements[i]));
        }else if(listElements[i].getName().compare("scripted")==0){
            addElement(new Scripted(listElements[i]));
        }else if(listElements[i].getName().compare("camera")==0){
            addElement(new Camera(listElements[i]));
        }
    }
    cout << "Loading: " << glGetError() << endl;
    delete file;
}

Editable* State::getByIndex(unsigned int ind){
    if(!ind)return NULL;
    return list.at(ind-1);
}

StateType State::getType(){
    return EMPTYST;
}

void State::initPhysicsWorld(){
    if(!isStopped){
        cerr << "Cannot add physics to an already started state" << endl;
        return;
    }
    pWorld=new PhysicsWorld;
}

void State::stepPhysWorld(){
    if(pWorld){
        pWorld->stepPhys();
    }
}

void State::updateLoop(){
    while(!isStopped){
        diffTime=tUpdated.getTicks();
        tUpdated.reset();
        
        updateLock->lock();
        updateElements();
        updateLock->unlock();
        
        tMillis sl=std::chrono::milliseconds(int(UPDATE_STEP*1000))- tUpdated.getTicks();
        std::this_thread::sleep_for(sl);
    }
}

void State::lockUpdate(){
    updateLock->lock();
}

void State::unlockUpdate(){
    updateLock->unlock();
}

void State::physicsCallback(btDynamicsWorld *world, btScalar timeStep){
    State *st=Base::getInstance()->getCurState();
    st->diffTime=st->tUpdated.getTicks();
    st->tUpdated.reset();
    if(!st->netNode||!st->netNode->thinClient()){
        btCollisionObjectArray objects = world->getCollisionObjectArray();
        world->clearForces();

        for (int i = 0; i < objects.size(); i++) {
             btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
             if (!rigidBody) {
                  continue;
             }
             rigidBody->applyGravity();
        }
    }
    st->updateElements();
}

void State::gameLoop(){
    if(!isStopped){
        cout << "Trying to start already started state\n";
        return;
    }
    isStopped=false;
    //update thread
    if(pWorld ==NULL){//in case physics world is created it will automatically call update
        updateTh=new thread(&State::updateLoop,this);
    }else{
        pWorld->setCallBack(physicsCallback);//set function to be called in order to update
    }
}

int State::addCam(){
   cameras.push_back(new Camera());
   return cameras.size()-1;
}

void State::setCam(int no){
    if(cameras.size()>no){
        currCam = cameras.at(no);
    }
}

void State::draw(){
#ifndef NODRAW
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    currCam->view();
    
    for(int i=0;i<tiles.size();i++)
        tiles[i]->draw();
    
    for(int i=0;i<objects.size();i++)
        objects[i]->draw();
    Shading::getActive()->setObjMat(Matrix());
    //if(pWorld)pWorld->draw();
#endif
}

void State::updateElements(){
    //Client: send controllers previous to use them to predict
    if(netNode&&!netNode->isServing()){
        netNode->sendUpdate();
    }
    if(currCam)currCam->view();
    if(!netNode||!netNode->thinClient()){
        for(int i=0;i<objects.size();i++){
            objects[i]->update();
        }
        if(netNode){
            for(int i=0;i<list.size();i++){
                list[i]->draw();
            }
        }
    }
    //Server: send state once it gets updated
    if(netNode&&netNode->isServing()){
        netNode->sendUpdate();
    }
    
    if(netNode&&netNode->updated()){
       /**
        * Server reads input from clients
        * Client updates elements
        */
       netNode->updateState();
    }
}

void State::iteration(){
#ifndef NODRAW
    stest->useProgram();
    stest->update();
#endif
}

void State::clear(){
    if(!isStopped){
        cout << "State needs to be stopped in order to clear it\n";
        return;
    }
    for(int i=0;i<tiles.size();i++){
        delete tiles[i];
    }
    tiles.clear();
    for(int i=0;i<objects.size();i++){
        delete objects[i];
    }
    objects.clear();
    for(int i=0;i<cameras.size();i++){
        delete cameras[i];
    }
    cameras.clear();
    list.clear();
}

 unsigned int* State::selection( const int x, const int y ){
#ifndef NODRAW
    ptext->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sselect->useProgram();

    draw();

    ptext->unBind();
    return ptext->ReadId(x,y);
#endif
}

string State::getName(){
    return name;
}

bool State::reusable(){
    return !deleteOnEnd;
}

void State::stop(){
    isStopped=true;
    if(updateTh)
        updateTh->join();
    delete updateTh;
    updateTh = NULL;
    if(!reusable())clear();
    cout << "State " << name << " stopped\n";
}

void State::addElement(Editable *a){
    addToList(a);
    
    switch(a->getType()){
        case E_TILE:
            tiles.push_back(static_cast<Tile*>(a));
            break;
        case E_OBJECT:
            objects.push_back(static_cast<Object*>(a));
            break;
        case E_CAMERA:
            cameras.push_back(static_cast<Camera*>(a));
            break;
    }
}

void State::addRigidBody(btRigidBody *body){
    if(!pWorld){
        cerr<<"Cannot add rigid body to uninitialised physics state" << endl;
        body=NULL;
        return;
    }
    pWorld->addRigidBody(body);
}

void State::deleteRigidBody(btRigidBody *del){
    if(pWorld)pWorld->deleteRigidBody(del);
}

Camera* State::getCam(){
    return currCam;
}

MXML::Tag State::toXML(){
    Tag ret("gameState");
    for(Camera* c:cameras){
        ret.addChildren(c->toXML());
    }
    for(Tile* t:tiles){
        if(t!=NULL)
                ret.addChildren(t->toXML());
    }
    for(Object* o:objects){
        ret.addChildren(o->toXML());
    }
    return ret;
}

void State::fromXML(MXML::Tag &code){
    unsigned int id;
    
    for(Tag &c:code.getChildren()){
        if(c.getName().compare("tile")==0){
            id=c["id"].getAttrib().getInt();
            Tile* t=dynamic_cast<Tile*> (getByIndex(id));
            if(t==NULL){
                cout << "UNINITUALIZED\n";
            }else{
                t->fromXML(c);
            }
        }
        if(c.getName().compare("object")==0){
            id=c["id"].getAttrib().getInt();
            Object* t=dynamic_cast<Object*> (getByIndex(id));
            if(t==NULL){
                cout << "UNINITUALIZED\n";
            }else{
                t->fromXML(c);
            }
        }
        if(c.getName().compare("camera")==0){
            id=c["id"].getAttrib().getInt();
            while(cameras.size()<id){
                cameras.push_back(NULL);
            }
            if(!cameras[id]){
                cameras[id]=new Camera(c);
                continue;
            }
            cameras[id]->fromXML(c);
        }
    }
}

MXML::Tag State::difference(Networkable &n){  
    Tag ret("gameState");
    if(&n == NULL){
        return ret;
    }
    try{
        State& s = dynamic_cast<State&> (n);
        for(int i=0;i<s.tiles.size();i++){
            if(tiles.size()<i){
                ret.addChildren(s.tiles[i]->toXML());
                continue;
            }
            Tag diff=tiles[i]->difference((*s.tiles[i]));
            if(!diff.isEmpty())ret.addChildren(diff);
        }
        for(int i=0;i<s.objects.size();i++){
            if(objects.size()<i){
                ret.addChildren(s.objects[i]->toXML());
            }
            Tag diff=objects[i]->difference(*s.objects[i]);
            if(!diff.isEmpty())ret.addChildren(diff);
        }
        for(int i=0;i<s.cameras.size();i++){
            if(cameras.size()<i){
                ret.addChildren(s.cameras[i]->toXML());
            }
            Tag diff=cameras[i]->difference(*s.cameras[i]);
            if(!diff.isEmpty())ret.addChildren(diff);
        }
    }catch (bad_cast& bc){
        cerr << "bad_cast in state::difference" << bc.what()<< endl;
    }
    return ret;    
}

void State::setNetNode(NetNode *n){
    netNode=n;
}

tMillis State::getDiffTime(){
    return diffTime;
}

void State::changeResolution()
{
  for(Camera* cam:cameras){
    cam->reloadMatrix();
  }
  delete ptext;
  Vector3 res = Base::getInstance()->getResolution();
  ptext = new PickingTexture(res.x,res.y);
}
