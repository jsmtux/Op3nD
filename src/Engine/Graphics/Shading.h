/* 
 * File:   Shading.h
 * Author: jsmtux
 *
 * Created on September 24, 2012, 10:58 AM
 */

#ifndef SHADING_H
#define	SHADING_H
#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <stack>
#include <map>
#include <iostream>
#include <string>
using namespace std;

#include "../Math/Matrix.h"
#include "../Math/Vector3.h"

#define MAX_BONES 100
/*
struct BaseLight{
    Vector3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;    
};

struct DirectionalLight
{
    BaseLight Base;
    Vector3 Direction;
};

struct Attenuation{
    float constant, linear, exp;
};

struct PointLight{
    BaseLight Base;
    Attenuation Att;
    Vector3 Direction;
};

struct DirectionalLightLocation{
    GLint Color;
    GLint AmbientIntensity;
    GLint Direction;
    GLint DiffuseIntensity;
};
**/
class Shading{
public:
    Shading();
    virtual void initShader(string file);
    void useProgram();
    void compileShader();
    GLuint getProgramInd();
    GLuint getPosLocation();
    GLuint getTexLocation();
    GLuint getNormLocation();
    GLuint getSamplerLocation();
    void deleteShading();
    static Shading* getActive();
    void setWVP(Matrix w);
    void setWorldPos(Matrix pos);
    void setObjMat(Matrix obj);
    Matrix getObjMat();
    void setColor(Vector3 col);
    void setBoneTransform(uint ind, const Matrix& transform);
    void update();
    void setObjInd(unsigned int ind);
    void setMeshInd(unsigned int ind);
    void setAnimated(bool animated);
    GLint getVarLocation(string name, int index=-1);
    static void push();
    static void pop();
private:
    void addShader(string shader, GLenum shadertype);
    GLint getId(string name, int index=-1);
    virtual void initVars();
    
    static Shading* active;
    static vector<GLuint> shaderlist;
    static stack<Shading*> shaderStack;
    map<string, GLint> shaderLocs;
    
    static Matrix WVP, worldMat, objMat;
    
    GLuint wvpId, worldMatId, objMatId, animatedId;
    GLuint objId, meshId;
    GLint program;
    GLuint colorId;
    GLuint boneLocation[MAX_BONES];
    GLuint posId;
    GLuint texId;
    GLuint normId;
    GLuint samplerId;
};

#endif	/* SHADING_H */

