/* 
 * File:   Resource.h
 * Author: jsmtux
 *
 * Created on August 20, 2012, 12:48 PM
 */

#ifndef RESOURCE_H
#define	RESOURCE_H

#include "../Math/Matrix.h"
#include <string>
#include <map>
using namespace std;

class Resource{
public:
    static Resource *Load(string dir);
    Resource(Resource &r)=delete;
    virtual void Draw(); 
    virtual string getType();
    string getName();
    void free();
    virtual Matrix getTransform();
protected:
    virtual void del()=0;
    Resource();
    string name;
    int copies;
};


#endif	/* RESOURCE_H */

