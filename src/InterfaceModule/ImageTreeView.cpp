#include "ImageTreeView.h"
#include "HeaderTreeViewDelegate.h"
#include "TreeItem.h"
#include <QPainter>
#include <QPushButton>

ImageTreeView::ImageTreeView(QWidget* parent): QTreeView(parent)
{
  setItemDelegate(new HeaderTreeViewDelegate(this));
}

ImageTreeView::~ImageTreeView()
{

}

void ImageTreeView::drawRow(QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index) const
{
  QTreeView::drawRow( painter, options, index );
  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
  
  if(item->getPixmap()!=NULL){
    QPixmap pixmap;
      
    pixmap.load("blue.bmp");
    pixmap.scaledToHeight(options.rect.height());
    pixmap.scaledToWidth(15);
    painter->drawPixmap(QPointF( options.rect.topLeft().x()+10, options.rect.topRight().y()), *item->getPixmap());
  }
}