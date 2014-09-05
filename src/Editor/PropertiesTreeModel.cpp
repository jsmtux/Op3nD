#include "PropertiesTreeModel.h"
#include "Resources/Resource.h"
#include "../Engine/ObjectTypes/Tile.h"
#include "../Engine/ObjectTypes/Scripted.h"
#include "Resources/Image.h"
#include "Resources/Model3d.h"
#include <QDebug>

PropertiesTreeModel::PropertiesTreeModel(Editable* editabe): QAbstractItemModel()
{
  this->editable=editabe;
  if(!editabe){
    return;
  }
  rootItem= new TreeItem("root");
  TreeItem* tmp;
  switch(editabe->getType()){
    case E_OBJECT:
      tmp= new TreeItem("Object properties");
      rootItem->addItem(tmp);
      fillObjectProperties(tmp);
    case E_TILE:
      tmp=new TreeItem("Tile properties");
      rootItem->addItem(tmp);
      fillTileProperties(tmp);
      addResourceProperties(rootItem);
      break;
    case E_CAMERA:
      break;
  }
}

PropertiesTreeModel::~PropertiesTreeModel()
{
  delete rootItem;
}

QVariant PropertiesTreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  if(index.column()==0){
    return item->getName();
  }else{
    return item->getData();
  }
}

bool PropertiesTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if(role==Qt::EditRole){
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    item->setData(value);
  }
  return true;
}

Qt::ItemFlags PropertiesTreeModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return 0;
  if(index.column()==0){
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }else{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
  }
}

QVariant PropertiesTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
    if(section==0)
      return tr("Value");
    return tr("Property");
  }

  return QVariant();
}

QModelIndex PropertiesTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  TreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex PropertiesTreeModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int PropertiesTreeModel::rowCount(const QModelIndex& parent) const
{
  TreeItem *parentItem;
  if (parent.column() > 0)
      return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

int PropertiesTreeModel::columnCount(const QModelIndex& parent) const
{
  if(parent.internalPointer()==rootItem){
    return 1;
  }
  return 2;
}

void PropertiesTreeModel::fillObjectProperties(TreeItem* objectItem)
{
  Scripted* element=dynamic_cast<Scripted*>(editable);
  objectItem->addItem(new TreeItem("Script",objectItem,QString::fromStdString(element->getName())));
}

void PropertiesTreeModel::fillTileProperties(TreeItem* tileItem)
{
  tileItem->addItem(new TreeItem(tr("Position"),tileItem,editable->getPos(),[=](Vector3 vec){editable->setPos(vec);}));
  tileItem->addItem(new TreeItem(tr("Size"),tileItem,editable->getSize(),[=](Vector3 vec){editable->setSize(vec);}));
  tileItem->addItem(new TreeItem(tr("Rotation"),tileItem,editable->getRot(),[=](Quaternion q){editable->setRot(q);}));
  tileItem->addItem(new TreeItem(tr("Shape"),tileItem,"edit",[&](){emit editShape(dynamic_cast<Tile*>(editable)->getResource()->getName());}));
  TreeItem* animationItem=new TreeItem(tr("Animation"),tileItem);
  tileItem->addItem(animationItem);
  setAnimationProperties(animationItem);
}

void PropertiesTreeModel::addResourceProperties(TreeItem* resItem)
{
  Resource* res=dynamic_cast<Tile*>(editable)->getResource();
  TreeItem* tmp, *tmpChild;
  switch(res->getType()){
    case Resource::BOX:
      break;
    case Resource::IMAGE:
      tmp = new TreeItem("Image properties");
      resItem->addItem(tmp);
      tmpChild = new TreeItem("Path");
      tmpChild->setData(QString::fromStdString(dynamic_cast<Image*>(res)->getName()));
      tmp->addItem(tmpChild);
      break;
    case Resource::LIGHT:
      break;
    case Resource::LINE:
      break;
    case Resource::MODEL3D:
      tmp = new TreeItem("Mesh properties");
      resItem->addItem(tmp);
      tmpChild = new TreeItem("Path");
      tmpChild->setData(QString::fromStdString(dynamic_cast<Model3d*>(res)->getName()));
      tmp->addItem(tmpChild);
      break;
    case Resource::FONT:
      break;
  }
}

void PropertiesTreeModel::setAnimationProperties(TreeItem* animationItem)
{
  Resource* res=dynamic_cast<Tile*>(editable)->getResource();
  Model3d* model= dynamic_cast<Model3d*>(res);
}
