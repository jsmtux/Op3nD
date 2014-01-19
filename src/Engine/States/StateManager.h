#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include "State.h"
#include <stack>

class StateManager{
  
public:
  StateManager();
  /**
    * Creates a new state, note that it must be initialised before running it with beginState
    * @param name name of the new state
    * @return returns the id of the state created
    */
  int newState( State* newSt );
  /**
    * Deletes state with name name
    * @param name of the state it is going to delete
    */
  void deleteState();
  /**
    * returns a pointer to the current state
    * @return 
    */
  State* getCurState();
  
  void clearStates();
  
  stack<State*> getStates(){return states;}//TODO: Delete
  /**
   * Iterates current state and swaps it if needed
   * Returns if state ended
   */
  bool iteration();
private:
  void beginState();
  stack<State*> states;
  NetNode *netNode;
  mutex changeStateMt;
};

#endif