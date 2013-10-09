#include "QSFMLCanvas.h"
#include "../Engine/Base.h"
#include <QDebug>

QSFMLCanvas::QSFMLCanvas(QWidget* Parent,/* const QPoint& Position, const QSize& Size,*/ unsigned int FrameTime) :
QWidget       (Parent),
myInitialized (false)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);

    //move(Position);
    //resize(Size);

    myTimer.setInterval(FrameTime);
    Base::getInstance()->setRC(this);
    Base::getInstance()->addController(this);
}

QSFMLCanvas::~QSFMLCanvas()
{

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

        
        RenderWindow::create(winId());

        OnInit();
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    OnUpdate();
    display();
}

void QSFMLCanvas::OnInit()
{
  RenderingContext::init();
}

void QSFMLCanvas::OnUpdate()
{
  if(Base::getInstance()->getCurState()){
    Base::getInstance()->getCurState()->iteration();
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
  }
}

void QSFMLCanvas::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  
  int w=event->size().width();
  int h=event->size().height();
  Vector3 resolution(w,h,32);
  
  Base::getInstance()->setResolution(resolution);
  
  glViewport(0,0,w,h);
}

void QSFMLCanvas::mouseDoubleClickEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);
  setSelection(Base::getInstance()->getCurState()->selection(event->x(),event->y()));
  
}

