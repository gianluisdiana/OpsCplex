#pragma once

#ifdef _DEBUG
	#include <cassert>
	#include <iostream>
	#include <iomanip>
	#include <cstring>
	
	using namespace std;
	
    #define IT_PER_LINE 8	
	#define WIDE_OUTPUT 9
#endif 

#include <sys/resource.h>

namespace emir {

	class timer_t {
	protected:

		double acumulated_;
		rusage start_;

	public:
		timer_t(void);
		virtual ~timer_t(void);

		void start(void);
		void stop(void);
		void reset(void);
		
		double current_time(void) const;

		double seconds(void) const;
	};
	
}
