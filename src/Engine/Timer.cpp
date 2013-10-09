/**
 * @(#) Timer.cpp
 */


#include "Timer.h"
using namespace std::chrono;

void Timer::reset(){
    clock = system_clock::now();
}

tMillis Timer::getTicks(){
    return duration_cast<tMillis>(system_clock::now()-clock);
}