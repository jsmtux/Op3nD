#ifndef LUAEDITOR_H
#define LUAEDITOR_H

#include "ui_luaEditor.h"
#include "LuaHighlighter.h"
#include "../Engine/ObjectTypes/Scripted.h"
#include <QDialog>
#include <QFile>
#include <QTextStream>

class LuaEditor: public QDialog{
  Q_OBJECT
public:
  LuaEditor(QWidget* parent = 0);
  void exec(Scripted* scripted);
private:
  Scripted* currentScripted;
  Ui::EditorDialog dialog;
  LuaHighlighter* highlighter;
  QFile *sourceFile;
  QTextStream *source;
};

#endif
