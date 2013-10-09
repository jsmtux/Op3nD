#include <list>
#include <vector>

#include "Shading.h"

#include "../Math/Matrix.h"
#include "../Math/Vector3.h"

#include "../../ProjectManagement/MXML.h"
#include "../../ProjectManagement/Project.h"
#include "../Base.h"
#include "GL/glew.h"
#include <sstream>

#ifdef ANDROID
#include <android/log.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
  const char *v = (const char *) glGetString(s);
  LOGI("GL %s = %s\n", name, v);
}
#endif

Shading* Shading::active;
stack<Shading*> Shading::shaderStack;
vector<GLuint> Shading::shaderlist;
Matrix Shading::WVP;
Matrix Shading::worldMat;
Matrix Shading::objMat;

Shading::Shading():program(glCreateProgram()){ 
  active = NULL;
  worldMatId=-1;
  objMatId=-1;
}

void Shading::initShader(string file){
  MXML::Tag root;
  MXML::XMLFile shaderFile(Base::getInstance()->getProj()->getDir(file,Project::SHADER),root,"shaders.dtd");
  shaderFile.read();
  #ifndef ANDROID
  string vshader ="#version " + root["version"].getAttrib().getString()+ "\n";
  string fshader= vshader;
  #else
  string vshader;
  string fshader;
  #endif
  vshader += root["vertex"].getAttrib().getString();
  fshader += root["fragment"].getAttrib().getString();
  
  addShader(vshader.c_str(),GL_VERTEX_SHADER);
  addShader(fshader.c_str(),GL_FRAGMENT_SHADER); 
  compileShader();
}

void Shading::update(){    
  #ifndef ANDROID
  glUniformMatrix4fv(wvpId, 1, GL_TRUE, &(WVP).m[0][0]);
  
  glUniformMatrix4fv(objMatId, 1, GL_TRUE, &(objMat).m[0][0]);
  #else
  Matrix mat;
  glUniformMatrix4fv(wvpId, 1, GL_FALSE, &WVP.transposed().m[0][0]);
  
  glUniformMatrix4fv(objMatId, 1, GL_FALSE, &objMat.transposed().m[0][0]);	
  #endif
}

GLint Shading::getId(string name){
  GLint Location= glGetUniformLocation(program, name.c_str());
  if (Location == 0xFFFFFFFF)
  {
    //cerr << "Warning! Unable to get the location of uniform " << name << endl;
  }
  
  return Location;
}

void Shading::deleteShading(){
  for (vector<GLuint>::iterator it = shaderlist.begin() ; it != shaderlist.end() ; ++it)
  {
    glDeleteShader(*it);
  }
  
  if (program != 0)
  {
    glDeleteProgram(program);
    program = 0;
  }
}

void Shading::addShader(string shader, GLenum shadertype){
  GLuint ShaderObj= glCreateShader(shadertype);
  shaderlist.push_back(ShaderObj);
  const GLchar* p[1];
  p[0] = shader.c_str();
  GLint Lengths[1];
  Lengths[0]= shader.length();
  glShaderSource(ShaderObj, 1, p, Lengths);
  glCompileShader(ShaderObj);
  
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
    #ifdef ANDROID
    LOGI("Error compiling shader: %s",InfoLog);
    exit(0);
    #endif
    cerr << "Error compiling shader type " << shadertype<<": " << InfoLog << endl;
    cerr << shader << endl;
  }
  glAttachShader(program, ShaderObj);
}

void Shading::compileShader(){
  glLinkProgram(program);
  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == 0) {
    GLchar ErrorLog[1024];
    glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
    cerr << "Error linking shader program: "<< ErrorLog<< endl;
  }
  glValidateProgram(program);
  initVars();
}

void Shading::initVars(){
  #ifdef ANDROID
  posId = glGetAttribLocation(gProgram, "Position");
  checkGlError("glGetAttribLocation");
  texId = glGetAttribLocation(gProgram, "TexCoord");
  checkGlError("glGetAttribLocation");
  normId = glGetAttribLocation(gProgram, "Normal");
  checkGlError("glGetAttribLocation");
  samplerId = glGetAttribLocation(gProgram, "gSampler");
  checkGlError("glGetAttribLocation");
  #else    
  GLuint posId=0;
  GLuint texId=1;
  GLuint normId=2;
  #endif
  
  wvpId=getId("gWVP");
  worldMatId=getId("gWorld");
  objMatId=getId("gObjMat");
  
  objId=getId("gObjectIndex");
  meshId=getId("gMeshIndex");
  
  colorId=getId("color");
  
  animatedId=getId("animated");
  
  for(int i=0;i<MAX_BONES;i++){
    stringstream str;
    str << "gBones[" << i <<"]";
    boneLocation[i]= getId(str.str());
  }
}

void  Shading::setBoneTransform(uint ind, const Matrix& transform){
  assert(ind<MAX_BONES);
  glUniformMatrix4fv(boneLocation[ind],1,GL_TRUE,(const GLfloat*)transform.m);
}

void Shading::setWVP(Matrix w){
  WVP=w;
}

void Shading::setWorldPos(Matrix pos){
  worldMat=pos;
}

void Shading::setObjMat(Matrix obj){    
  objMat=obj;
  glUniformMatrix4fv(objMatId, 1, GL_TRUE, &(objMat).m[0][0]);
}

void Shading::useProgram(){
  active=this;
  glUseProgram(program);
  update();
}

GLuint Shading::getProgramInd(){
  return program;
}

Shading* Shading::getActive(){
  return active;
}

void Shading::setColor(Vector3 col){
  glUniform3f(colorId, col.x, col.y, col.z );
}

void Shading::setObjInd(unsigned int ind){
  if(objId!=0xFFFFFFFF){
    #ifndef ANDROID
    glUniform1ui(objId, ind);
    #else
    glUniform1i(objId, ind);
    #endif
  }
}

void Shading::setMeshInd(unsigned int ind){
  if(meshId!=0xFFFFFFFF){
    #ifndef ANDROID
    glUniform1ui(meshId, ind);
    #else
    glUniform1i(meshId, ind);
    #endif
  }
}

void Shading::setAnimated(bool animated){
  if(animatedId!=0xFFFFFFFF){
    glUniform1i(animatedId, animated);
  }
}

void Shading::push(){
  shaderStack.push(active);
}

void Shading::pop(){
  if(shaderStack.empty())return;
		   shaderStack.top()->useProgram();
  shaderStack.pop();    
}

GLuint Shading::getPosLocation()
{
  return 0;
}

GLuint Shading::getTexLocation()
{
  return 1;
}

GLuint Shading::getNormLocation()
{
  return 2;
}

GLuint Shading::getSamplerLocation()
{
  return 3;
}
