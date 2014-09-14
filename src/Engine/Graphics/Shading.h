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

#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"

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
    void deleteShading();
    static Shading* getActive();
    void setMatrix(const Matrix &mat, string name, int ind = -1);
    Matrix getMatrix(string name, int ind = -1);
    void setVector3(const Vector3 &vec, string name, int ind = -1);
    Vector3 getVector3(string name, int ind = -1);
    void setVector2(const Vector2 &vec, string name, int ind = -1);
    Vector2 getVector2(string name, int ind = -1);
    void setInt(unsigned int value, string name, int ind = -1);
    unsigned int getInt(string name, int ind = -1);
    void setBool(bool value, string name, int ind = -1);
    bool getBool(string name, int ind = -1);
    void update();
    static void push();
    static void pop();
    GLint getVarLocation(string name, int index=-1);
    
    void setWVP(Matrix w);
    void setWorldPos(Matrix pos);
    void setObjMat(Matrix obj);
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
    GLint program;
    GLuint colorId;
    GLuint boneLocation[MAX_BONES];
    GLuint posId;
    GLuint texId;
    GLuint normId;
    GLuint samplerId;
};

#endif	/* SHADING_H */

