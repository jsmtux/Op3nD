/* 
 * File:   Box.h
 * Author: jsmtux
 *
 * Created on October 15, 2012, 9:39 AM
 */

#ifndef BOX_H
#define	BOX_H
#include "Resource.h"

class Box:public Resource{
public:
    void Draw();
    ResourceType getType();
    static Box* loadBox();
    void del();
private:
    static Box* stBox;
};

#endif	/* BOX_H */

