#include "TestDialog.h"
#include <QProcess>

TestDialog::TestDialog(QWidget* parent): QDialog(parent)
{
  testDialog.setupUi(this);
  testDialog.cbTestOptions->addItem("Default");
  testDialog.cbResOptions->addItem("640x480");
  testDialog.cbResOptions->addItem("800x600");
  testDialog.cbResOptions->addItem("1024x768");
  testDialog.cbShowLog->setCheckState(Qt::Checked);
}

void TestDialog::exec(Project* project)
{
  currentProject=project;
  testDialog.cbTestOptions->clear();
  vector<string> maps = project->listFiles(Project::MAP);
  for(string map:maps){
    testDialog.cbTestOptions->addItem(map.c_str());
  }
  QDialog::exec();
}

void TestDialog::accept()
{
  QDialog::accept();
  QProcess testProc;
  QStringList arguments;
  arguments << "-hold" << "-e" << "./Op3nD_Interpreter" << currentProject->getDir().c_str() << "-map"<<
	    testDialog.cbTestOptions->currentText() << "-res" << testDialog.cbResOptions->currentText();
	    
  if(testDialog.cbFullScreen->checkState()==Qt::Checked){
    arguments << "-fullscreen";
  }
  testProc.start("xterm",arguments);
  testProc.waitForFinished(-1);
}
