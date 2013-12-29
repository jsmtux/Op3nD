#include "ResourceTreeModel.h"
#include <QDebug>

ResourceTreeModel::ResourceTreeModel(Project* project): QAbstractItemModel()
{
  this->project=project;
  rootItem= new TreeItem("root");
  rootItem->addItem(new TreeItem("Images"));
  rootItem->addItem(new TreeItem("Meshes"));
  rootItem->addItem(new TreeItem("Objects"));
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

void ResourceTreeModel::scanDirs()
{
  vector<string> imFiles=project->listFiles(Project::IMAGE);
  for(string file:imFiles){
    rootItem->child(0)->addItem(new TreeItem(QString::fromStdString(file),rootItem,QString::fromStdString(project->getDir(file,Project::IMAGE))));
  }
  vector<string> meshFiles=project->listFiles(Project::MESH);
  for(string file:meshFiles){
    rootItem->child(1)->addItem(new TreeItem(QString::fromStdString(file)));
  }
}