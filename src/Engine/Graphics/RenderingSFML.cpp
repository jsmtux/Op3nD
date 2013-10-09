/**
 * @(#) RenderingSFML.cpp
 */

#include "RenderingSFML.h"
#include "../Base.h"
#include <SFML/Window/Keyboard.hpp>

RenderingSFML::RenderingSFML(){
  appName="Open3D";
  Base::getInstance()->addController(this);
}

void RenderingSFML::init()
{
  Vector3 resolution=Base::getInstance()->getResolution();
  window= new sf::Window(sf::VideoMode(resolution.x,resolution.y,resolution.z),appName);
  window->setFramerateLimit(60);
  window->setActive();
  window->display();
  bgColor=Vector3(0.,0.,0.);
  RenderingContext::init();
}

void RenderingSFML::update(){
  window->display();
  setKey(K_LEFT,sf::Keyboard::isKeyPressed(sf::Keyboard::A));
  setKey(K_RIGHT,sf::Keyboard::isKeyPressed(sf::Keyboard::D));
  setKey(K_UP,sf::Keyboard::isKeyPressed(sf::Keyboard::W));
  setKey(K_DOWN,sf::Keyboard::isKeyPressed(sf::Keyboard::S));
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    exit(0);
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
    delete window;
    Vector3 resolution=Base::getInstance()->getResolution();
    window=new sf::Window(sf::VideoMode(resolution.x,resolution.y,resolution.z),appName,sf::Style::Fullscreen);
  }
}
