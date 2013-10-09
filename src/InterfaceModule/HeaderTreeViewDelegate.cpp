#include "HeaderTreeViewDelegate.h"
/*#include <QPainter>
#include <QStyle>
#include <QAbstractItemView>
#include <QStylePainter>

HeaderTreeViewDelegate::HeaderTreeViewDelegate(QAbstractItemView* view,QObject* parent): QStyledItemDelegate(parent)
{
  this->view = view;
}

void HeaderTreeViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if(index.column()==0){
    QStylePainter style(view);
    style.drawControl(QStyle::CE_PushButton,option);
  }else{
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QSize HeaderTreeViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  return QStyledItemDelegate::sizeHint(option, index);
}
*/