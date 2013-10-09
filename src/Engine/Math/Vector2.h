/* 
 * File:   Vector2.h
 * Author: jsmtux
 *
 * Created on September 29, 2012, 12:58 AM
 */

#ifndef VECTOR2_H
#define	VECTOR2_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
using namespace std;
#include "../../ProjectManagement/MXML.h"

class Vector2
{
public:
    Vector2(float X=0., float Y=0.);
    float x, y;
    Vector2 operator+ (const Vector2 &b);
    Vector2 operator- (const Vector2 &b);
    Vector2 operator* (const Vector2 &b);
    Vector2 operator/ (const Vector2 &b);
    Vector2 operator+ (const float &b);
    Vector2 operator- (const float &b);
    Vector2 operator* (const float &b);
    Vector2 operator/ (const float &b);
    bool operator< (const Vector2 &b);
    bool operator> (const Vector2 &b);
    Vector2 operator+ () const;
    Vector2 operator- () const;
    void add(const Vector2 &b);
    Vector2 normalized();
    friend ostream& operator<<(ostream& os, const Vector2& v);
    MXML::Tag toXML();
    void fromXML(MXML::Tag &code);
};

#endif	/* VECTOR2_H */

