#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <ObjectTypes/Tile.h>
#include "Shader.h"
#include "PickingTexture.h"

class RendererBase{
public:
  void addElement(Tile* element);
  void setShading(Tile* element, string shadingName);
  virtual void draw();

  void setWVP(Matrix w);
  void setWorldPos(Matrix pos);
  void setObjMat(Matrix obj);
  void update();
protected:
  void addShader(std::string name);
private:
  struct ShaderInfo{
    Shader shader;
    vector<Tile*> tileList;
  };
  map<std::string, ShaderInfo> shaders;
  Matrix WVP, worldMat, objMat;
  GLuint wvpId, worldMatId, objMatId;
};

class PickingRenderer: public RendererBase {
public:
  PickingRenderer();
  void setResolution(Vector2 res);
  void draw();
  unsigned int* getValue(int x, int y);
private:
  PickingTexture* ptext;
};

class ForwardRenderer: public RendererBase {
  ForwardRenderer();
  void draw();
};

#endif