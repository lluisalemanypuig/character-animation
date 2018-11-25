// C++ includes
#include <iostream>
using namespace std;

// charanim includes
#include <anim/charanim.hpp>
#include <render/include_gl.hpp>

int main(int argc, char *argv[]) {
	int r = charanim::initialise_animation(argc, argv);
	if (r == 1) {
		cerr << "Error during initialisation." << endl;
		return 1;
	}
	if (r == 2) {
		return 0;
	}

	cout << "Initialisation successful" << endl;

	glutDisplayFunc(charanim::refresh);
	glutReshapeFunc(charanim::resize);
	glutMouseFunc(charanim::mouse_click);
	glutPassiveMotionFunc(charanim::mouse_passive);
	glutMotionFunc(charanim::mouse_drag);
	glutSpecialFunc(charanim::special_keys_keyboard);
	glutKeyboardFunc(charanim::regular_keys_keyboard);

	int fps = charanim::FPS;
	glutTimerFunc(1000.0f/fps, charanim::timed_refresh, 0);

	glutMainLoop();
}
