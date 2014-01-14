#include "LuaEditor.h"
#include <QInputDialog>
#include <QMessageBox>
#include "../src/Engine/Base.h"
#include "../src/ProjectManagement/Project.h"

LuaEditor::LuaEditor(QWidget* parent): QDialog(parent)
{
  dialog.setupUi(this);
  highlighter = new LuaHighlighter(dialog.textEdit->document());
}

void LuaEditor::exec(QString file)
{
  path=file;
  if(!path.contains('.')){
    path=path+".o3s";
  }
  if(!path.contains('/')){
    path=QString::fromStdString(Base::getInstance()->getProj()->getDir((path).toStdString(),Project::OBJECT));
  }
  
  QFile sourceFile(path);
  sourceFile.open(QIODevice::ReadOnly|QIODevice::Text);
  QTextStream source(&sourceFile);
  dialog.textEdit->setText(source.readAll());
  QDialog::exec();
}

void LuaEditor::newScript()
{
  bool ok;
  QString name;
  do{
    if(!name.isEmpty()){
      QMessageBox msg;
      msg.setText(tr("Chosen name must not contain '.', please enter a new one"));
      msg.exec();
    }
    name=QInputDialog::getText(NULL,"Please enter new script name",
				      "Name:",QLineEdit::Normal,"",&ok);
  }while(name.contains('.'));
  
  if(ok&&!name.isEmpty()){
    string path=Base::getInstance()->getProj()->getDir((name+".o3s").toStdString(),Project::OBJECT);
    QFile file(path.c_str());
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << Scripted::getTemplate(name.toStdString()).c_str();
    file.close();
    emit scriptListNeedsReload();
    exec(path.c_str());
  }
}

void LuaEditor::accept()
{
  QFile sourceFile(path);
  sourceFile.open(QIODevice::WriteOnly|QIODevice::Text);
  sourceFile.resize(0);
  QTextStream source(&sourceFile);
  
  source << dialog.textEdit->toPlainText();
  
  QDialog::accept();
}

void LuaEditor::reject()
{
  QDialog::reject();
}

