#include "Op3nD.h"

#include <QDebug>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QMessageBox>
#include "./Engine/Base.h"
#include "Engine/ObjectTypes/Resource.h"
#include "InterfaceModule/ResourceTreeModel.h"
#include "InterfaceModule/SpinBoxDelegate.h"

Op3nD::Op3nD()
{
  projectDialog= new ProjectDialog();
  
  projectDialog->exec();
  
  window.setupUi(this);
  
  luaEditor= new LuaEditor(this);
  
  connect(window.actionProjectProperties,SIGNAL(triggered(bool)),SLOT(editProject()));
  connect(window.twScene,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),SLOT(changeScene(QTreeWidgetItem*,int)));
  connect(window.actionPlay,SIGNAL(triggered(bool)),SLOT(launchTest()));
  connect(window.SFMLWidget,SIGNAL(selectionChanged(Editable*)),SLOT(changeSelection(Editable*)));
  connect(window.SFMLWidget,&QSFMLCanvas::editObjectSource,luaEditor,&LuaEditor::exec);
  window.actionPlay->setIcon(QIcon::fromTheme("media-playback-start"));
  
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
  propertiesModel=NULL;
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

void Op3nD::launchTest()
{
  testdialog.exec(Base::getInstance()->getProj());
}

void Op3nD::changeSelection(Editable* sel)
{
  window.tvProperties->setModel(NULL);
  if(propertiesModel){
    delete propertiesModel;
    propertiesModel=NULL;
  }
  if(sel){
    propertiesModel = new PropertiesTreeModel(sel);
    connect(propertiesModel,&PropertiesTreeModel::editShape,this,&Op3nD::execPhysicsDialog);
    window.tvProperties->setModel(propertiesModel);
    window.tvProperties->setItemDelegate(&delegate);
  }
}

void Op3nD::execPhysicsDialog(Editable* ed)
{
  PhysicsDialog physDialog;
  physDialog.exec(ed);
}
