// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

namespace charanim {
namespace study_cases {

	void sim_00(int argc, char *argv[]);
	void sim_01(int argc, char *argv[]);

} // -- namespace study_cases
} // -- namespace charanim

void list_all_cases() {
	cout << "Welcome to the particles renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    * 00 : visualise any map passed as parameter." << endl;
	cout << "           Find a path in this map." << endl;
	cout << "    * 01 : visualise any map passed as parameter." << endl;
	cout << "           Make an agent particle follow a path." << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: lacking parameter for scene: ./particles case_identifier" << endl;
		cerr << "    Use './anim --list' or" << endl;
		cerr << "        './anim --help' or" << endl;
		cerr << "    to see all scenes available." << endl;
		return 1;
	}

	if (strcmp(argv[1], "--list") == 0 or strcmp(argv[1], "--help") == 0) {
		list_all_cases();
		return 0;
	}

	if (strcmp(argv[1], "00") == 0) {
		charanim::study_cases::sim_00(argc, argv);
	}
	else if (strcmp(argv[1], "01") == 0) {
		charanim::study_cases::sim_01(argc, argv);
	}
	else {
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './anim --list' to see all cases" << endl;
	}

	return 0;
}
