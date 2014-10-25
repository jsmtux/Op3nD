/**
 * @(#) Camera.cpp
 */


#include "Camera.h"
#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Tile.h"
#include "Graphics/Renderer.h"
#include "Base.h"


Camera::Camera(State* state, float ratio, float fov, float zNear, float zFar):Editable(state), visible(state, Vector3(0,0,0),Vector3(1,1,1),Quaternion(0,0,0,1),"*camera.3ds"){
    visible.setParentId(id);
    step=0.05;
    reloadMatrix();
    
    target=Vector3(0,0,2);
    up=Vector3(0,1,0);
    pos=(0,0,-3);
    angleH=275;
    angleV=0;
    targetDefined=false;
}

Camera::Camera(State* state, MXML::Tag &code): Editable(state), visible(state, Vector3(0,0,0),Vector3(1,1,1),Quaternion(0,0,0,1),"*camera.3ds"){
    visible.setParentId(id);
    step=0.05;
    
    reloadMatrix();
    ResourceManager *resourceManager;
    
    target=Vector3(0,0,2);
    up=Vector3(0,1,0);
    pos=(0,0,-3);
    angleH=275;
    angleV=0;
    fromXML(code);
    targetDefined=false;
}

void Camera::reloadMatrix()
{
  float zNear=0.01, zFar=1000, fov=30;
  step=0.05;
  const float zRange     = zNear - zFar;
  const float tanHalfFOV = tanf(ToRadian(fov / 2.0f));
  Matrix m;
  m.m[0][0] = 1.0f/(tanHalfFOV * Base::getInstance()->getRC()->getRatio()); m.m[0][1] = 0.0f;            m.m[0][2] = 0.0f;                   m.m[0][3] = 0.0;
  m.m[1][0] = 0.0f;                   m.m[1][1] = 1.0f/tanHalfFOV; m.m[1][2] = 0.0f;                   m.m[1][3] = 0.0;
  m.m[2][0] = 0.0f;                   m.m[2][1] = 0.0f;            m.m[2][2] = (-zNear -zFar)/zRange ; m.m[2][3] = 2.0f * zFar*zNear/zRange;
  m.m[3][0] = 0.0f;                   m.m[3][1] = 0.0f;            m.m[3][2] = 1.0f;                   m.m[3][3] = 0.0;
  
  perspMat=m;
  
  m=Matrix();
  
  int top=800;
  int bottom=0;
  int right=600;
  int left=0;
  m.m[0][0] = 200.0f / (right - left);
  m.m[1][1] = 200.0f / (top - bottom);
  m.m[2][2] = - 1.0f / (zFar - zNear);
  m.m[2][3] = -zNear /(zFar-zNear);

  orthoMat=m;
}


void Camera::view(Renderer *renderer){
  Matrix camMat;
  N = target.normalized();
  U = up.normalized();
  U = U.cross(N);
  V = N.cross(U);

  camMat.m[0][0] = U.x; camMat.m[0][1] = U.y; camMat.m[0][2] = U.z; camMat.m[0][3] = 0.0f;
  camMat.m[1][0] = V.x; camMat.m[1][1] = V.y; camMat.m[1][2] = V.z; camMat.m[1][3] = 0.0f;
  camMat.m[2][0] = N.x; camMat.m[2][1] = N.y; camMat.m[2][2] = N.z; camMat.m[2][3] = 0.0f;
  camMat.m[3][0] = 0.0f; camMat.m[3][1] = 0.0f; camMat.m[3][2] = 0.0f; camMat.m[3][3] = 1.0f;
#ifndef NODRAW
  renderer->setWVP(perspMat*camMat*(-pos).toPositionMatrix());
#endif
}

void Camera::setId(unsigned int id){
    Editable::setId(id);
    visible.setParentId(id);
}

