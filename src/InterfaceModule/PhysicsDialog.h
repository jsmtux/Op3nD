#ifndef _PHYSICSDIALOG
#define _PHYSICSDIALOG

#include "ui_physicsDialog.h"
#include "../src/Engine/ObjectTypes/Physical.h"

class PhysicsDialog: public QDialog{
  Q_OBJECT
public:
  PhysicsDialog(QWidget* parent = 0);
  void exec(string resource);
  void closeEvent(QCloseEvent *ev);
  void accept();
  void reject();
public slots:
  void addElementMenu(QAction* action);
  void updateShapesList();
  void selectShape(int row);
  void deleteShape();
  void updateShape();
private:
  int getNewId();
  void restoreCanvas();
  void clear();
  Ui::PhysicsDialog physicsDialog;
  string toEdit;
  struct shapeInfo{
    int id;
    btRigidBody* rigidBody;
    tShape shapeType;
    Vector3 size;
    Vector3 offset;
    Quaternion rotation;
  };
  QList<shapeInfo> shapes;
};

#endif
