#ifndef _TREEITEM
#define _TREEITEM
#include <QList>
#include <QString>
#include <QPixmap>
#include <QVariant>
#include <functional>
#include <signal.h>
#include <QObject>
#include "../Engine/Math/Vector3.h"
#include "../Engine/Math/Quaternion.h"

class TreeItem: public QObject{
  Q_OBJECT
public:
  TreeItem(QString name="", TreeItem* parent=NULL, QString path="");
  TreeItem(QString name, TreeItem* parent, Vector3 vector, function<void (Vector3)> func=[](Vector3){});
  TreeItem(QString name, TreeItem* parent, Quaternion quaternion, function<void (Quaternion)> func=[](Quaternion){});
  TreeItem(QString name, TreeItem* parent, QString actionName, function<void ()> func);
  ~TreeItem();
  void addItem(TreeItem *item);
  TreeItem* child(int row);
  int childCount();
  TreeItem* parent();
  int row() const;
  QString getName();
  void setParent(TreeItem* parent);
  QPixmap* getPixmap();
  void setData(QVariant d);
  QVariant getData() const;
  bool isButton();
  function<void ()> getCallBack();
public slots:
  void updateSource();
private:
  enum itemType{NONE,VECTOR,QUATERNION,BUTTON};
  TreeItem* parentItem;
  QList<TreeItem*> children;
  QString name;
  QPixmap* image;
  QVariant data;
  itemType type;
  function< void (Vector3)> vectorFunc;
  function< void (Quaternion)> quaternionFunc;
  function< void ()> buttonFunc;
signals:
  void valueChanged();
};

#endif