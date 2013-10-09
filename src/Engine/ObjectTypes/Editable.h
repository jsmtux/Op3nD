/* 
 * File:   Editable.h
 * Author: jsmtux
 *
 * Created on August 21, 2012, 2:35 PM
 */

#ifndef EDITABLE_H
#define	EDITABLE_H

#include "../../Network/Networkable.h"

typedef enum {E_TILE,E_OBJECT,E_SCRIPTED,E_CAMERA}edType;

class Vector3;
class Quaternion;

class Editable: public Networkable
{
public:
    virtual Vector3 getPos()=0;
    virtual Vector3 getSize()=0;
    virtual Quaternion getRot()=0;
    virtual void setPos(Vector3 p)=0;
    virtual void setRot(Quaternion s)=0;
    virtual void setSize(Vector3 r)=0;
    virtual void draw()=0;
    /**
     * return type of editable entity
     * @return edType according to type
     */
    virtual edType getType()=0;
    void setParentId(unsigned int p);
    unsigned int getId();
    virtual void setId(unsigned int i);
protected:
    Editable(unsigned int i);
    Editable();
    unsigned int id;
    unsigned int pId;
};

#endif	/* EDITABLE_H */

