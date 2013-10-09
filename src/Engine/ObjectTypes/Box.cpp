#include "Box.h"
#include "../Base.h"
#include "Line.h"

Box* Box::stBox=new Box();

void Box::Draw(){
    /*Vector3 Vertices[8] = {
    Vector3(-0.5f, -0.5f,-0.5f),
    Vector3(0.5f, -0.5f,-0.5f),
    Vector3(0.5f, 0.5f,-0.5f),
    Vector3(-0.5f, 0.5f,-0.5f),
    Vector3(-0.5f, -0.5f, 0.5f),
    Vector3(0.5f, -0.5f, 0.5f),
    Vector3(0.5f, 0.5f, 0.5f),
    Vector3(-0.5f, 0.5f, 0.5f)}; 
    unsigned int Indices[] = { 0,1,1,2,2,3,3,0,
                               4,5,5,6,6,7,7,4,
                               0,4,1,5,2,6,3,7};
    for(int i=0;i<24;i+=2)
        Line::drawLine(Vertices[Indices[i]],Vertices[Indices[i+1]],Vector3(1,1,1));*/
}

string Box::getType(){
    return "Box";
}

Box* Box::loadBox(){
    return stBox;
}

void Box::del(){
    ;
}