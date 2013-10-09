/* 
 * File:   MeshState.h
 * Author: jsmtux
 *
 * Created on November 8, 2012, 12:33 PM
 */

#ifndef MESHSTATE_H
#define	MESHSTATE_H
#include "State.h"
#include <string>
using namespace std;

class MeshState: public State{
public:
    MeshState(string name);
    void iteration();
    StateType getType(){return MESHST;}
};

#endif	/* MESHSTATE_H */

