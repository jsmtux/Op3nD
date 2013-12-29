#ifndef Op3nD_H
#define Op3nD_H

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "InterfaceModule/ProjectDialog.h"
#include "InterfaceModule/ResourceTreeModel.h"
#include "InterfaceModule/TestDialog.h"
#include "InterfaceModule/PropertiesTreeModel.h"
#include "InterfaceModule/SpinBoxDelegate.h"
#include "InterfaceModule/LuaEditor.h"
#include "InterfaceModule/PhysicsDialog.h"
#include "Engine/ObjectTypes/Editable.h"

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
