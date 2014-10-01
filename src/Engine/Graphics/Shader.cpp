#include <list>
#include <vector>

#include "Shader.h"

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

Shader::Shader():program(glCreateProgram()){ 
}

Shader::Shader(string file)
  : program(glCreateProgram())
{
  initShader(file);
}

void Shader::initShader(string file){
  MXML::Tag root;
  MXML::XMLFile shaderFile(file,root,"shaders.dtd");
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

void Shader::addShader(string shader, GLenum shadertype){
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

void Shader::compileShader(){
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

GLint Shader::getId(string name, int index)
{
  GLint tmp = glGetUniformLocation(program, name.c_str());
  if(tmp == -1){
    tmp = glGetAttribLocation(program, name.c_str());
  }
  return tmp;
}

GLint Shader::getVarLocation(string name, int index)
{
  map<string,GLint>::iterator tmp;
  if(index != -1){
    stringstream str;
    str << name << "[" << index <<"]";
    name = str.str();
  }
  if((tmp=shaderLocs.find(name))==shaderLocs.end()){
    cout << name << "not found";
    GLint ret = getId(name, index);
    shaderLocs.insert(pair<string, GLint>(name,ret));
    cout << "Its location is " << ret << endl;
    return ret;
  }
  return tmp->second;
}

void Shader::initVars(){
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
  
  //objMatId=getId("gObjMat");
  
  colorId=getId("color");
  
  animatedId=getId("animated");

  for(int i=0;i<MAX_BONES;i++){
    boneLocation[i] = getId("gBones", i);
  }

}

void Shader::deleteShading(){
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

void Shader::setMatrix(const Matrix& mat, string name, int ind)
{
  GLint loc = getVarLocation(name, ind);
  if(loc!=0xFFFFFFFF){
    glUniformMatrix4fv(loc, 1, GL_TRUE, (const GLfloat*)mat.m);
  }else{
    //cout << __func__ << " Error: " << name << " not found" << endl;
  }
}

Matrix Shader::getMatrix(string name, int ind)
{
}

void Shader::setVector3(const Vector3& vec, string name, int ind)
{
  GLint loc = getVarLocation(name, ind);
  if(loc!=0xFFFFFFFF){
    glUniform3f(colorId, vec.x, vec.y, vec.z );
  }else{
    //cout << __func__ << " Error: " << name << " not found" << endl;
  }
}

Vector3 Shader::getVector3(string name, int ind)
{

}

void Shader::setVector2(const Vector2& vec, string name, int ind)
{
  GLint loc = getVarLocation(name, ind);
  if(loc!=0xFFFFFFFF){
    glUniform2f(loc, vec.x, vec.y);
  }else{
    //cout << __func__ << " Error: " << name << " not found" << endl;
  }
}

Vector2 Shader::getVector2(string name, int ind)
{

}

void Shader::setInt(unsigned int value, string name, int ind)
{
  GLint loc = getVarLocation(name, ind);
  if(loc!=0xFFFFFFFF){
    glUniform1i(loc, value);
  }else{
    //cout << __func__ << " Error: " << name << " not found" << endl;
  }
}

unsigned int Shader::getInt(string name, int ind)
{

}

void Shader::setBool(bool value, string name, int ind)
{
  GLint loc = getVarLocation(name, ind);
  if(loc!=0xFFFFFFFF){
    glUniform1i(loc, value);
  }else{
    //cout << __func__ << " Error: " << name << " not found" << endl;
  }
}

bool Shader::getBool(string name, int ind)
{

}

void Shader::useProgram(){
  glUseProgram(program);
}

GLuint Shader::getProgramInd(){
  return program;
}