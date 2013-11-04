#ifndef _PHYSICSDIALOG
#define _PHYSICSDIALOG

#include "ui_physicsDialog.h"
#include "../src/Engine/ObjectTypes/Physical.h"

class PhysicsDialog: public QDialog{
  Q_OBJECT
public:
  PhysicsDialog(QWidget* parent = 0);
  void exec(Editable* editable);
  void closeEvent(QCloseEvent *ev);
public slots:
  void addElementMenu(QAction* action);
  void updateShapesList();
private:
  Ui::PhysicsDialog physicsDialog;
  QString prevState;
  Editable* mesh;
  struct shapeInfo{
    int id;
    btRigidBody* rigidBody;
    tShape shapeType;
  };
  QList<shapeInfo> shapes;
  int numShapes=0;
};

#endif
