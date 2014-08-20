/**
 * @(#) RenderingSFML.cpp
 */

#include <Graphics/RenderingContext.h>
#include <Base.h>
#include <SFML/Window/Keyboard.hpp>
#include "RenderingSFML.h"

RenderingSFML::RenderingSFML(){
  appName="Open3D";
  Base::getInstance()->addController(this);
}

void RenderingSFML::init()
{
  Vector3 resolution=getResolution();
  
  sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
  window= new sf::Window(sf::VideoMode(resolution.x,resolution.y,resolution.z),appName,sf::Style::Default,settings);

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
  setKey(K_A,sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    exit(0);
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
    delete window;
    Vector3 resolution = getResolution();
    window=new sf::Window(sf::VideoMode(resolution.x,resolution.y,resolution.z),appName,sf::Style::Fullscreen);
  }
}


void RenderingSFML::gameLoop(){
  StateManager* stateManager=Base::getInstance()->getStateManager();
  while(stateManager->iteration()){
    update();
  }
}
