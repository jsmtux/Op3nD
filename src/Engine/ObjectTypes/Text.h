/**
 * @(#) Text.h
 */

#ifndef TEXT_H_H
#define TEXT_H_H
#include "Resource.h"
#include "Font.h"
#include "Tile.h"
#include "../Math/Quaternion.h"

class Text: public Resource{
public:
  ResourceType getType();
  static Text* loadText(string text,string font);
  void Draw();
  void setText(string text);
  string getText();
private:
  Text(string text, string font);
  void del();
  Font* font;
  string text;
};

#endif
