#include "Editable.h"

#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"

#include "../Graphics/Shading.h"

Editable::Editable(){
    id=0;
    pId=0;
}

Editable::Editable(unsigned int i){
    id=i;
    pId=0;
}

unsigned int Editable::getId(){
    if(pId!=0){
        return pId;
    }
    return id;
}

void Editable::setId(unsigned int i){
    if(id!=0){
        cout << "Trying to set id to already set editable\n";
        return;
    }
    if(i==0){
        cout << "Id must be higher than 0\n";
        return;
    }    
    id=i;    
}

void Editable::setParentId(unsigned int p){
    pId= p;
}