#include "Quaternion.h"
#include <math.h>
#include "../../ProjectManagement/MXML.h"
#include "Matrix.h"

Quaternion::Quaternion(float _x, float _y, float _z, float _w){
    x=_x;
    y=_y;
    z=_z;
    w=_w;
}

Quaternion::Quaternion(btQuaternion bt){
    x=bt.getX();
    y=bt.getY();
    z=bt.getZ();
    w=bt.getW();
}

Quaternion::Quaternion(Vector3 angle){
    fromEuler(angle);
}

Quaternion::Quaternion(MXML::Tag &code){
    fromXML(code);
}

Quaternion Quaternion::normalized(){
    Quaternion ret;
    
    float Length = sqrt(x * x + y * y + z * z + w * w);
    
    ret.x = x / Length;
    ret.y = y / Length;
    ret.z = z / Length;
    ret.w = w / Length;
    
    
    return ret;
}

Quaternion Quaternion::conjugated(){
    return Quaternion(-x,-y,-z,w);
}


Quaternion Quaternion::operator*(const Quaternion& r)
{
    const float _w = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
    const float _x = (w * r.x) + (x * r.w) + (y * r.z) - (z * r.y);
    const float _y = (w * r.y) - (x * r.z) + (y * r.w) + (z * r.x);
    const float _z = (w * r.z) + (x * r.y) - (y * r.x) + (z * r.w);
    
    return Quaternion(_x, _y, _z, _w);
}

Quaternion Quaternion::angleToQuaternion(Vector3 axis,float a){
    Quaternion ret;
    float angle=sinf(a/2.0);
    ret.x=axis.x*angle;
    ret.y=axis.y*angle;
    ret.z=axis.z*angle;
    ret.w=cosf(a/2.0);
    return ret;
}

Quaternion Quaternion::operator*(const Vector3& v)
{
    const float _w = - (x * v.x) - (y * v.y) - (z * v.z);
    const float _x =   (w * v.x) + (y * v.z) - (z * v.y);
    const float _y =   (w * v.y) + (z * v.x) - (x * v.z);
    const float _z =   (w * v.z) + (x * v.y) - (y * v.x);

    Quaternion ret(_x, _y, _z, _w);

    return ret;
}

using namespace MXML;

MXML::Tag Quaternion::toXML(){
    Tag ret("quaternion");
    ret.addChildren(Tag("x"));
    ret.addChildren(Tag("y"));
    ret.addChildren(Tag("z"));
    ret.addChildren(Tag("w"));
    ret["x"].setAttrib(Attribute(x));
    ret["y"].setAttrib(Attribute(y));
    ret["z"].setAttrib(Attribute(z));
    ret["w"].setAttrib(Attribute(w));
    return ret;
}

void Quaternion::fromXML(MXML::Tag &code){
    x=code["x"].getAttrib().getFloat();
    y=code["y"].getAttrib().getFloat();
    z=code["z"].getAttrib().getFloat();
    w=code["w"].getAttrib().getFloat();
}


ostream& operator<<(ostream& os, const Quaternion& v){
    os << "("<< v.x << "," << v.y << "," << v.z << "," << v.w << ")";
    return os;    
}

Matrix Quaternion::toRotationMatrix(){
    Matrix ret;
    ret.m[0][0]=1.0f - 2.0f*y*y - 2.0f*z*z; ret.m[0][1] =2.0f*x*y - 2.0f*z*w;ret.m[0][2]= 2.0f*x*z + 2.0f*y*w;ret.m[0][3]= 0.0f;
    ret.m[1][0]=2.0f*x*y + 2.0f*z*w;ret.m[1][1]= 1.0f - 2.0f*x*x - 2.0f*z*z; ret.m[1][2]= 2.0f*y*z - 2.0f*x*w; ret.m[1][3]= 0.0f;
    ret.m[2][0]=2.0f*x*z - 2.0f*y*w; ret.m[2][1]= 2.0f*y*z + 2.0f*x*w; ret.m[2][2]= 1.0f - 2.0f*x*x - 2.0f*y*y; ret.m[2][3]= 0.0f;
    return ret;
}

bool Quaternion::operator!=(const Quaternion& r){
    return x!=r.x||y!=r.y||z!=r.z||w!=r.w;
}

Quaternion Quaternion::fromAngleMagnitude(float m, float i, float j, float k){
    Quaternion ret;
    ret.w  = cosf( m/2);
    ret.x = i * sinf( m/2 );
    ret.y = j * sinf( m/2 );
    ret.z = k * sinf( m/2 );
    
    return ret.normalized();
}

Vector3 Quaternion::toEuler(){
    Vector3 ret;
    ret.x= atan2(2*((w * x) + (y * z)), 1 - (2 * ((x* x) + (y * y))));
    ret.y= asin(2 * ((w * y) - (z * x)));
    ret.z= atan2(2 * ((w * z) + (x * y)), 1 - (2 * ((y * y) + (z * z))));
    ret = ret *180 / M_PI;
    return ret;
}

void Quaternion::fromEuler(Vector3 angle){
    angle = angle * M_PI/180;
    float cos_z_2 = cosf(0.5*angle.z);
    float cos_y_2 = cosf(0.5*angle.y);
    float cos_x_2 = cosf(0.5*angle.x);

    float sin_z_2 = sinf(0.5*angle.z);
    float sin_y_2 = sinf(0.5*angle.y);
    float sin_x_2 = sinf(0.5*angle.x);

    // and now compute quaternion
    w   = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
    x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
    y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
    z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
}