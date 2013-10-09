#ifndef _QSFMLCANVAS
#define _QSFMLCANVAS

#include <QWidget>
#include <SFML/Graphics.hpp>
#include <QTimer>
#include "../Engine/Graphics/RenderingContext.h"
#include "../Engine/Controller.h"

class QSFMLCanvas : public QWidget, public sf::RenderWindow, public RenderingContext, public Controller{
  Q_OBJECT
public :
    QSFMLCanvas(QWidget* Parent/*, const QPoint& Position, const QSize& Size*/, unsigned int FrameTime = 1);
    virtual ~QSFMLCanvas();

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
    QTimer myTimer;
    bool   myInitialized;
};

#endif
