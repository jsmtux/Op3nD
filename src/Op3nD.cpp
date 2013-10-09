#include "Op3nD.h"

#include <QDebug>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QMessageBox>
#include "./Engine/Base.h"
#include "InterfaceModule/ResourceTreeModel.h"

Op3nD::Op3nD()
{
  window.setupUi(this);
  projectDialog= new ProjectDialog();
  connect(window.actionProjectProperties,SIGNAL(triggered(bool)),SLOT(editProject()));
  connect(window.twScene,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),SLOT(changeScene(QTreeWidgetItem*,int)));
  
  projectDialog->exec();
  while(Base::getInstance()->getProj()==NULL){
    if(projectDialog->shouldExit()){
      exit(0);
    }
    QMessageBox msgBox;
    msgBox.setText(tr("No project selected"));
    msgBox.setInformativeText(tr("Please select a project"));
    msgBox.exec();
    projectDialog->exec();
  }
  resourceModel=new ResourceTreeModel(Base::getInstance()->getProj());
  resourceModel->scanDirs();
  window.tvResources->setModel(resourceModel);
  updateScenesList();
}

Op3nD::~Op3nD()
{}

void Op3nD::editProject()
{
  projectDialog->exec();
}

void Op3nD::updateScenesList()
{
  if(!Base::getInstance()){
    qFatal("Uninitialized base project");
  }
  
  window.twScene->clear();
  vector<string> scenes = Base::getInstance()->getProj()->listFiles(Project::MAP);
  
  for(string scene:scenes){
    window.twScene->addTopLevelItem(new QTreeWidgetItem(QStringList()<<scene.c_str()));
  }
}

void Op3nD::changeScene(QTreeWidgetItem* item, int column)
{
  qDebug() << "Changing scene to :" << item->text(0);
  Base::getInstance()->newState(item->text(0).toStdString(),EDITORST);
  Base::getInstance()->changeState(item->text(0).toStdString(),EDITORST);
  Base::getInstance()->beginState();
  Base::getInstance()->getCurState()->loadFile();
}
