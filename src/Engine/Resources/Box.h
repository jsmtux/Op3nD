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
    Box(): Resource("box"){}
    void Draw(Shader* shader);
    ResourceType getType();
    static Box* loadBox();
    void del();
    string getDefaultShader(){return "color.sfx";}
private:
    static Box* stBox;
};

#endif	/* BOX_H */

