#include <charanim/charanim.hpp>

namespace charanim {

	physim::simulator S;

	/* glut window */
	int window_id;
	int width;
	int height;

	int special_key;
	int pressed_button;

	point last_mouse_moved;
	point last_mouse_click;
	point special_key_pressed;
	point regular_key_pressed;

	glm::vec3 bgd_color;

	/* view control */
	viewer V;

	/* animation control variables */
	int FPS;
	int fps_count;
	bool display_fps;
	timing::time_point sec;

} // -- namespace charanim
