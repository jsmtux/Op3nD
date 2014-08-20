#include "HeaderTreeViewDelegate.h"
#include <QPalette>
#include <QDebug>

QRect HeaderTreeViewDelegate::buttonArea;

HeaderTreeViewDelegate::HeaderTreeViewDelegate(QAbstractItemView* view, QObject *parent, 
                         const QPixmap &closeIcon): QStyledItemDelegate(parent)
{
  this->view=view;
  moreButton.load(":/images/button.png");
  moreButton=moreButton.scaledToHeight(15);
}

void HeaderTreeViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
	    const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
    if(index.parent()==QModelIndex()){
      QColor baseColor=QPalette().color(QPalette::Active,QPalette::Highlight);
      QPen lineMarkedPen(baseColor.darker(400), 1, Qt::SolidLine);
      QPen fontMarkedPen(QPalette().color(QPalette::Active,QPalette::HighlightedText), 1, Qt::SolidLine);
      QRect r = option.rect;
      QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
      gradientSelected.setColorAt(0.0, baseColor.lighter());
      gradientSelected.setColorAt(0.8, baseColor);
      gradientSelected.setColorAt(1.0, baseColor.darker());
      painter->setBrush(gradientSelected);
      painter->drawRect(r);
      
      //BORDER
      painter->setPen(lineMarkedPen);
      painter->drawLine(r.topLeft(),r.topRight());
      painter->drawLine(r.topRight(),r.bottomRight());
      painter->drawLine(r.bottomLeft(),r.bottomRight());
      painter->drawLine(r.topLeft(),r.bottomLeft());
      
      //BUTTON
      painter->drawPixmap(QPointF( r.right()-moreButton.width()-5, r.top()+2), moreButton);
      buttonArea=QRect(r.right()-moreButton.width()-5, r.top()+2,moreButton.size().width(),moreButton.size().height());
      
      painter->setPen(fontMarkedPen);
      painter->drawText(r.left()+5, r.top(), r.width(), r.height(), 
			Qt::AlignBottom|Qt::AlignLeft, index.data(Qt::DisplayRole).toString(), &r);
    }
}

QSize HeaderTreeViewDelegate::sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
  QSize size = QStyledItemDelegate::sizeHint(option, index);
  if(index.parent()!=QModelIndex()){
    size.setHeight(35);
  }
  return size;
}

bool HeaderTreeViewDelegate::isButton(QPoint position)
{
  return position.x()>=buttonArea.x()&&position.x()<=buttonArea.x()+buttonArea.width();
}
