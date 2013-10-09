/**
 * @(#) Vector.h
 */

#ifndef VECTOR_H_H
#define VECTOR_H_H

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
using namespace std;

#include "../../ProjectManagement/MXML.h"
#include <btBulletDynamicsCommon.h>

class Matrix;

class Vector3
{
public:
    Vector3(float X=0., float Y=0., float Z=0.);
    Vector3(MXML::Tag &code);
    float x, y, z;
    Vector3 operator+ (const Vector3 &b) const;
    void operator+= (const Vector3 &b);
    bool operator==(const Vector3 &b) const;
    bool operator!=(const Vector3 &b) const;
    Vector3 operator- (const Vector3 &b) const;
    float operator* (const Vector3 &b) const;
    Vector3 operator/ (const Vector3 &b) const; 
    Vector3 cross(const Vector3 &b) const;
    Vector3 operator+ (const float &b) const;
    Vector3 operator- (const float &b) const;
    Vector3 operator* (const float &b) const;
    Vector3 operator/ (const float &b) const;
    bool operator< (const Vector3 &b) const;
    bool operator> (const Vector3 &b) const;
    Vector3 operator+ () const;
    Vector3 operator- () const;
    float operator[](const int ind) const;
    void add(const Vector3 &b);
    Vector3 normalized() const;
    Vector3 abs() const;
    friend ostream& operator<<(ostream& os, const Vector3& v);
    MXML::Tag toXML() const;
    void fromXML(MXML::Tag &code);
    Vector3 rotated(float angle,Vector3 Axe) const;
    float getBiggest() const;
    operator btVector3() const{
        return btVector3(x,y,z);
    };
    Vector3(btVector3 v);
    Matrix toPositionMatrix() const;
    Matrix toScaleMatrix() const;
    float length() const;
};

#endif