void Camera::orthoView(Renderer *renderer){
    Matrix camMat;
    
    N = target.normalized();
    U = up.normalized();
    U = U.cross(N);
    V = N.cross(U);
    

    camMat.m[0][0] = U.x; camMat.m[0][1] = U.y; camMat.m[0][2] = U.z; camMat.m[0][3] = 0.0f;
    camMat.m[1][0] = V.x; camMat.m[1][1] = V.y; camMat.m[1][2] = V.z; camMat.m[1][3] = 0.0f;
    camMat.m[2][0] = N.x; camMat.m[2][1] = N.y; camMat.m[2][2] = N.z; camMat.m[2][3] = 0.0f;
    camMat.m[3][0] = 0.0f; camMat.m[3][1] = 0.0f; camMat.m[3][2] = 0.0f; camMat.m[3][3] = 1.0f;
#ifndef NODRAW
	renderer->setWVP(orthoMat*camMat*(-pos).toPositionMatrix());
#endif
}

Vector3 Camera::getTarget(){
    return target;
}

void Camera::setTarget(Vector3 t){
    direction=t;
    targetDefined=true;
}

Vector3 Camera::getU(){
    return U;
}

Vector3 Camera::getV(){
    return V;
}

Vector3 Camera::getN(){
    return N;
}
void Camera::setPosition(Vector3 e, Vector3 o, Vector3 u ){
    pos=e;
    /*target=o;
    up=u;*/
}

void Camera::move(Vector3 v){
    if(v.x!=0)pos=pos+target*step*v.x;
    if(v.y!=0)pos=pos+(target.cross(up).normalized())*step*v.y;
    if(v.z!=0)pos=pos+up*step*v.z;
}

MXML::Tag Camera::difference(Networkable &n){
    return MXML::Tag("");
}

Quaternion rotationBtwVectors(Vector3 u, Vector3 v){
    u = u.normalized();
    v = v.normalized();
    
    if(u==-v){
        return Quaternion(0,1,0,0);
    }
    
    Vector3 half = (u+v).normalized();
    Vector3 vec = u.cross(half);
    float w = u*half;
    return Quaternion(vec.x,vec.y,vec.z,w);
}

void Camera::orientate(Vector3 v){
    if(targetDefined){
        Vector3 difference=(direction-pos);
        angleH=atan(difference.z/-difference.x);
	float radH=angleH;
        angleH=((angleH*180)/M_PI);
        if(difference.x<0)angleH+=180;
	
        angleV=atan((difference.z*-sin(radH)+difference.x*cos(radH))/difference.y);
        angleV=((angleV*180)/M_PI)+90;
    }else{
        angleH+=v.x;
        angleV+=v.y;        
    }
    Vector3 Vaxis(0.0f, 1.0f, 0.0f);    
    Vector3 View(1.f, 0.0f, 0.0f);

    View=View.rotated(angleH, Vaxis).normalized();

    Vector3 Haxis = Vaxis.cross(View);
    View= View.rotated(angleV, Haxis.normalized()).normalized();

    target = View.normalized();

    up = target.cross(Haxis).normalized();
}

using namespace MXML;

MXML::Tag Camera::toXML(){
    Tag ret("camera");
    ret.addChildren(Tag("position"));
    ret.addChildren(Tag("target"));
    ret.addChildren(Tag("up"));
    
    ret["position"].addChildren(pos.toXML());
    ret["target"].addChildren(target.toXML());
    ret["up"].addChildren(up.toXML());
    return ret;
}

void Camera::fromXML(MXML::Tag &code){
    pos = Vector3(code["position"]["vector"]);
    target = Vector3(code["target"]["vector"])+pos;
    up = Vector3(code["up"]["vector"]);
}

void Camera::notify(){
    
}

Vector3 Camera::getPos(){
    return pos;
}

Vector3 Camera::getSize(){
    return Vector3(1,1,1);
}

Quaternion Camera::getRot(){
    return Quaternion(0,0,0,1);
}

void Camera::draw(Shader* shader){
    visible.setPos(pos);
    visible.draw(shader);
}

edType Camera::getType(){
    return E_CAMERA;
}

void Camera::setPos(Vector3 p){
    pos=p;
}

void Camera::setRot(Quaternion s){
    
}

void Camera::setSize(Vector3 r){
    
}
