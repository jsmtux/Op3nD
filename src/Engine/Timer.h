/**
 * @(#) Timer.h
 */

#ifndef TIMER_H_H
#define TIMER_H_H
#include <chrono>

typedef std::chrono::milliseconds tMillis;
#define TIME_TO_SECONDS(X) std::chrono::duration_cast<std::chrono::duration<float>>(X).count()

class Timer
{
private:    
        std::chrono::system_clock::time_point clock;
public:
	tMillis getTicks( );
	void reset( );
	
};

#endif
