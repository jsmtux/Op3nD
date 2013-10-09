/**
 * @(#) Networkable.h
 */

#ifndef NETWORKABLE_H_H
#define NETWORKABLE_H_H
#include <string>
#include "../ProjectManagement/MXML.h"

class Networkable
{	
public:
    virtual MXML::Tag toXML()=0;
    virtual void fromXML(MXML::Tag &code)=0;
    virtual MXML::Tag difference(Networkable &n)=0;
};

#endif
