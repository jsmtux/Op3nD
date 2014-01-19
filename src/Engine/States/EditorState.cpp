#include "EditorState.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
#include "../ObjectTypes/Tile.h"
#include "../Base.h"
#include "../ObjectTypes/Camera.h"
#include "../../ProjectManagement/Project.h"

EditorState::EditorState(string n):State(n){
  selected=NULL;
  handler = new Tile(Vector3(5,0,0),Vector3(1,1,1),Quaternion(0.,0.,0.,1.),"untitled.3ds");
  //initPhysicsWorld();
  cameras.push_back(new Camera());
  currCam=cameras[0];
  currCam->setPosition(Vector3(0,0,-100));
}


#include "../Graphics/Shading.h"

int prevSel=-1;
void EditorState::iteration(){
  State::iteration();    
  draw();
}

void EditorState::draw(){
  #ifndef NODRAW
  State::draw();
  if(selected!=NULL){
    glClear(GL_DEPTH_BUFFER_BIT);
    handler->setPos(selected->getPos());
    handler->draw();
  }
  for(int i=0;i<cameras.size();i++){
    if(cameras[i]!=currCam)
      cameras[i]->draw();        
  }
  #endif
}

void EditorState::saveState(){
  MXML::Tag root("scene");
  for(int i=0;i<tiles.size();i++){
    root.addChildren(tiles[i]->toXML());        
  }
  for(int i=0;i<objects.size();i++)
    root.addChildren(objects[i]->toXML());
  for(int i=1;i<cameras.size();i++)//1st camera is editor's
    root.addChildren(cameras[i]->toXML());
  MXML::XMLFile f(Base::getInstance()->getProj()->getDir(name,Project::MAP).c_str(),root,"maps.dtd");
  f.write();
  //MXML::BINFile f2(Base::getInstance()->getProj()->getDir(name+".bin",MAP).c_str(),"maps.dtd",root);
  //f2.write();
}

void EditorState::updateElements(){
  if(!Base::getInstance()->getControllers().size()){
    cerr << "No controller configured for editor state\n";
  }
  
  Controller* cont1 = Base::getInstance()->getControllers()[0];
  
  currCam->orientate(Vector3(cont1->getAxis()[A_Y],
			    cont1->getAxis()[A_X],0));
  if(cont1->getKeys()[K_UP]){
    currCam->move(Vector3(10,0,0));
  }
  if(cont1->getKeys()[K_DOWN])
    currCam->move(Vector3(-10,0,0));
  if(cont1->getKeys()[K_LEFT])
    currCam->move(Vector3(0,10,0));
  if(cont1->getKeys()[K_RIGHT])
    currCam->move(Vector3(0,-10,0));
  
  int tmp=-1;
  bool handled=false;
  
  bool changed =false;
  if((tmp=cont1->getSelected()[0])!=prevSel){
    changed =true;
    if(tmp==handler->getId()){
      handled=true;
    }else{
      prevSel=tmp;
      selected=getByIndex(tmp);
    }
  }
  if(!netNode||!netNode->thinClient()){
    if(selected!=NULL){      
      Vector3 movement;
      if(!handled){
	movement=currCam->getU()*cont1->getAxis()[A_L]/10;
	movement=movement-currCam->getV()*cont1->getAxis()[A_R]/10;
	
	Editable *sel=static_cast<Editable*>(selected);
	if(cont1->getKeys()[K_A]){
	  if(cont1->getAxis()[A_L]!=0||cont1->getAxis()[A_R]!=0){
	    changed =true;
	  }
	  sel->setPos(sel->getPos()+movement);                    
	}
      }else{            
	switch(cont1->getSelected()[1]){
	  case 0:
	    movement=Vector3(1,0,0);
	    break;
	  case 1:
	    movement=Vector3(0,1,0);
	    break;
	  case 2:
	    movement=Vector3(0,0,1);
	    break;
	}
	float amount=cont1->getAxis()[A_L]-cont1->getAxis()[A_R];
	amount/=10;            
	Editable *sel=static_cast<Editable*>(selected);            
	if(cont1->getKeys()[K_A]){
	  sel->setPos(sel->getPos()+movement*amount);
	}
      }
    }
  }
}