#include <anim/charanim.hpp>

namespace charanim {

	int _argc;
	char **_argv;

	/* physics-based simulation */
	physim::simulator S;

	/* glut window */
	int window_id = -1;
	int width;
	int height;

	int special_key;
	int mouse_button;
	int mouse_state;

	latticePoint last_mouse_moved;
	latticePoint last_mouse_click;
	latticePoint special_key_pressed;
	latticePoint regular_key_pressed;

	glm::vec3 bgd_color;

	/* animation control variables */
	unsigned int FPS;
	unsigned int fps_count;
	bool display_fps;
	timing::time_point sec;
	timing::time_point exe_time;

	/* RENDERISATION */

	/* characters */
	vector<rendered_character> characters;

	/* view control */
	viewer V;
	float move_x;
	float move_z;
	/* geometry (planes, triangles, rectangles, ...) */
	std::vector<rgeom *> geometry;
	/* sphere mesh */
	rendered_triangle_mesh *sphere = nullptr;

	/* shaders */
	shader flat_shader;
	shader material_shader;
	shader texture_shader;

	/* render control */
	bool render_base_spheres = false;
	bool render_grid = false;
	bool render_dist_func = false;
	bool render_targets = false;
	bool render_velocity_vector = true;
	bool render_target_vector = true;
	bool render_orientation_vector = true;

} // -- namespace charanim
