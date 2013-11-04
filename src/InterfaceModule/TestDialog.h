#ifndef _TESTDIALOG
#define _TESTDIALOG
#include <QDialog>
#include "ui_testDialog.h"
#include "../ProjectManagement/Project.h"

class TestDialog: public QDialog{
  Q_OBJECT
public:
  TestDialog(QWidget* parent = 0);
  void exec(Project* project);
  void accept();
private:
  Ui::TestDialog testDialog;
  Project* currentProject;
};

#endif
