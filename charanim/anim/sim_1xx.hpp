#pragma once

// C++ includes
#include <vector>

// physim includes
#include <physim/particles/agent_particle.hpp>
#include <physim/math/vec3.hpp>

namespace charanim {
/// Variables and functions for simulations 1xx
namespace sim_1xx {

	// only agent in the simulation
	extern physim::particles::agent_particle *sim_1xx_agent;

	extern physim::math::vec3 sim_1xx_ini_pos;
	extern physim::math::vec3 sim_1xx_ini_vel;
	extern physim::math::vec3 sim_1xx_target;

	extern float sim_1xx_max_speed;
	extern float sim_1xx_max_force;
	extern float sim_1xx_seek_weight;
	extern float sim_1xx_flee_weight;
	extern float sim_1xx_arrival_weight;
	extern float sim_1xx_coll_avoid_weight;
	extern float sim_1xx_mass;
	extern float sim_1xx_slowing_distance;

	extern size_t sim_1xx_path_it;
	extern std::vector<physim::math::vec3> sim_1xx_path;

	void print_1xx_info();

} // -- namespace sim_1xx
} // -- namespace charanim
