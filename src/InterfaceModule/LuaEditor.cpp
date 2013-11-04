#include "LuaEditor.h"

LuaEditor::LuaEditor(QWidget* parent): QDialog(parent)
{
  dialog.setupUi(this);
  highlighter = new LuaHighlighter(dialog.textEdit->document());
}

void LuaEditor::exec(Scripted* scripted)
{
  currentScripted=scripted;
  sourceFile = new QFile(scripted->getPath().c_str());
  sourceFile->open(QIODevice::ReadWrite|QIODevice::Text);
  source= new QTextStream(sourceFile);
  dialog.textEdit->setText(source->readAll());
  QDialog::exec();
}
