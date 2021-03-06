#include "TreeItem.h"
#include <QDebug>

TreeItem::TreeItem(QString name, TreeItem* parent, QString path)
{
  parentItem=parent;
  this->name=name;
  if(!path.isEmpty()){
    QPixmap tmpImage;
    tmpImage.load(path);
    if(tmpImage.width()>tmpImage.height()){
      image = new QPixmap(tmpImage.scaledToWidth(30));
    }else{
      image = new QPixmap(tmpImage.scaledToHeight(30));
    }
  }else{
    image = NULL;
  }
  vectorFunc=[](Vector3){};
  quaternionFunc=[](Quaternion){};
  type=NONE;
}

TreeItem::TreeItem(QString name, TreeItem* parent, Vector3 vector, function< void (Vector3)> func)
{
  image=NULL;
  parentItem=parent;
  this->name=name;
  TreeItem *t= new TreeItem("x",this);
  t->setData((double)vector.x);
  connect(t,&TreeItem::valueChanged,this,&TreeItem::updateSource);
  addItem(t);
  t= new TreeItem("y",this);
  t->setData((double)vector.y);
  connect(t,&TreeItem::valueChanged,this,&TreeItem::updateSource);
  addItem(t);
  t= new TreeItem("z",this);
  t->setData((double)vector.z);
  connect(t,&TreeItem::valueChanged,this,&TreeItem::updateSource);
  addItem(t);
  type=VECTOR;
  vectorFunc=func;
}

TreeItem::TreeItem(QString name, TreeItem* parent, Quaternion quaternion, function<void (Quaternion)> func)
{
  image=NULL;
  parentItem=parent;
  this->name=name;
  Vector3 angles= quaternion.toEuler();
  TreeItem *t= new TreeItem("x",this);
  t->setData((double)angles.x);
  connect(t,&TreeItem::valueChanged,this,&TreeItem::updateSource);
  addItem(t);
  t= new TreeItem("y",this);
  t->setData((double)angles.y);
  connect(t,&TreeItem::valueChanged,this,&TreeItem::updateSource);
  addItem(t);
  t= new TreeItem("z",this);
  t->setData((double)angles.z);
  connect(t,&TreeItem::valueChanged,this,&TreeItem::updateSource);
  addItem(t);
  type=QUATERNION;
  quaternionFunc=func;
}

TreeItem::TreeItem(QString name, TreeItem* parent, QString actionName, function< void ()> func)
{
  image=nullptr;
  parentItem=parent;
  this->name=name;
  data=actionName;
  buttonFunc=func;
  type=BUTTON;
}

void TreeItem::updateSource()
{
  if(type==VECTOR){
    Vector3 current;
    current.x=children[0]->data.toDouble();
    current.y=children[1]->data.toDouble();
    current.z=children[2]->data.toDouble();
    vectorFunc(current);
  }
  if(type==QUATERNION){
    Vector3 current;
    current.x=children[0]->data.toDouble();
    current.y=children[1]->data.toDouble();
    current.z=children[2]->data.toDouble();
    quaternionFunc(Quaternion(current));
  }
}

void TreeItem::setContextMenu(QMenu* menu)
{
  contextMenu=menu;
}

QMenu* TreeItem::getContextMenu()
{
  return contextMenu;
}

TreeItem::~TreeItem()
{
  qDeleteAll(children);
  if(image){
    delete image;
  }
  if(contextMenu){
    delete contextMenu;
  }
}

void TreeItem::addItem(TreeItem *item){
  item->setParent(this);
  children.append(item);
}

TreeItem* TreeItem::child(int row){
  return children.at(row);
}

int TreeItem::childCount(){
  return children.count();
}

TreeItem* TreeItem::parent(){
  return parentItem;
}

int TreeItem::row() const
{
  if (!parentItem)
    return 0;
  parentItem->children.indexOf(const_cast<TreeItem*>(this));
}

QString TreeItem::getName(){
  return name;
}

void TreeItem::setParent(TreeItem* parent){
  parentItem=parent;
}

QPixmap* TreeItem::getPixmap(){
  return image;
}

void TreeItem::setData(QVariant d){
  data = d;
}

QVariant TreeItem::getData() const{
  return data;
}

bool TreeItem::isButton()
{
  return type==BUTTON;
}

function< void ()> TreeItem::getCallBack()
{
  return buttonFunc;
}

void TreeItem::setCallBack(function< void ()> func)
{
  buttonFunc=func;
}

void TreeItem::clear()
{
  qDeleteAll(children);
  children.clear();
}
