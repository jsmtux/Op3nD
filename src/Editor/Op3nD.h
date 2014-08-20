#ifndef Op3nD_H
#define Op3nD_H

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "ProjectDialog.h"
#include "ResourceTreeModel.h"
#include "TestDialog.h"
#include "PropertiesTreeModel.h"
#include "SpinBoxDelegate.h"
#include "LuaEditor.h"
#include "PhysicsDialog.h"
#include <ObjectTypes/Editable.h>

class Op3nD : public QMainWindow
{
Q_OBJECT
public:
    Op3nD();
    virtual ~Op3nD();
public slots:
  void editProject();
  void changeScene(QTreeWidgetItem* item, int column);
  void launchTest();
  void changeSelection(Editable* sel);
  void execPhysicsDialog(string resource);
  void editScript();
  void addScene();
  void saveCurrentScene();
  void elementDroppedScene(const QMimeData* data);
  void addCameraToScene();
  void addFontToProject();
private:
  void updateScenesList();
  Ui::MainWindow window;
  ProjectDialog* projectDialog;
  ResourceTreeModel* resourceModel;
  PropertiesTreeModel* propertiesModel;
  LuaEditor* luaEditor;
  TestDialog testdialog;
  SpinBoxDelegate delegate;
  QAction* editableCopyAction;
  QAction* editableDeleteAction;
  QAction* objectEditAction;
};

#endif // Op3nD_H
