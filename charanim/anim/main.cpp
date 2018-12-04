// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include <anim/study_cases.hpp>

void list_all_cases() {
	cout << "Welcome to the particles renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    * 00 : visualise any map passed as parameter." << endl;
	cout << "           Throw some particles in it." << endl;
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

	charanim::study_cases::choose_case(argc, argv);
	return 0;
}
