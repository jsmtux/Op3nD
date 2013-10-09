#ifndef _HEADERTREEVIEWDELEGATE
#define _HEADERTREEVIEWDELEGATE

#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

class HeaderTreeViewDelegate: public QStyledItemDelegate {
    Q_OBJECT
public:

    explicit HeaderTreeViewDelegate(QAbstractItemView* view, QObject *parent = 0, 
                         const QPixmap &closeIcon = QPixmap())
        : QStyledItemDelegate(parent)
    {
      this->view=view;
    }
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const {
        QStyledItemDelegate::paint(painter, option, index);
	if(index.parent()==QModelIndex()){
	  QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);
	  QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);
	  QRect r = option.rect;
	  QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
	  gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
	  gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
	  gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
	  painter->setBrush(gradientSelected);
	  painter->drawRect(r);
	  
	  //BORDER
	  painter->setPen(lineMarkedPen);
	  painter->drawLine(r.topLeft(),r.topRight());
	  painter->drawLine(r.topRight(),r.bottomRight());
	  painter->drawLine(r.bottomLeft(),r.bottomRight());
	  painter->drawLine(r.topLeft(),r.bottomLeft());
	  
	  painter->setPen(fontMarkedPen);
	  painter->drawText(r.left()+5, r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, index.data(Qt::DisplayRole).toString(), &r);
	}
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
    {
      QSize size = QStyledItemDelegate::sizeHint(option, index);
      if(index.parent()!=QModelIndex()){
	size.setHeight(30);
      }
      return size;
    }
private:
    QAbstractItemView* view;
    Q_DISABLE_COPY(HeaderTreeViewDelegate)
};

#endif
