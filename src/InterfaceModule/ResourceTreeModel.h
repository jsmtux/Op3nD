#ifndef _RESOURCETREEMODEL
#define _RESOURCETREEMODEL
#include <QAbstractItemModel>
#include <QVariant>

#include "../ProjectManagement/Project.h"
#include "TreeItem.h"

class ResourceTreeModel: public QAbstractItemModel{
  Q_OBJECT
public:
  ResourceTreeModel(Project* project);
  virtual ~ResourceTreeModel();
  QVariant data(const QModelIndex &index, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QStringList mimeTypes() const;
  QMimeData* mimeData( const QModelIndexList & indexes ) const;
  bool clearBranch(const QModelIndex &parent=QModelIndex());
public slots:
  void scanDirs();
private:
  Project* project;
  TreeItem* rootItem;
signals:
  void createScript();
  void editScript(QString file);
  void createCamera();
};

#endif
