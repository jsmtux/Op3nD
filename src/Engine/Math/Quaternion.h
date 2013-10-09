/* 
 * File:   Quaternion.h
 * Author: jsmtux
 *
 * Created on September 27, 2012, 2:18 PM
 */

#ifndef QUATERNION_H
#define	QUATERNION_H
#include "Vector3.h"

class Quaternion{
public:
    float x,y,z,w;
    Quaternion(float _x=0, float _y=0, float _z=0, float _w=0);
    Quaternion(Vector3 angle);
    Quaternion(MXML::Tag &code);
    Quaternion(btQuaternion bt);
    Quaternion normalized();
    Quaternion conjugated();
    Quaternion operator*(const Quaternion& r);
    Quaternion operator*(const Vector3& v);
    bool operator!=(const Quaternion& r);
    friend ostream& operator<<(ostream& os, const Quaternion& v);
    static Quaternion angleToQuaternion(Vector3 axis,float a);
    virtual MXML::Tag toXML();
    virtual void fromXML(MXML::Tag &code);
    operator btQuaternion() const{
        return btQuaternion(x,y,z,w);
    };
    Matrix toRotationMatrix();
    Vector3 toEuler();
    void fromEuler(Vector3 angle);
    static Quaternion fromAngleMagnitude(float i, float j, float k, float m);
};

#endif	/* QUATERNION_H */

