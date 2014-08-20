#ifndef _IMAGETREEVIEW
#define _IMAGETREEVIEW
#include <QTreeView>

class ImageTreeView: public QTreeView{
  Q_OBJECT
public:
  ImageTreeView( QWidget *parent = 0 );
  ~ImageTreeView();
public slots:
  void showContextMenu(QPoint point);
protected:
  void drawRow( QPainter *painter, const QStyleOptionViewItem &options, 
		  const QModelIndex &index ) const;
  void mousePressEvent(QMouseEvent* event);
};

#endif
