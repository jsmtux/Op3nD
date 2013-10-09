/* 
 * File:   PlayState.h
 * Author: jsmtux
 *
 * Created on November 21, 2012, 10:43 AM
 */

#ifndef PLAYSTATE_H
#define	PLAYSTATE_H
#include "State.h"

class PlayState: public State{
public:
    PlayState(string n);
    PlayState(State *s);
    StateType getType(){return GAMEST;}
    void updateElements();
    void iteration();
    void draw();
};
#endif	/* PLAYSTATE_H */

