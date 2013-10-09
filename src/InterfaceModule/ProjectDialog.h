#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include "ui_newProjectDialog.h"

class ProjectDialog: public QDialog{
  Q_OBJECT
public:
  ProjectDialog();
  bool shouldExit();
  void accept(QString directory);
public slots:
  void setImage();
  void updateDir();
  void openProject();
  void openRecentProject();
  void changeProjectDir();
  void acceptCreate();
  void exit();
private:
  Ui::Dialog dialog;
  bool defaultDir;
  bool exitFlag;
  
  void addProjectToRecent(QString dir);
};

#endif