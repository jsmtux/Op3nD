#include "Vector2.h"
#include "Quaternion.h"

#include "../../Network/Networkable.h"

Vector2 Vector2::operator + ( const Vector2 &b ){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x + b.x;
    temp.y = y + b.y;

    return temp;
}

Vector2 Vector2::operator * ( const Vector2 &b ){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x * b.x;
    temp.y = y * b.y;

    return temp;
}

Vector2 Vector2::operator / ( const Vector2 &b ){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x / b.x;
    temp.y = y / b.y;

    return temp;
}

Vector2 Vector2::operator * ( const float &b ){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x * b;
    temp.y = y * b;

    return temp;
}

Vector2 Vector2::operator / ( const float &b ){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x / b;
    temp.y = y / b;

    return temp;
}

bool Vector2::operator < ( const Vector2 &b ){
    if (x > b.x) return false;
    if (y > b.y) return false;
    return true;
}

bool Vector2::operator > ( const Vector2 &b ){
    if (x < b.x) return false;
    if (y < b.y) return false;
    return true;
}

Vector2 Vector2::operator + ( void ) const{
    return *this;
}

Vector2 Vector2::operator - ( const Vector2 &b ){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x - b.x;
    temp.y = y - b.y;
    return temp;
}

Vector2 Vector2::operator - ( void ) const{
    Vector2 temp(-x, -y);

    return temp;
}

Vector2 Vector2::normalized(){
    const float length=sqrt(x*x+y*y);
    return Vector2(x/length,y/length);
}

void Vector2::add(const Vector2 &b){
    if(fabs(b.x)>fabs(x))x=b.x;
    if(fabs(b.y)>fabs(y))y=b.y;
}

Vector2::Vector2(float X, float Y){
    x=X;
    y=Y;
}

ostream& operator<<(ostream& os, const Vector2& v){
    os << "("<< v.x << "," << v.y << ")";
    return os;
}

Vector2 Vector2::operator+ (const float &b){
    Vector2 temp(0.0f, 0.0f);

    temp.x = x + b;
    temp.y = y + b;

    return temp;
}

Vector2 Vector2::operator- (const float &b){    
    Vector2 temp(0.0f, 0.0f);

    temp.x = x - b;
    temp.y = y - b;

    return temp;
}

using namespace MXML;

MXML::Tag Vector2::toXML(){
    Tag ret("vector");
    ret.addChildren(Tag("x"));
    ret.addChildren(Tag("y"));
    ret["x"].setAttrib(Attribute(x));
    ret["y"].setAttrib(Attribute(y));
    return ret;
}

void Vector2::fromXML(MXML::Tag &code){
    x=code["x"].getAttrib().getFloat();
    y=code["y"].getAttrib().getFloat();
}
