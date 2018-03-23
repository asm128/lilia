#include "llc_typeint.h"	// for int64_t

#ifndef LLC_TIMER_H_29837409823740928987652132
#define LLC_TIMER_H_29837409823740928987652132

namespace llc	// 
{
	struct STimer {
		double						LastTimeSeconds					= 0;	// This variable is set by Frame() or Reset() and holds the time difference in seconds between a Frame() call and the next.
		uint64_t					LastTimeMicroseconds			= 0;	// This variable is set by Frame() or Reset() and holds the time difference in seconds between a Frame() call and the next.
		
									STimer							()						{ Reset(); }		// STimer constructor. This function/method executes when we create an instance of STimer without us requiring to call the function directly. It's used to set the initial values of the struct.
		// --
		void						Reset							();	// Reset timer
		void						Frame							();	// Calculate time elapsed since the last Frame() or Reset() call.
	
	private:
		int64_t						CountsPerSecond					= 0;	
		int64_t						PrevTimeStamp					= 0;
		int64_t						CurrentTimeStamp				= 0;
		double						SecondsPerCount					= 0;
		double						MicrosecondsPerCount			= 0;
	};
}

#endif // LLC_TIMER_H_29837409823740928987652132
