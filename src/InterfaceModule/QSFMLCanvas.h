#ifndef _QSFMLCANVAS
#define _QSFMLCANVAS

#include <QWidget>
#include <QTimer>
#include <QMimeData>
#include <SFML/Graphics.hpp>
#include <stack>
#include "../Engine/Graphics/RenderingContext.h"
#include "../Engine/Controller.h"
#include "../Engine/ObjectTypes/Tile.h"
#include "../Engine/ObjectTypes/Scripted.h"

class QSFMLCanvas : public QWidget, public sf::RenderWindow, public RenderingContext, public Controller{
  Q_OBJECT
public :
  QSFMLCanvas(QWidget* Parent/*, const QPoint& Position, const QSize& Size*/, unsigned int FrameTime = 1);
  virtual ~QSFMLCanvas();
  void setEnabled(bool enabled);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent* event);
private :
  virtual void OnInit();
  virtual void OnUpdate();
  virtual QPaintEngine* paintEngine() const;
  virtual void showEvent(QShowEvent*);
  virtual void paintEvent(QPaintEvent*);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);
  virtual void resizeEvent(QResizeEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  static stack<QSFMLCanvas*> canvasStack;
  QTimer repaintTimer;
  bool myInitialized;
  bool isEnabled;
  Editable* currentSelected;
  QPoint mousePosition;
signals:
  void selectionChanged(Editable* selection);
  void editObjectSource(Scripted* scripted);
  void receivedDrop(const QMimeData* data);
};

#endif
