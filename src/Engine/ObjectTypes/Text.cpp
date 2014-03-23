#include "Text.h"

Text* Text::loadText(string text, string font)
{
  return new Text(text,font);
}

Text::Text(string text, string font)
{
  this->font=Font::loadFont(font);
  this->text=text;
}

void Text::Draw()
{
  font->Draw(text);
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

void Text::del()
{
  font->free();
}
