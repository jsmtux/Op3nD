#include "PhysicsDialog.h"
#include "../src/Engine/Base.h"
#include "../src/Engine/ObjectTypes/Resource.h"
#include </home/jsmtux/projects/Op3nD/src/Engine/PhysicsWorld.h>
#include </home/jsmtux/projects/Op3nD/src/Engine/States/MeshState.h>
#include </home/jsmtux/projects/Op3nD/src/ProjectManagement/Project.h>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

PhysicsDialog::PhysicsDialog(QWidget* parent): QDialog(parent)
{
  physicsDialog.setupUi(this);
  QMenu* menu= new QMenu;
  QAction* action=new QAction("Sphere",this);
  action->setData(S_SPHERE);
  menu->addAction(action);
  action= new QAction("Box",this);
  action->setData(S_BOX);
  menu->addAction(action);
  
  connect(menu,SIGNAL(triggered(QAction*)),SLOT(addElementMenu(QAction*)));
  connect(physicsDialog.twShapes,SIGNAL(cellClicked(int,int)),this,SLOT(selectShape(int)));
  connect(physicsDialog.pbDeleteShape,SIGNAL(clicked(bool)),SLOT(deleteShape()));
  
  connect(physicsDialog.sbBoxSizeX,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbBoxSizeY,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbBoxSizeZ,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbBoxOffX,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbBoxOffY,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbBoxOffZ,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  
  connect(physicsDialog.sbShpereSize,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbSphereOffX,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbSphereOffY,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  connect(physicsDialog.sbSphereOffZ,SIGNAL(valueChanged(double)),SLOT(updateShape()));
  
  physicsDialog.tbAddShape->setMenu(menu);
  physicsDialog.twShapes->setColumnCount(2);
  physicsDialog.twShapes->setHorizontalHeaderLabels(QStringList()={"Id","Type"});
}

void PhysicsDialog::accept()
{
  if(shapes.size()==0){
    QMessageBox msg;
    msg.setText(tr("Please add at least an element"));
    msg.exec();
    return;
  }else{
    Physical phys;
    phys.setMass(physicsDialog.sbWeigh->value());
    phys.setRestitution(physicsDialog.sbRestitution->value());
    phys.setFriction(physicsDialog.sbFriction->value());
    phys.setAngularFriction(physicsDialog.sbAngularFriction->value());
    for(int i=0;i<physicsDialog.twShapes->rowCount();i++){
      phys.addShape(shapes[i].size,shapes[i].offset,shapes[i].shapeType);
    }
    Tag root=phys.toXML();
    XMLFile out(Base::getInstance()->getProj()->getDir(toEdit+".xml",Project::MESH),root,"physical.dtd");
    out.write();
  }
  clear();
  restoreCanvas();
  QDialog::accept();
}

void PhysicsDialog::exec(string resource)
{
  toEdit=resource;
  StateManager* stateManager= Base::getInstance()->getStateManager();
  State* meshState= new MeshState("meshEditor");
  Tile* mesh=new Tile(Vector3(0,0,0),Vector3(1,1,1),Quaternion(0,0,0,1),resource);
  mesh->setPhysical(NULL);
  meshState->addElement(mesh);
  stateManager->newState(meshState);
  Tag root;
  XMLFile in(Base::getInstance()->getProj()->getDir(toEdit+".xml",Project::MESH),root,"physical.dtd");
  in.read();
  
  Physical phys;
  phys.fromXML(root);
  physicsDialog.sbWeigh->setValue(phys.getMass());
  physicsDialog.sbRestitution->setValue(phys.getRestitution());
  physicsDialog.sbFriction->setValue(phys.getFriction());
  physicsDialog.sbAngularFriction->setValue(phys.getAngularFriction());

  
  for(Tag t:root.getChildren()){
    if(t.getName().compare("box")==0){
      Vector3 size(t["size"]);
      btRigidBody *body=new btRigidBody(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))),
					new btBoxShape(size));
      Base::getInstance()->getStateManager()->getCurState()->addRigidBody(body);
      shapes.push_back(shapeInfo{getNewId(),body,S_BOX,Vector3::zero,size});
    }else if(t.getName().compare("sphere")==0){
      float size=Vector3(t["size"]).x;
      btRigidBody *body=new btRigidBody(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))),
					new btSphereShape(size));
      Base::getInstance()->getStateManager()->getCurState()->addRigidBody(body);
      shapes.push_back(shapeInfo{getNewId(),body,S_SPHERE,Vector3::zero,Vector3(size,0,0)});
    }
  }
  updateShapesList();
  QDialog::exec();
}

