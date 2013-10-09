#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QSettings>

#include "ProjectDialog.h"
#include "../ProjectManagement/Project.h"
#include "../Engine/Base.h"

#define MAX_LAST_RECORD 5

ProjectDialog::ProjectDialog()
{
  dialog.setupUi(this);
  exitFlag=false;
  connect(dialog.pbSetImage,SIGNAL(clicked(bool)),SLOT(setImage()));
  connect(dialog.leProName,SIGNAL(textChanged(QString)),SLOT(updateDir()));
  connect(dialog.pbChangeDir,SIGNAL(clicked(bool)),SLOT(changeProjectDir()));
  connect(dialog.pbAccept,SIGNAL(clicked(bool)),SLOT(acceptCreate()));
  connect(dialog.pbExit,SIGNAL(clicked(bool)),SLOT(exit()));
  connect(dialog.tbOpenProj,SIGNAL(clicked(bool)),SLOT(openProject()));
  
  dialog.tbOpenProj->setPopupMode(QToolButton::MenuButtonPopup);
  dialog.tbOpenProj->setMenu(new QMenu("menu"));
  
  QSettings settings;
  settings.beginGroup("lastOpened");
  const int begIndex=settings.value("lastIndex").toInt();
  for(int i=0;i<MAX_LAST_RECORD;i++){
    int realIndex=(begIndex-i)%MAX_LAST_RECORD;
    if(realIndex<0){
      realIndex+=MAX_LAST_RECORD;
    }
    
    QString indexName="project"+QString::number(realIndex);
    if(!settings.value(indexName).isValid()){
      break;
    }
    QAction* menuAction=new QAction(settings.value(indexName).toString(),this);
    menuAction->setData(settings.value(indexName+"Dir").toString());
    menuAction->setToolTip(settings.value(indexName+"Dir").toString());
    dialog.tbOpenProj->menu()->addAction(menuAction);
    connect(menuAction,SIGNAL(triggered(bool)),SLOT(openRecentProject()));
  }
  
  defaultDir=true;
  updateDir();
}

void ProjectDialog::accept(QString directory)
{
  Base::getInstance()->setProj(directory.toStdString());
  
  addProjectToRecent(directory);
  
  QDialog::accept();
}

void ProjectDialog::addProjectToRecent(QString dir)
{
  QSettings settings;
  settings.beginGroup("lastOpened");
  
  const int lastIndex=settings.value("lastIndex").toInt();
  if(settings.value("project"+QString::number(lastIndex)+"Dir").toString().compare(dir)!=0){
    const int currIndex=(lastIndex+1)%MAX_LAST_RECORD;
    settings.setValue("lastIndex",currIndex);
    const QString indexName="project"+QString::number(currIndex);
    settings.setValue(indexName,Base::getInstance()->getProj()->getName().c_str());
    settings.setValue(indexName+"Dir",dir);
  }
}


void ProjectDialog::setImage()
{
  QString filename=QFileDialog::getOpenFileName(this,tr("Open image"), QDir::home().absolutePath(), tr("Image Files(*.png *.jpg *.bmp *.gif)"));
  QImage projectImage;
  projectImage.load(filename);
  dialog.imageLabel->setPixmap(QPixmap::fromImage(projectImage.scaledToWidth(250)));
}

void ProjectDialog::updateDir()
{
  if(!defaultDir){
    return;
  }
  dialog.lbProjDir->setText(QDir::homePath()+"/O3D/"+dialog.leProName->text().replace(" ","_"));
}

void ProjectDialog::changeProjectDir()
{
  QString filename=QFileDialog::getExistingDirectory(this,tr("Choose folder"), QDir::home().absolutePath());
  if(filename.isEmpty()){
    return;
  }
  defaultDir=false;
  dialog.lbProjDir->setText(filename);
}

void ProjectDialog::acceptCreate()
{
  bool result = Project::createProject(dialog.leProName->text().toStdString(),dialog.lbProjDir->text().toStdString(),
			 dialog.leProVersion->text().toStdString(),dialog.teProDesc->toPlainText().toStdString());
  if(!result){
    QMessageBox msgBox(this);
    msgBox.setText(tr("Error creating project"));
    msgBox.setInformativeText(tr("Please, choose another directory"));
    msgBox.exec();
    return;
  }
  const QPixmap* projectLogo=dialog.imageLabel->pixmap();
  if(projectLogo){
    projectLogo->save(dialog.lbProjDir->text()+"/logo.png");
  }
  accept(dialog.lbProjDir->text());
}

void ProjectDialog::openProject()
{
  QString dir=QFileDialog::getExistingDirectory(this,"Choose project dir",QDir::homePath());
  
  while(!Project::isProject(dir.toStdString())&&!dir.isEmpty()){    
    QMessageBox msgBox(this);
    msgBox.setText(tr("Selected folder is not an Op3nD project"));
    msgBox.setInformativeText(tr("Please select another one"));
    msgBox.exec();
    QString dir=QFileDialog::getExistingDirectory(this,"Choose project dir",QDir::homePath());
  }
  
  if(!dir.isEmpty()){
    accept(dir);
  }
}

void ProjectDialog::openRecentProject()
{
  QAction* sender;
  try{
    sender=dynamic_cast<QAction*>(QObject::sender());
  }catch(exception *e){
    return;
  }
  
  accept(sender->data().toString());
}

void ProjectDialog::exit()
{
  exitFlag=true;
  close();
}

bool ProjectDialog::shouldExit()
{
  return exitFlag;
}

