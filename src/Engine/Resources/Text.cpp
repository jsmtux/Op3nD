#include "Text.h"
#include "FontLoader.h"

Text::Text(string _text, Font* _font)
{
  text = _text;
  font = _font;
}

void Text::Draw(Shader* shader)
{
  font->Draw(shader, text);
}

Resource::ResourceType Text::getType()
{
  return TEXT;
}

string Text::getText()
{
  return text;
}

void Text::setText(string text)
{
  this->text=text;
}
