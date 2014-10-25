/* 
 * File:   Shader.h
 * Author: jsmtux
 *
 * Created on September 24, 2012, 10:58 AM
 */

#ifndef SHADING_H
#define SHADING_H
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
class Shader{
public:
    Shader();
    virtual void initShader(string file);
    void useProgram();
    void compileShader();
    GLuint getProgramInd();
    void deleteShading();
    bool setMatrix(const Matrix& mat, string name, int ind = -1);
    Matrix getMatrix(string name, int ind = -1);
    bool setVector3(const Vector3& vec, string name, int ind = -1);
    Vector3 getVector3(string name, int ind = -1);
    bool setVector2(const Vector2& vec, string name, int ind = -1);
    Vector2 getVector2(string name, int ind = -1);
    bool setInt(unsigned int value, string name, int ind = -1);
    unsigned int getUInt(string name, int ind = -1);
    bool setUInt(unsigned int value, string name, int ind = -1);
    unsigned int getInt(string name, int ind = -1);
    bool setBool(bool value, string name, int ind = -1);
    bool getBool(string name, int ind = -1);
    void update(const Matrix& WVP, const Matrix& worldMat);
    GLint getVarLocation(string name, int index=-1);
    
    void setObjMat(Matrix obj);
private:
    void addShader(string shader, GLenum shadertype);
    GLint getId(string name, int index=-1);
    virtual void initVars();

    map<string, GLint> shaderLocs;
    
    GLuint wvpId, worldMatId;
    GLint program;
    GLuint boneLocation[MAX_BONES];
};

#endif	/* SHADING_H */

