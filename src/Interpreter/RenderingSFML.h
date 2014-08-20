/**
 * @(#) RenderingContext.h
 */

#ifndef RENDERINGSFML_H_H
#define RENDERINGSFML_H_H

#include <Graphics/RenderingContext.h>
#include <Controller.h>
#include <SFML/Window.hpp>
#include <string>
using namespace std;

class RenderingSFML: public RenderingContext, public Controller
{
private:
    sf::Window* window;
public:
    void init();
    void update();
    RenderingSFML();
    void gameLoop();
};

#endif
