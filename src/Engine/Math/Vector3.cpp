#include "Vector3.h"

#include "../../Network/Networkable.h"
#include "Matrix.h"
#include "Quaternion.h"


Vector3::Vector3(btVector3 v){
    x=v.getX();
    y=v.getY();
    z=v.getZ();
}

Vector3::Vector3(MXML::Tag &code){
    fromXML(code);
}

Vector3 Vector3::operator + ( const Vector3 &b ) const{
    Vector3 temp(0.0f, 0.0f, 0.0f);

    temp.x = x + b.x;
    temp.y = y + b.y;
    temp.z = z + b.z;

    return temp;
}

void Vector3::operator += ( const Vector3 &b ){
    x += b.x;
    y += b.y;
    z += b.z;
}

float Vector3::operator * ( const Vector3 &b ) const{
    float temp=0;

    temp += x * b.x;
    temp += y * b.y;
    temp += z * b.z;

    return temp;
}

Vector3 Vector3::operator / ( const Vector3 &b ) const{
    Vector3 temp(0.0f, 0.0f, 0.0f);

    temp.x = x / b.x;
    temp.y = y / b.y;
    temp.z = z / b.z;

    return temp;
}

Vector3 Vector3::cross(const Vector3 &b) const{
    const float _x = y * b.z - z * b.y;
    const float _y = z * b.x - x * b.z;
    const float _z = x * b.y - y * b.x;

    return Vector3(_x, _y, _z);
}

Vector3 Vector3::abs() const{
    return Vector3(fabs(x),fabs(y),fabs(z));
}

Vector3 Vector3::operator * ( const float &b ) const{
    Vector3 temp(0.0f, 0.0f, 0.0f);

    temp.x = x * b;
    temp.y = y * b;
    temp.z = z * b;

    return temp;
}

Vector3 Vector3::operator / ( const float &b ) const{
    Vector3 temp(0.0f, 0.0f, 0.0f);

    temp.x = x / b;
    temp.y = y / b;
    temp.z = z / b;

    return temp;
}

bool Vector3::operator < ( const Vector3 &b ) const{
    if (x > b.x) return false;
    if (y > b.y) return false;
    if (z > b.z) return false;
    return true;
}

bool Vector3::operator > ( const Vector3 &b ) const{
    if (x < b.x) return false;
    if (y < b.y) return false;
    if (z < b.z) return false;
    return true;
}

Vector3 Vector3::operator + ( void ) const{
    return *this;
}

Vector3 Vector3::operator - ( const Vector3 &b ) const{
    Vector3 temp(0.0f, 0.0f);

    temp.x = x - b.x;
    temp.y = y - b.y;
    temp.z = z - b.z;

    return temp;
}

Vector3 Vector3::operator - ( void ) const{
    Vector3 temp(-x, -y, -z);

    return temp;
}

float Vector3::operator[](const int ind) const{
    switch(ind){
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
}

Vector3 Vector3::normalized() const{
    const float length=sqrt(x*x+y*y+z*z);
    return Vector3(x/length,y/length,z/length);
}

void Vector3::add(const Vector3 &b){
    if(fabs(b.x)>fabs(x))x=b.x;
    if(fabs(b.y)>fabs(y))y=b.y;
    if(fabs(b.z)>fabs(z))z=b.z;
}

Vector3::Vector3(float X, float Y, float Z){
    x=X;
    y=Y;
    z=Z;
}

ostream& operator<<(ostream& os, const Vector3& v){
    os << "("<< v.x << "," << v.y << "," << v.z << ")";
    return os;
}

Vector3 Vector3::operator+ (const float &b) const{
    Vector3 temp(0.0f, 0.0f,0.0f);

    temp.x = x + b;
    temp.y = y + b;
    temp.z = z + b;

    return temp;
}

Vector3 Vector3::operator- (const float &b) const{
    Vector3 temp(0.0f, 0.0f,0.0f);

    temp.x = x - b;
    temp.y = y - b;
    temp.z = z - b;

    return temp;
}

using namespace MXML;

MXML::Tag Vector3::toXML() const{
    Tag ret("vector");
    ret.addChildren(Tag("x"));
    ret.addChildren(Tag("y"));
    ret.addChildren(Tag("z"));
    ret["x"].setAttrib(Attribute(x));
    ret["y"].setAttrib(Attribute(y));
    ret["z"].setAttrib(Attribute(z));
    return ret;
}

void Vector3::fromXML(MXML::Tag &code){
    x=code["x"].getAttrib().getFloat();
    y=code["y"].getAttrib().getFloat();
    z=code["z"].getAttrib().getFloat();
}

Vector3 Vector3::rotated(float angle,Vector3 Axe) const{
    Vector3 ret;
    const float SinHalfAngle = sinf(ToRadian(angle/2));
    const float CosHalfAngle = cosf(ToRadian(angle/2));

    const float Rx = Axe.x * SinHalfAngle;
    const float Ry = Axe.y * SinHalfAngle;
    const float Rz = Axe.z * SinHalfAngle;
    const float Rw = CosHalfAngle;
    Quaternion RotationQ(Rx, Ry, Rz, Rw);

    Quaternion W = RotationQ * (*this) *  RotationQ.conjugated();

    ret.x = W.x;
    ret.y = W.y;
    ret.z = W.z;
    return ret;
}

float Vector3::getBiggest() const{
    float tmp=x;
    if(y>tmp)tmp=y;
    if(z>tmp)tmp=z;
    return tmp;
}

Matrix Vector3::toPositionMatrix() const{
    Matrix m;
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
    return m;
}

Matrix Vector3::toScaleMatrix() const{
    Matrix m;
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

bool Vector3::operator==(const Vector3 &b)const {
    if(b.x!=x||b.y!=y||b.z!=z)return false;
    return true;
}

bool Vector3::operator!=(const Vector3 &b) const{
    if(b.x==x&&b.y==y&&b.z==z)return false;
    return true;
}

float Vector3::length() const{
    return sqrt(x*x+y*y+z*z);
}