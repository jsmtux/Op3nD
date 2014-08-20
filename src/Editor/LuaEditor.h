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
public slots:
  void exec(QString file);
  void newScript();
  void accept();
  void reject();
private:
  Ui::EditorDialog dialog;
  LuaHighlighter* highlighter;
  QString path;
signals:
  void scriptListNeedsReload();
};

#endif
