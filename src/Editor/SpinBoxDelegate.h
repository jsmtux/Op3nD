#ifndef _HEADERTREEVIEWDELEGATE
#define _HEADERTREEVIEWDELEGATE

#include <QItemDelegate>
#include <QAbstractItemView>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>

class SpinBoxDelegate: public QItemDelegate {
  Q_OBJECT
public:
  
  SpinBoxDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
			const QModelIndex &index) const;

  void paint(QPainter *painter,
		      const QStyleOptionViewItem &option,
		      const QModelIndex &index) const ;
		      
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
		    const QModelIndex &index) const;

  void updateEditorGeometry(QWidget *editor,
			    const QStyleOptionViewItem &option, const QModelIndex &index) const;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, 
                           const QStyleOptionViewItem &option, 
                           const QModelIndex &index);
  QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const ;
private:
  QStyle::State _state;
};

#endif
