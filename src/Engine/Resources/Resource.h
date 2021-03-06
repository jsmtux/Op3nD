/* 
 * File:   Resource.h
 * Author: jsmtux
 *
 * Created on August 20, 2012, 12:48 PM
 */

#ifndef RESOURCE_H
#define	RESOURCE_H

#include "Math/Matrix.h"
#include "Graphics/Shader.h"
#include <string>
#include <map>
using namespace std;

class Resource{
public:
  enum ResourceType{BOX,IMAGE,LIGHT,LINE,MODEL3D,FONT,TEXT,ANIMATION};
  Resource(Resource &r)=delete;
  virtual void Draw(Shader* shader)=0; 
  virtual ResourceType getType()=0;
  string getName();
  virtual Matrix getTransform();
  string typeToText(ResourceType type);
  virtual string getDefaultShader()=0;
protected:
  Resource(string name);
private:
  string name;
};


#endif	/* RESOURCE_H */

