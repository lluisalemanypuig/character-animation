// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

namespace charanim {
namespace study_cases {

	void sim_000(int argc, char *argv[]);

	void sim_100(int argc, char *argv[]);
	void sim_101(int argc, char *argv[]);
	void sim_102(int argc, char *argv[]);
	void sim_103(int argc, char *argv[]);

	void sim_200(int argc, char *argv[]);

} // -- namespace study_cases
} // -- namespace charanim

void list_all_cases() {
	cout << "Welcome to the particles renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    * 000 : visualise any map passed as parameter." << endl;
	cout << "            Find a path in this map." << endl;
	cout << "    * 100 : Visualise seek steering behaviour." << endl;
	cout << "    * 101 : Visualise flee steering behaviour." << endl;
	cout << "    * 102 : Visualise wander steering behaviour." << endl;
	cout << "    * 103 : Visualise obstacle avoidance and seek steering behaviours."
		<< endl;
	cout << "    * 200 : visualise any map passed as parameter." << endl;
	cout << "            Make an agent particle follow a path." << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: lacking parameter for scene: ./particles case_identifier"
			 << endl;
		cerr << "    Use './anim --list' or" << endl;
		cerr << "        './anim --help' or" << endl;
		cerr << "    to see all scenes available." << endl;
		return 1;
	}

	if (strcmp(argv[1], "--list") == 0 or strcmp(argv[1], "--help") == 0) {
		list_all_cases();
		return 0;
	}

	if (strcmp(argv[1], "000") == 0) {
		charanim::study_cases::sim_000(argc, argv);
	}
	else if (strcmp(argv[1], "100") == 0) {
		charanim::study_cases::sim_100(argc, argv);
	}
	else if (strcmp(argv[1], "101") == 0) {
		charanim::study_cases::sim_101(argc, argv);
	}
	else if (strcmp(argv[1], "102") == 0) {
		charanim::study_cases::sim_102(argc, argv);
	}
	else if (strcmp(argv[1], "103") == 0) {
		charanim::study_cases::sim_103(argc, argv);
	}
	else if (strcmp(argv[1], "200") == 0) {
		charanim::study_cases::sim_200(argc, argv);
	}
	else {
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './anim --list' to see all cases" << endl;
	}

	return 0;
}
