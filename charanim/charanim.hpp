#pragma once

// glm includes
#include <glm/vec3.hpp>

// charanim includes
#include <charanim/utils.hpp>
#include <charanim/render/viewer.hpp>

typedef std::pair<int,int> point;

namespace charanim {

	/* --------- */
	/* VARIABLES */

	/* glut window */
	extern int window_id;
	extern int width;
	extern int height;

	extern int special_key;
	extern int pressed_button;

	extern point last_mouse_click;
	extern point last_mouse_moved;
	extern point special_key_pressed;
	extern point regular_key_pressed;

	extern glm::vec3 bgd_color;

	/* animation control variables */
	extern int FPS;
	extern int fps_count;
	extern bool display_fps;
	extern timing::time_point sec;

	/* view control */
	extern viewer V;

	/* --------- */
	/* FUNCTIONS */

	/* initialise animation (load models, ...) */

	/// Returns 0 on success, 1 on error, 2 on '--help'
	int initialise_animation(int argc, char *argv[]);

	/* rendering functions */
	void refresh();
	void timed_refresh(int v);
	void exit_func();
	void special_keys_keyboard(int key, int x, int y);
	void regular_keys_keyboard(unsigned char c, int x, int y);
	void mouse_passive(int x, int y);
	void mouse_drag(int x, int y);
	void mouse_click(int b, int s, int x, int y);
	void resize(int w, int h);

} // -- namespace charanim
