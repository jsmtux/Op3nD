#include "PhysicsDialog.h"
#include "../src/Engine/Base.h"
#include "../src/Engine/ObjectTypes/Resource.h"
#include <QMenu>
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
  
  physicsDialog.tbAddShape->setMenu(menu);
  physicsDialog.twShapes->setColumnCount(2);
  physicsDialog.twShapes->setHorizontalHeaderLabels(QStringList()={"Id","Type"});
  mesh=NULL;
}

void PhysicsDialog::exec(Editable* editable)
{
  prevState= Base::getInstance()->getCurState()->getName().c_str();
  Base::getInstance()->newState("meshEditor",MESHST);
  Base::getInstance()->changeState("meshEditor",MESHST);
  Resource* res= dynamic_cast<Tile*>(editable)->getResource();
  Tile* mesh=new Tile(Vector3(0,0,0),Vector3(1,1,1),Quaternion(0,0,0,1),res->getName());
  mesh->setPhysical(NULL);
  Base::getInstance()->getCurState()->addElement(mesh);
  Base::getInstance()->beginState();
  QDialog::exec();
}

void PhysicsDialog::closeEvent(QCloseEvent* ev)
{
  Base::getInstance()->getCurState()->stop();
  Base::getInstance()->getCurState()->clear();
  Base::getInstance()->changeState(prevState.toStdString(),EDITORST);
  Base::getInstance()->beginState();
  QDialog::closeEvent(ev);
}

void PhysicsDialog::addElementMenu(QAction* action)
{
  State* current = Base::getInstance()->getCurState();
  btRigidBody* rigidBody;
  switch(action->data().toInt()){
    case S_SPHERE:
      rigidBody=new btRigidBody(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))),new btSphereShape(1));
      break;
    case S_BOX:
      rigidBody=new btRigidBody(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))),new btBoxShape(btVector3(1,1,1)));
  }
  shapes.append(shapeInfo{numShapes++,rigidBody,tShape(action->data().toInt())});
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
