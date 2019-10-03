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

#pragma once

// C++ includes
#include <vector>

// physim includes
#include <physim/particles/agent_particle.hpp>
#include <physim/math/vec3.hpp>

namespace charanim {
/// Variables and functions for simulations 10x
namespace sim_1xx {

	extern physim::math::vec3 sim_1xx_ini_pos;
	extern physim::math::vec3 sim_1xx_ini_vel;
	extern physim::math::vec3 sim_1xx_target;

	extern float sim_1xx_mass;

	extern float sim_1xx_max_speed;
	extern float sim_1xx_max_force;
	extern float sim_1xx_alignment_weight;
	extern float sim_1xx_seek_weight;
	extern float sim_1xx_flee_weight;
	extern float sim_1xx_arrival_weight;
	extern float sim_1xx_arrival_distance;
	extern float sim_1xx_coll_weight;
	extern float sim_1xx_coll_distance;
	extern float sim_1xx_ucoll_weight;
	extern float sim_1xx_ucoll_distance;
	extern float sim_1xx_wwm_weight;
	extern float sim_1xx_wwm_distance;

	extern size_t sim_1xx_path_it;
	extern std::vector<physim::math::vec3> sim_1xx_path;

	void print_1xx_info(const physim::particles::agent_particle& a);

} // -- namespace sim_1xx
} // -- namespace charanim
