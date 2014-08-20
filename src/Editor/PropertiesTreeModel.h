#ifndef _PROPERTIESTREEMODEL
#define _PROPERTIESTREEMODEL
#include <QAbstractItemModel>
#include <QTreeView>
#include "../Engine/ObjectTypes/Editable.h"
#include "TreeItem.h"

class PropertiesTreeModel:public QAbstractItemModel{
  Q_OBJECT
public:
  PropertiesTreeModel(Editable* editabe);
  virtual ~PropertiesTreeModel();
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role= Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
private:
  void fillTileProperties(TreeItem* tileItem);
  void addResourceProperties(TreeItem* resItem);
  void fillObjectProperties(TreeItem* objectItem);
  void setAnimationProperties(TreeItem* animationItem);
  Editable* editable;
  TreeItem* rootItem;
signals:
  void editShape(string resource);
};

#endif