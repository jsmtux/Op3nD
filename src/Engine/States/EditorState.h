/* 
 * File:   EditorState.h
 * Author: jsmtux
 *
 * Created on October 30, 2012, 11:53 AM
 */

#ifndef EDITORSTATE_H
#define	EDITORSTATE_H
#include "State.h"

class Editable;

class EditorState: public State{
public:
    EditorState(string n);
    void saveState();
    void iteration();
    void draw();
    void updateElements();
    StateType getType(){return EDITORST;}
private:    
    Editable * selected;
    Tile* handler;
};

#endif	/* EDITORSTATE_H */

