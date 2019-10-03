/*********************************************************************
 * charanim - Character Animation Project
 * Copyright (C) 2018 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
	bool run = false;
	bool record = false;
	size_t record_counter = 0;

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
