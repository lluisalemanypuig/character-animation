#pragma once

// C++ includes
#include <vector>

// glm includes
#include <glm/vec3.hpp>

// physim includes
#include <physim/simulator.hpp>

// charanim includes
#include <render/scene/viewer.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/geometry/rgeometry.hpp>
#include <render/shader/shader.hpp>
#include <anim/utils.hpp>
#include <anim/definitions.hpp>

namespace charanim {

	// copy of input parameters
	extern int _argc;
	extern char **_argv;

	/* --------- */
	/* VARIABLES */

	/* important keys */
	#define ESC 27

	/* physics-based simulation */
	extern physim::simulator S;

	/* glut window */
	extern int window_id;
	extern int width;
	extern int height;

	extern int special_key;
	extern int mouse_button;
	extern int mouse_state;

	extern latticePoint last_mouse_click;
	extern latticePoint last_mouse_moved;
	extern latticePoint special_key_pressed;
	extern latticePoint regular_key_pressed;

	extern glm::vec3 bgd_color;

	/* animation control variables */
	extern unsigned int FPS;
	extern unsigned int fps_count;
	extern bool display_fps;
	extern timing::time_point sec;

	/* RENDERISATION */

	/* view control */
	extern viewer V;
	extern float move_x;
	extern float move_z;
	/* geometry (planes, triangles, rectangles, ...) */
	extern std::vector<rgeom *> geometry;
	/* sphere mesh */
	extern rendered_triangle_mesh *sphere;

	/* shaders */
	extern shader flat_shader;
	extern shader material_shader;
	extern shader texture_shader;

	/* render control */
	extern bool draw_base_spheres;

	/* --------- */
	/* FUNCTIONS */

	/* initialise animation (load models, ...) */

	/// Returns 0 on success, 1 on error, 2 on '--help'
	int init_anim(bool init_window);
	/// Returns 0 on success, 1 on error, 2 on '--help'
	int initialise_animation(int argc, char *argv[]);

	/* rendering functions */
	void base_render();
	void full_render();
	void timed_refresh(int v);
	void exit_func();
	void special_keys_keyboard(int key, int x, int y);
	void regular_keys_keyboard(unsigned char c, int x, int y);
	void mouse_passive(int x, int y);
	void mouse_drag(int x, int y);
	void mouse_click(int b, int s, int x, int y);
	void resize(int w, int h);

} // -- namespace charanim
