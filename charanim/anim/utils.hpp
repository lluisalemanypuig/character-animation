#pragma once

// C++ includes
#include <chrono>
#include <thread>
using namespace std::chrono;
using namespace std::this_thread;

namespace timing {

	typedef high_resolution_clock::time_point time_point;

	// Returns the current time
	time_point now();

	// Returns the elapsed time between 'begin' and 'end' in seconds
	double elapsed_seconds(const time_point& begin, const time_point& end);

	// Returns the elapsed time between 'begin' and 'end' in microseconds
	double elapsed_milliseconds(const time_point& begin, const time_point& end);

	// Returns the elapsed time between 'begin' and 'end' in microseconds
	double elapsed_microseconds(const time_point& begin, const time_point& end);

	// This thread will pause for 's' seconds.
	void sleep_seconds(double s);

	// This thread will pause for 's' milliseconds.
	void sleep_milliseconds(double ms);

	// This thread will pause for 's' microseconds.
	void sleep_microseconds(double us);

} // -- namespace timing
