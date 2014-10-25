/**
 * @(#) Text.h
 */

#ifndef TEXT_H_H
#define TEXT_H_H
#include "Resource.h"
#include "Font.h"
#include "../Math/Quaternion.h"

class Text: public Resource{
public:
  ResourceType getType();
  void Draw(Shader* shader);
  void setText(string text);
  string getText();
  string getDefaultShader(){return "text.sfx";}
private:
  Text(string text, Font* font);
  Font* font;
  string text;
};

#endif
