#include "Renderer.h"
#include "Project.h"

void RendererBase::addElement(Tile* element)
{
  shaders.begin()->second.tileList.push_back(element);
}

void RendererBase::setShading(Tile* element, string shadingName)
{
    map<std::string, ShaderInfo>::iterator it = shaders.find(shadingName);
    
    if(it == shaders.end()){
      addShader(shadingName);
      it = shaders.find(shadingName);
      cout << "Adding shader " << shadingName << endl;
    }else{
      cout << "Shading " << shadingName << "exists" << endl;
    }
    
    it->second.tileList.push_back(element);
}

void RendererBase::draw()
{
  cout << "Drawing" << endl;
  for(pair<std::string, ShaderInfo> it:shaders){
    it.second.shader.useProgram();
    cout << "setting shader " << it.first << endl;
    update();
    for(int i = 0; i < it.second.tileList.size(); i++){
      cout << "drawing tile" << endl;
      it.second.tileList[i]->draw(&it.second.shader);
    }
  }
}

void RendererBase::addShader(string name)
{
  string dir;
  dir=Project::common()->getDir(dir.c_str()+1,Project::SHADER) + name;
  shaders.insert(pair<std::string, ShaderInfo>(
    name
    , {Shader(dir), {}}
  ));
}

PickingRenderer::PickingRenderer()
{
  addShader("picking.sfx");
}

void PickingRenderer::setResolution(Vector2 res)
{
  ptext = new PickingTexture(res.x,res.y);
}

void PickingRenderer::draw(){
  assert(ptext);
  ptext->bind();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RendererBase::draw();

  ptext->unBind();
}

unsigned int* PickingRenderer::getValue(int x, int y)
{
  return ptext->ReadId(x,y);
}

ForwardRenderer::ForwardRenderer(){
  addShader("normal.sfx");
}

void ForwardRenderer::draw(){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  RendererBase::draw();
  
  //shader->setObjMat(Matrix());
  //if(pWorld&&debug)pWorld->draw();
}

void RendererBase::setWVP(Matrix w){
  WVP=w;
}

void RendererBase::setWorldPos(Matrix pos){
  worldMat=pos;
}

void RendererBase::update(){
  #ifndef ANDROID
  glUniformMatrix4fv(wvpId, 1, GL_TRUE, &(WVP).m[0][0]);
  
  glUniformMatrix4fv(objMatId, 1, GL_TRUE, &(objMat).m[0][0]);
  #else
  Matrix mat;
  glUniformMatrix4fv(wvpId, 1, GL_FALSE, &WVP.transposed().m[0][0]);
  
  glUniformMatrix4fv(objMatId, 1, GL_FALSE, &objMat.transposed().m[0][0]);	
  #endif
}
