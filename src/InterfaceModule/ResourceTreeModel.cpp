#include "ResourceTreeModel.h"
#include <QDebug>
#include <QMimeData>
#include <QFile>
#include <QMessageBox>

ResourceTreeModel::ResourceTreeModel(Project* project): QAbstractItemModel()
{
  this->project=project;
  rootItem= new TreeItem("root");
  TreeItem* tmp=new TreeItem("Images");
  tmp->setCallBack([](){
    QMessageBox msg;
    msg.setText("Clicked images button");
    msg.exec();
  });
  tmp->setData(Project::IMAGE);
  rootItem->addItem(tmp);
  tmp=new TreeItem("Meshes");
  tmp->setData(Project::MESH);
  tmp->setCallBack([](){
    QMessageBox msg;
    msg.setText("Clicked meshes button");
    msg.exec();
  });
  rootItem->addItem(tmp);
  tmp=new TreeItem("Objects");
  tmp->setData(Project::OBJECT);
  tmp->setCallBack([=](){
    emit createScript();
  });
  rootItem->addItem(tmp);
  tmp=new TreeItem("cameras");
  tmp->setCallBack([=](){
    emit createCamera();
  });
  rootItem->addItem(tmp);
}

ResourceTreeModel::~ResourceTreeModel()
{
  delete rootItem;
}

QVariant ResourceTreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

  return item->getName();
}

Qt::ItemFlags ResourceTreeModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return 0;

  Qt::ItemFlags flags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  if(index.parent()!=QModelIndex()){
    flags |= Qt::ItemIsDragEnabled;
  }
  return flags;
}

QVariant ResourceTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return "Files";

  return QVariant();
}

QModelIndex ResourceTreeModel::index(int row, int column, const QModelIndex& parent) const
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

QModelIndex ResourceTreeModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int ResourceTreeModel::rowCount(const QModelIndex& parent) const
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

int ResourceTreeModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QStringList ResourceTreeModel::mimeTypes() const
{
  return QStringList() << "application/text.tile";
}

QMimeData* ResourceTreeModel::mimeData(const QModelIndexList& indexes) const
{
  QMimeData *mimeDataPtr = new QMimeData();
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);
  
  foreach (QModelIndex index, indexes) {
    if (index.isValid()) {
      stream << QString::number(static_cast<TreeItem*>(index.parent().internalPointer())->getData().toInt());
      stream << index.data().toString();
    }
  }
  
  mimeDataPtr->setData("application/text.tile", encodedData);
  return mimeDataPtr;
}

bool ResourceTreeModel::clearBranch(const QModelIndex& parent)
{
  if(parent==QModelIndex())
    return false;
  
  TreeItem *item=static_cast<TreeItem*>(parent.internalPointer());
  beginRemoveRows(parent,0,item->childCount());
  item->clear();
  endRemoveRows();
  return true;
}

void ResourceTreeModel::scanDirs()
{
  vector<string> imFiles=project->listFiles(Project::IMAGE);
  
  clearBranch(index(0,0).child(0,0));
  for(string file:imFiles){
    rootItem->child(0)->addItem(new TreeItem(QString::fromStdString(file),rootItem,QString::fromStdString(project->getDir(file,Project::IMAGE))));
  }
  vector<string> meshFiles=project->listFiles(Project::MESH);
  clearBranch(index(0,0).child(1,0));
  rootItem->child(1)->clear();
  for(string file:meshFiles){
    rootItem->child(1)->addItem(new TreeItem(QString::fromStdString(file)));
  }
  vector<string> scriptFiles=project->listFiles(Project::OBJECT);
  clearBranch(index(0,0).child(2,0));
  rootItem->child(2)->clear();
  for(string file:scriptFiles){
    TreeItem* scriptItem=new TreeItem(QString::fromStdString(file));
    QMenu* scriptMenu=new QMenu();
    QAction *editScriptAction=new QAction(QString("editScript"),this);
    connect(editScriptAction,&QAction::triggered,[=](){emit editScript(QString::fromStdString(file));});
    scriptMenu->addAction(editScriptAction);
    scriptMenu->addAction("rename");
    scriptItem->setContextMenu(scriptMenu);
    rootItem->child(2)->addItem(scriptItem);
  }
}