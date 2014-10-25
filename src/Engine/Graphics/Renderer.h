#ifndef RENDERER_H_
#define RENDERER_H_

#include <string>
#include <vector>
#include <map>

#include "Math/Matrix.h"

class Editable;
class Shader;
class PickingTexture;

class Renderer
{
public:
  void draw();

  void* addDrawElement(Editable* editable, std::string shaderName);

  void setWVP(Matrix w);
  void setWorldPos(Matrix pos);
protected:
  struct ShaderSet{
    Shader* shader;
    std::vector<Editable*> toDrawList;
  };
  std::map<std::string, ShaderSet> shaderList;
  Matrix WVP, worldMat;
};

class PickingRenderer: public Renderer
{
public:
  PickingRenderer(Vector3 res);
  void* addDrawElement(Editable* editable, string shaderName = "picking.sfx") { Renderer::addDrawElement(editable, shaderName);}
  void updateResolution(Vector3 res);
  /**
    * Selects the object in the positions x y
    * @param x mouse postition
    * @param y mouse position
    * @return id of the object
    */
  unsigned int* selection( const int x, const int y );
private:
  PickingTexture *ptext;
};

#endif