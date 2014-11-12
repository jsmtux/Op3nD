#include "Renderer.h"
#include "Project.h"
#include "Shader.h"
#include "ObjectTypes/Editable.h"
#include "Graphics/PickingTexture.h"

void Renderer::draw()
{
  for(pair<std::string, ShaderSet> set:shaderList){
    set.second.shader->useProgram();
    set.second.shader->update(WVP, worldMat);
    for(Editable* editable:set.second.toDrawList){
      editable->draw(set.second.shader);
    }
  }
}

void* Renderer::addDrawElement(Editable* editable, string shaderName)
{
  if (shaderList.find(shaderName) == shaderList.end())
  {
    ShaderSet newSet;
    newSet.shader = new Shader();
    newSet.shader->initShader(Project::common()->getDir(shaderName, Project::SHADER));

    shaderList.insert(pair<string, ShaderSet>(shaderName, newSet));
  }
  ShaderSet& set = shaderList.find(shaderName)->second;

  set.toDrawList.push_back(editable);
}

void Renderer::deleteElement(Editable* editable)
{
  std::map<std::string, ShaderSet>::iterator it;
  for (it = shaderList.begin(); it != shaderList.end(); it++)
  {
    std::vector<Editable*> *list = &it->second.toDrawList;
    std::vector<Editable*>::iterator it2 = std::find(list->begin(), list->end(), editable);
    if (it2 != list->end())
    {
      list->erase(it2);
    }
  }
}

void Renderer::setWVP(Matrix w){
  WVP=w;
}

void Renderer::setWorldPos(Matrix pos){
  worldMat=pos;
}

PickingRenderer::PickingRenderer(Vector3 res)
{
  ptext = new PickingTexture(res.x,res.y);
}

unsigned int* PickingRenderer::selection(const int x, const int y)
{
    ptext->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw();

    ptext->unBind();
    return ptext->ReadId(x,y);
}

void PickingRenderer::updateResolution(Vector3 res)
{
  delete ptext;
  ptext = new PickingTexture(res.x,res.y);
}
