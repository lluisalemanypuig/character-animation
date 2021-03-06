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

#include <anim/sim_1xx.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/particles/agent_particle.hpp>
using namespace physim::particles;
using namespace physim::math;

namespace charanim {
namespace sim_1xx {

	vec3 sim_1xx_ini_pos;
	vec3 sim_1xx_ini_vel;
	vec3 sim_1xx_target;

	float sim_1xx_mass;

	float sim_1xx_max_speed;
	float sim_1xx_max_force;
	float sim_1xx_alignment_weight;
	float sim_1xx_seek_weight;
	float sim_1xx_flee_weight;
	float sim_1xx_arrival_weight;
	float sim_1xx_arrival_distance;
	float sim_1xx_coll_weight;
	float sim_1xx_coll_distance;
	float sim_1xx_ucoll_weight;
	float sim_1xx_ucoll_distance;
	float sim_1xx_wwm_weight;
	float sim_1xx_wwm_distance;

	vector<vec3> sim_1xx_path;
	size_t sim_1xx_path_it;

#define has_behaviour(behav) a.is_behaviour_set(behav)

	void print_1xx_info(const agent_particle& a) {
		bool has_seek = has_behaviour(agent_behaviour_type::seek);
		bool has_flee = has_behaviour(agent_behaviour_type::flee);
		bool has_arrival = has_behaviour(agent_behaviour_type::arrival);
		bool has_coll_avoid = has_behaviour(agent_behaviour_type::collision_avoidance);
		bool has_ucoll_avoid = has_behaviour(agent_behaviour_type::unaligned_collision_avoidance);
		bool has_wow = has_behaviour(agent_behaviour_type::walk_with_me);

		cout << "Agent " << a.index << " information:" << endl;
		cout << "    Target position: (" << a.target.x << ","
										 << a.target.y << ","
										 << a.target.z << ")"
										 << endl;
		cout << "    Agent's initial position: ("
											 << a.cur_pos.x << ","
											 << a.cur_pos.y << ","
											 << a.cur_pos.z << ")"
											 << endl;
		cout << "    Agent's initial velocity: ("
											 << a.cur_vel.x << ","
											 << a.cur_vel.y << ","
											 << a.cur_vel.z << ")"
											 << endl;
		cout << "    Behaviour parameters:" << endl;
		cout << "        max speed:        " << a.max_speed << endl;
		cout << "        max force:        " << a.max_force << endl;
		cout << "        alignment weight: " << a.align_weight << endl;
		cout << "        agent's weight:   " << a.mass << endl;
		cout << "        Behaviour: " << endl;

		cout << "            Seek? " << (has_seek ? "Yes" : "No") << endl;
		if (has_seek) {
			cout
			 << "                seek weight: " << a.seek_weight << endl;
		}

		cout << "            Flee? " << (has_flee ? "Yes" : "No") << endl;
		if (has_flee) {
			cout
			 << "                flee weight: " << a.flee_weight << endl;
		}

		cout << "            Arrival? " << (has_arrival ? "Yes" : "No") << endl;
		if (has_arrival) {
			cout
			 << "                arrival weight:    " << a.arrival_weight << endl
			 << "                arrival distance:  " << a.arrival_distance << endl;
		}

		cout << "            Collision avoidance? "
			 << (has_coll_avoid ? "Yes" : "No") << endl;
		if (has_coll_avoid) {
			cout << "                collision avoidance weight: "
				 << a.coll_weight << endl;
			cout << "                collision avoidance distance: "
				 << a.coll_distance << endl;
		}

		cout << "            Unaligned collision avoidance? "
			 << (has_ucoll_avoid ? "Yes" : "No") << endl;
		if (has_ucoll_avoid) {
			cout << "                unaligned collision avoidance weight: "
				 << a.ucoll_weight << endl;
			cout << "                unaligned collision avoidance distance: "
				 << a.ucoll_distance << endl;
		}

		cout << "            Walk with me? "
			 << (has_wow ? "Yes" : "No") << endl;
		if (has_wow) {
			cout << "                'walk with me' weight: "
				 << a.wow_weight << endl;
			cout << "                'walk with me' distance: "
				 << a.wow_distance << endl;
		}
	}

} // -- namespace sim_1xx
} // -- namespace charanim
