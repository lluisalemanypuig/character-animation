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

#include "utils.hpp"

namespace timing {

	time_point now() {
		return high_resolution_clock::now();
	}

	double elapsed_seconds(const time_point& begin, const time_point& end) {
		return duration<double, seconds::period>( end - begin ).count();
	}

	double elapsed_milliseconds(const time_point& begin, const time_point& end) {
		return duration<double, milliseconds::period>( end - begin ).count();
	}

	double elapsed_microseconds(const time_point& begin, const time_point& end) {
		return duration<double, microseconds::period>( end - begin ).count();
	}

	void sleep_seconds(double s) {
		sleep_for(duration<double, seconds::period>(s));
	}

	void sleep_milliseconds(double ms) {
		sleep_for(duration<double, milliseconds::period>(ms));
	}

	void sleep_microseconds(double us) {
		sleep_for(duration<double, microseconds::period>(us));
	}

}
