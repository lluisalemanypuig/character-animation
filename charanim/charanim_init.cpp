#include "charanim.hpp"

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// custom includes
#include <charanim/render/include_gl.hpp>

namespace charanim {

void print_usage() {

}

int initialise_animation(int argc, char *argv[]) {
	width = 640;
	height = 480;

	special_key = 0;
	pressed_button = 0;

	last_mouse_moved = point(0,0);
	last_mouse_click = point(0,0);
	special_key_pressed = point(0,0);
	regular_key_pressed = point(0,0);

	bgd_color = glm::vec3(0.8f,0.8f,0.8f);

	FPS = 60;
	fps_count = 0;
	display_fps = false;
	sec = timing::now();

	// parse arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 2;
		}
	}

	// initialise GLUT window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	window_id = glutCreateWindow("Spring meshes - Simulation 3");

	GLenum err = glewInit();
	if (err != 0) {
		cerr << "charanim::initialise_animation - Error:" << endl;
		cerr << "    when initialising glew: " << err << endl;
		return 1;
	}

	V.get_box().set_min_max(glm::vec3(-10,-10,-10), glm::vec3(10,10,10));
	V.set_window_dims(width, height);
	V.init_cameras();

	return 0;
}

} // -- namespace charanim
