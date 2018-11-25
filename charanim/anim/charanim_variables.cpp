#include <anim/charanim.hpp>

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

	/* animation control variables */
	int FPS;
	int fps_count;
	bool display_fps;
	timing::time_point sec;

	/* RENDERISATION */

	/* view control */
	viewer V;
	std::vector<rgeom *> geometry;
	/* sphere mesh */
	rendered_tri_mesh *sphere;

	/* shaders */
	shader flat_shader;
	shader material_shader;
	shader texture_shader;

	/* render control */
	bool draw_base_spheres;

} // -- namespace charanim