void PhysicsDialog::closeEvent(QCloseEvent* ev)
{
  restoreCanvas();
  QDialog::closeEvent(ev);
}

void PhysicsDialog::addElementMenu(QAction* action)
{
  State* current = Base::getInstance()->getStateManager()->getCurState();
  btRigidBody* rigidBody;
  switch(action->data().toInt()){
    case S_SPHERE:
      rigidBody=new btRigidBody(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))),new btSphereShape(1));
      break;
    case S_BOX:
      rigidBody=new btRigidBody(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))),new btBoxShape(btVector3(1,1,1)));
  }
  shapes.append(shapeInfo{getNewId(),rigidBody,tShape(action->data().toInt()),Vector3(1,1,1),Vector3(0,0,0)});
  current->addRigidBody(rigidBody);
  updateShapesList();
}

void PhysicsDialog::updateShapesList()
{
  physicsDialog.twShapes->setRowCount(shapes.count());
  for(int i=0;i<shapes.count();i++){
    physicsDialog.twShapes->setItem(i,0,new QTableWidgetItem(QString::number(shapes[i].id)));
    switch(shapes[i].shapeType){
      case S_SPHERE:
	physicsDialog.twShapes->setItem(i,1,new QTableWidgetItem("Sphere"));
	break;
      case S_BOX:
	physicsDialog.twShapes->setItem(i,1,new QTableWidgetItem("Box"));
	break;
      default:
	;
    }
  }
}

void PhysicsDialog::selectShape(int row)
{
  switch(shapes[row].shapeType){
    case S_SPHERE:
      physicsDialog.swShapeProp->setCurrentIndex(0);
      physicsDialog.sbShpereSize->setValue(shapes[row].size.x);
      physicsDialog.sbSphereOffX->setValue(shapes[row].offset.x);
      physicsDialog.sbSphereOffY->setValue(shapes[row].offset.y);
      physicsDialog.sbSphereOffZ->setValue(shapes[row].offset.z);
      break;
    case S_BOX:
      physicsDialog.swShapeProp->setCurrentIndex(1);
      physicsDialog.sbBoxSizeX->setValue(shapes[row].size.x);
      physicsDialog.sbBoxSizeY->setValue(shapes[row].size.y);
      physicsDialog.sbBoxSizeZ->setValue(shapes[row].size.z);
      physicsDialog.sbBoxOffX->setValue(shapes[row].offset.x);
      physicsDialog.sbBoxOffY->setValue(shapes[row].offset.y);
      physicsDialog.sbBoxOffZ->setValue(shapes[row].offset.z);
      break;
    default:
      physicsDialog.swShapeProp->setCurrentIndex(2);
  }
}

void PhysicsDialog::deleteShape()
{
  int row= physicsDialog.twShapes->currentRow();
  State* current = Base::getInstance()->getStateManager()->getCurState();
  current->deleteRigidBody(shapes[row].rigidBody);
  shapes.removeAt(row);
  updateShapesList();
}

int PhysicsDialog::getNewId()
{
  int i=0;
  bool used=true;
  while(used){
    used=false;
    for(shapeInfo shape:shapes){
      if(shape.id==i){
	used=true;
	i++;
	break;
      }
    }
  }
  return i;
}

void PhysicsDialog::updateShape()
{
  int row= physicsDialog.twShapes->currentRow();
  btRigidBody* current = shapes[row].rigidBody;
  btVector3 offset;
  switch(shapes[row].shapeType){
    case S_SPHERE:
	current->setCollisionShape(new btSphereShape(physicsDialog.sbShpereSize->value()));
	offset.setX(physicsDialog.sbSphereOffX->value());
	offset.setY(physicsDialog.sbSphereOffY->value());
	offset.setZ(physicsDialog.sbSphereOffZ->value());
      break;
    case S_BOX:
	current->setCollisionShape(new btBoxShape(btVector3(physicsDialog.sbBoxSizeX->value(),
							physicsDialog.sbBoxSizeY->value(),
							physicsDialog.sbBoxSizeZ->value())));
	offset.setX(physicsDialog.sbBoxOffX->value());
	offset.setY(physicsDialog.sbBoxOffY->value());
	offset.setZ(physicsDialog.sbBoxOffZ->value());
      break;
    default:
      break;
  }
  current->setMotionState(new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), offset)));
}

void PhysicsDialog::restoreCanvas(){  
  StateManager * stateManager= Base::getInstance()->getStateManager();
  stateManager->deleteState();
}

void PhysicsDialog::clear()
{
  for(shapeInfo s:shapes){
    Base::getInstance()->getStateManager()->getCurState()->getPhysicsWorld()->deleteRigidBody(s.rigidBody);
  }
  shapes.clear();
}
