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
	extern float sim_1xx_slowing_distance;
	extern float sim_1xx_coll_weight;
	extern float sim_1xx_collision_distance;
	extern float sim_1xx_ucoll_weight;
	extern float sim_1xx_ucollision_distance;

	extern size_t sim_1xx_path_it;
	extern std::vector<physim::math::vec3> sim_1xx_path;

	void print_1xx_info(const physim::particles::agent_particle& a);

} // -- namespace sim_1xx
} // -- namespace charanim
