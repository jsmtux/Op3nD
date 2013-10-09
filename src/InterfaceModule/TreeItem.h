#ifndef _TREEITEM
#define _TREEITEM
#include <QList>
#include <QString>
#include <QPixmap>

class TreeItem{
public:
  TreeItem(QString name="", TreeItem* parent=NULL, QString path=""){
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
  }
  ~TreeItem(){
    qDeleteAll(children);
    delete image;
  }
  void addItem(TreeItem *item){
    item->setParent(this);
    children.append(item);
  }
  TreeItem* child(int row){
    return children.at(row);
  }
  int childCount(){
    return children.count();
  }
  TreeItem* parent(){
    return parentItem;
  }
  int row() const
  {
    if (!parentItem)
	  return 0;
    parentItem->children.indexOf(const_cast<TreeItem*>(this));
  }
  QString getName(){
    return name;
  }
  void setParent(TreeItem* parent){
    parentItem=parent;
  }
  QPixmap* getPixmap(){
    return image;
  }
private:
  TreeItem* parentItem;
  QList<TreeItem*> children;
  QString name;
  QPixmap* image;
};

#endif