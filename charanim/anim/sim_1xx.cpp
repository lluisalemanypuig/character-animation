#include <anim/sim_1xx.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/particles/agent_particle.hpp>
using namespace physim::particles;
using namespace physim::math;

namespace charanim {
/// Variables for simulations 1xx
namespace sim_1xx {

	// only agent in the simulation
	agent_particle *sim_1xx_agent;

	vec3 sim_1xx_ini_pos;
	vec3 sim_1xx_ini_vel;
	vec3 sim_1xx_target;

	float sim_1xx_mass;

	float sim_1xx_max_speed;
	float sim_1xx_max_force;
	float sim_1xx_seek_weight;
	float sim_1xx_flee_weight;
	float sim_1xx_arrival_weight;
	float sim_1xx_slowing_distance;
	float sim_1xx_coll_avoid_weight;
	float sim_1xx_ahead_distance;

	vector<vec3> sim_1xx_path;
	size_t sim_1xx_path_it;

#define has_behaviour(behav) sim_1xx_agent->is_behaviour_set(behav)

	void print_1xx_info() {
		bool has_seek = has_behaviour(agent_behaviour_type::seek);
		bool has_flee = has_behaviour(agent_behaviour_type::flee);
		bool has_arrival = has_behaviour(agent_behaviour_type::arrival);
		bool has_coll_avoid = has_behaviour(agent_behaviour_type::collision_avoidance);

		cout << "Environment information: " << endl;
		cout << "    Target position: (" << sim_1xx_target.x << ","
											 << sim_1xx_target.y << ","
											 << sim_1xx_target.z << ")"
											 << endl;
		cout << "    Agent's initial position: ("
											 << sim_1xx_ini_pos.x << ","
											 << sim_1xx_ini_pos.y << ","
											 << sim_1xx_ini_pos.z << ")"
											 << endl;
		cout << "    Agent's initial velocity: ("
											 << sim_1xx_ini_vel.x << ","
											 << sim_1xx_ini_vel.y << ","
											 << sim_1xx_ini_vel.z << ")"
											 << endl;
		cout << "Behaviour parameters:" << endl;
		cout << "    max speed:      " << sim_1xx_max_speed << endl;
		cout << "    max force:      " << sim_1xx_max_force << endl;
		cout << "    agent's weight: " << sim_1xx_mass << endl;
		cout << "    Behaviour: " << endl;

		cout << "        Seek? " << (has_seek ? "Yes" : "No") << endl;
		if (has_seek) {
			cout
			 << "            seek weight: " << sim_1xx_seek_weight << endl;
		}

		cout << "        Flee? " << (has_flee ? "Yes" : "No") << endl;
		if (has_flee) {
			cout
			 << "            flee weight: " << sim_1xx_flee_weight << endl;
		}

		cout << "        Arrival? " << (has_arrival ? "Yes" : "No") << endl;
		if (has_arrival) {
			cout
			 << "            arrival weight:    " << sim_1xx_arrival_weight << endl
			 << "            slowing distance:  " << sim_1xx_slowing_distance << endl;
		}

		cout << "        Collision avoidance? " << (has_coll_avoid ? "Yes" : "No") << endl;
		if (has_coll_avoid) {
			cout << "            collision avoidance weight: "
				 << sim_1xx_coll_avoid_weight << endl;
			cout << "            ahead distance: "
				 << sim_1xx_ahead_distance << endl;
		}
	}

} // -- namespace sim_1xx
} // -- namespace charanim
