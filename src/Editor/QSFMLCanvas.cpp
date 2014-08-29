#include "QSFMLCanvas.h"
#include <QDebug>
#include <QKeyEvent>
#include <QAction>
#include "../Engine/Base.h"
#include "../Engine/ObjectTypes/Scripted.h"

stack<QSFMLCanvas*> QSFMLCanvas::canvasStack;

QSFMLCanvas::QSFMLCanvas(QWidget* Parent,/* const QPoint& Position, const QSize& Size,*/ unsigned int FrameTime) :
QGLWidget       (Parent),
myInitialized (false)
{
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
    QWidget::setAttribute(Qt::WA_OpaquePaintEvent);
    QWidget::setAttribute(Qt::WA_NoSystemBackground);
    QWidget::setAttribute(Qt::WA_PaintUnclipped);

    setFocusPolicy(Qt::StrongFocus);

    Base::getInstance()->setRC(this);
    Base::getInstance()->addController(this);
    
    repaintTimer.setInterval(FrameTime);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    currentSelected=nullptr;
    isEnabled=true;
    setAcceptDrops(true);
}

QSFMLCanvas::~QSFMLCanvas()
{
  canvasStack.pop();
  if(!canvasStack.empty()){
    canvasStack.top()->setEnabled(true);
  }
}

void QSFMLCanvas::setEnabled(bool enabled)
{
  static int sizeDelta=1;
  isEnabled=enabled;
  if(enabled){//Hack: Force size change in order to reload camera properties
    QSize size=parentWidget()->size();
    size.setWidth(size.width()-sizeDelta);
    sizeDelta*=-1;
    parentWidget()->resize(size);
  }
}

#ifdef Q_WS_X11
  #include <Qt/qx11info_x11.h>
  #include <qevent.h>
  #include <X11/Xlib.h>
#endif

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        
        RenderWindow::create(QGLWidget::winId());

        myInitialized = true;
	if(!canvasStack.empty()){
	  canvasStack.top()->setEnabled(false);
	}
	canvasStack.push(this);
	
    }
}

void QSFMLCanvas::initializeGL()
{
  RenderingContext::init();
  connect(&repaintTimer, SIGNAL(timeout()), this, SLOT(update()));
 repaintTimer.start();

}

void QSFMLCanvas::paintGL()
{
  if(isEnabled){
    Base::getInstance()->setRC(this);
    Base::getInstance()->getStateManager()->iteration();
    display();
  }
}

void QSFMLCanvas::keyPressEvent(QKeyEvent* event)
{
  QWidget::keyPressEvent(event);
  switch(event->key()){
    case 16777235:
      setAxis(A_X,-1);
      break;
    case 16777237:
      setAxis(A_X,1);
      break;
    case 16777234:
      setAxis(A_Y,-1);
      break;
    case 16777236:
      setAxis(A_Y,1);
      break;
    case 87:
      setKey(K_UP,true);
      break;
    case 83:
      setKey(K_DOWN,true);
      break;
    case 65:
      setKey(K_LEFT,true);
      break;
    case 68:
      setKey(K_RIGHT,true);
      break;
    case 32:
      setKey(K_A,true);
      break;
  }
}

void QSFMLCanvas::keyReleaseEvent(QKeyEvent* event)
{
  QWidget::keyReleaseEvent(event);
  switch(event->key()){
    case 16777235:
      setAxis(A_X,0);
      break;
    case 16777237:
      setAxis(A_X,0);
      break;
    case 16777234:
      setAxis(A_Y,0);
      break;
    case 16777236:
      setAxis(A_Y,0);
      break;
    case 87:
      setKey(K_UP,false);
      break;
    case 83:
      setKey(K_DOWN,false);
      break;
    case 65:
      setKey(K_LEFT,false);
      break;
    case 68:
      setKey(K_RIGHT,false);
      break;
    case 32:
      setKey(K_A,false);
      break;
  }
}

void QSFMLCanvas::resizeGL(int width, int height)
{
  Vector3 resolution(width,height,32);
  
  setResolution(resolution);
  
  glViewport(0,0,width,height);
}


void QSFMLCanvas::mouseDoubleClickEvent(QMouseEvent* event)
{
  setFocus();
}

void QSFMLCanvas::mousePressEvent(QMouseEvent* event)
{
  setFocus();
  setKey(K_A,true);
  mousePosition=event->pos();
  State* curState = Base::getInstance()->getStateManager()->getCurState();
  if(curState!=nullptr){
    unsigned int *sel=curState->selection(event->x(),event->y());
    setSelection(sel);
    currentSelected = curState->getByIndex(sel[0]);
    if(currentSelected){
      emit selectionChanged(curState->getByIndex(sel[0]));
    }
  }
  QWidget::mousePressEvent(event);
}

void QSFMLCanvas::mouseMoveEvent(QMouseEvent* event)
{
  QWidget::mouseMoveEvent(event);
  setAxis(A_L,event->pos().x()-mousePosition.x());
  setAxis(A_R,event->pos().y()-mousePosition.y());
  mousePosition=event->pos();
}

void QSFMLCanvas::mouseReleaseEvent(QMouseEvent* event)
{
  setKey(K_A,false);
  QWidget::mouseReleaseEvent(event);
}

void QSFMLCanvas::dragEnterEvent(QDragEnterEvent* event)
{
  if (event->mimeData()->hasFormat("application/text.tile")){
    event->acceptProposedAction();
    cout << "Accepted action!!" << endl;
  }
}

void QSFMLCanvas::dropEvent(QDropEvent* event)
{
  cout << "Received drop!!" << endl;
  emit receivedDrop(event->mimeData());
  event->acceptProposedAction();
}
