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

    HeaderTreeViewDelegate(QAbstractItemView* view, QObject *parent = 0, 
                         const QPixmap &closeIcon = QPixmap());
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
private:
    QAbstractItemView* view;
    Q_DISABLE_COPY(HeaderTreeViewDelegate)
};

#endif
