#include <anim/utils/utils.hpp>

// C includes
#include <string.h>

namespace parsing {

	bool is_help(char *str) {
		bool help = false;
		help = help or (strcmp(str, "-h") == 0);
		help = help or (strcmp(str, "--he") == 0);
		help = help or (strcmp(str, "--hel") == 0);
		help = help or (strcmp(str, "--help") == 0);
		return help;
	}

} // -- namespace parsing

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
