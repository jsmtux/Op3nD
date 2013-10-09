#ifndef Op3nD_H
#define Op3nD_H

#include <QtGui/QMainWindow>
#include "ui_mainWindow.h"
#include "InterfaceModule/ProjectDialog.h"
#include "InterfaceModule/ResourceTreeModel.h"

class Op3nD : public QMainWindow
{
Q_OBJECT
public:
    Op3nD();
    virtual ~Op3nD();
public slots:
  void editProject();
  void changeScene(QTreeWidgetItem* item, int column);
private:
  void updateScenesList();
  Ui::MainWindow window;
  ProjectDialog* projectDialog;
  ResourceTreeModel* resourceModel;
};

#endif // Op3nD_H
