/*********************************************************************
 * charanim - Character Animation Project
 * Copyright (C) 2018 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
