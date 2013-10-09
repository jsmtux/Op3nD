/**
 * @(#) Controller.h
 */

#ifndef CONTROLLER_H_H
#define CONTROLLER_H_H
#include <vector>
using namespace std;

#include <lua5.2/lua.hpp>
#include "../Network/Networkable.h"

enum tkeys{K_UP,K_DOWN,K_LEFT,K_RIGHT,K_A,K_B,K_X,K_Y,K_Z,K_I,K_K,K_J,K_L,K_R1,K_R2,K_L1,K_L2,K_START,K_END};
enum taxis{A_X,A_Y,A_R,A_L,A_END};

class Controller: public Networkable
{
public:
    Controller();
    const bool* getKeys();
    const float* getAxis();
    const unsigned int* getSelected();
    int getId();
    
    static void init(lua_State* L);
    MXML::Tag toXML();
    MXML::Tag difference(Networkable& n);
    void fromXML(MXML::Tag &code);
protected:
    void setKey(tkeys key,bool value);
    void setAxis(taxis axis,float value);
    void setSelection(unsigned int* sel);
private:
    
    int id;
    bool vkeys[14];
    float vaxis[4];
    unsigned int selected[2];
    static int contNum;
};

#endif
