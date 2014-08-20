#include "ImageTreeView.h"
#include "HeaderTreeViewDelegate.h"
#include "TreeItem.h"
#include <QPainter>
#include <QPushButton>
#include <QMenu>
#include <QDebug>

ImageTreeView::ImageTreeView(QWidget* parent): QTreeView(parent)
{
  QAbstractItemDelegate* delegate = new HeaderTreeViewDelegate(this);
  setItemDelegate(delegate);
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
    this, SLOT(showContextMenu(const QPoint&)));
}

ImageTreeView::~ImageTreeView()
{

}

void ImageTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index) const
{
  QTreeView::drawRow( painter, options, index );
  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
  
  if(item->getPixmap()!=NULL){
    painter->drawPixmap(QPointF( options.rect.topLeft().x()+10, options.rect.topRight().y()), *item->getPixmap());
  }
}

void ImageTreeView::mousePressEvent(QMouseEvent* event)
{
  if( event->buttons() == Qt::LeftButton )
  {
    if(indexAt(event->pos()).parent()==QModelIndex()){
      if(HeaderTreeViewDelegate::isButton(event->pos())){
	TreeItem* item = static_cast<TreeItem*>(indexAt(event->pos()).internalPointer());
	item->getCallBack()();
	return;
      }
    }
  }
  QTreeView::mousePressEvent(event);
}

void ImageTreeView::showContextMenu(QPoint point)
{
  TreeItem* currentItem=static_cast<TreeItem*>(currentIndex().internalPointer());
  QMenu* menu=currentItem->getContextMenu();
  if(menu!=nullptr){
    menu->exec(QCursor::pos());
  }
}
