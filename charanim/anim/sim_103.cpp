#include <anim/charanim.hpp>

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
typedef glm::vec3 gvec3;

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/agent_particle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>
using namespace physim::init;
using namespace physim::particles;
using namespace physim::geometric;
using namespace physim::math;

// render includes
#include <render/geometry/rplane.hpp>

// custom includes
#include <anim/sim_1xx.hpp>

namespace charanim {
using namespace sim_1xx;

namespace study_cases {

	void sim_103_usage() {
		cout << "Simulation 103: validation of collision avoidance behaviour" << endl;
		cout << endl;
		cout << "Specify a map file as parameter to visualise it." << endl;
		cout << "    --help : show the usage." << endl;
		cout << endl;
		cout << "Keyboard keys:" << endl;
		cout << "    h: show the usage." << endl;
		cout << "    r: reset simulation." << endl;
		cout << "    v: render velocity vector" << endl;
		cout << "    a: render attractor vector" << endl;
		cout << endl;
	}

	void sim_103_render() {
		glClearColor(bgd_color.x, bgd_color.y, bgd_color.z, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// no shader for all
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		V.apply_projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		V.apply_view();

		glTranslatef(move_x, 0.0f, move_z);

		render_agent_vectors();

		base_render();

		for (int i = 0; i < 10; ++i) {
			S.simulate_agent_particles();
		}

		if (window_id != -1) {
			glutSwapBuffers();
		}
	}

	void sim_103_timed_refresh(int v) {
		sim_103_render();

		++fps_count;
		timing::time_point here = timing::now();
		double elapsed = timing::elapsed_seconds(sec, here);
		if (elapsed >= 1.0) {
			if (display_fps) {
				cout << "fps= " << fps_count << " (" << FPS << ")" << endl;
			}
			fps_count = 0;
			sec = timing::now();
		}

		glutTimerFunc(1030/FPS, sim_103_timed_refresh, v);
	}

	void sim_103_init_simulation() {
		sim_1xx_agent = nullptr;

		// add agent particles
		sim_1xx_agent = new agent_particle();
		sim_1xx_agent->lifetime = 9999.0f; // immortal agent
		sim_1xx_agent->R = 1.0f;

		sim_1xx_agent->target = sim_1xx_target;

		sim_1xx_agent->cur_pos = sim_1xx_ini_pos;
		sim_1xx_agent->cur_vel = sim_1xx_ini_vel;

		sim_1xx_agent->max_speed = sim_1xx_max_speed;
		sim_1xx_agent->max_force = sim_1xx_max_force;
		sim_1xx_agent->seek_weight = sim_1xx_seek_weight;
		sim_1xx_agent->flee_weight = sim_1xx_flee_weight;
		sim_1xx_agent->arrival_weight = sim_1xx_arrival_weight;

		sim_1xx_agent->mass = sim_1xx_mass;
		sim_1xx_agent->bouncing = 1.0f;
		sim_1xx_agent->friction = 0.0f;

		sim_1xx_agent->unset_all_behaviours();
		sim_1xx_agent->set_behaviour(agent_behaviour_type::arrival);

		S.add_agent_particle(sim_1xx_agent);

		print_1xx_info();

		S.set_time_step(0.103f);
	}

	void sim_103_init_geometry() {
		// add the geometry read from the map

		rplane *rp = new rplane();
		rp->set_points(
			gvec3(-50.0f, -2.0f, -50.0f), gvec3(-50.0f, -2.0f, 50.0f),
			gvec3(50.0f, -2.0f, 50.0f), gvec3(50.0f, -2.0f, -50.0f)
		);
		rp->set_color(0.4f,0.4f,0.4f,0.5f);
		geometry.push_back(rp);

		V.get_box().set_min_max(
			gvec3(-50.0f, -10.0f, -50.0f), gvec3(50.0f, 10.0f, 50.0f)
		);

		V.get_box().make_buffers();

		V.set_window_dims(width, height);
		V.init_cameras();

		V.increment_theta(50.0f);
	}

	int sim_103_parse_arguments(int argc, char *argv[]) {
		string map_file = "none";

		float x,y,z;

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--help") == 0) {
				sim_103_usage();
				return 2;
			}
			else if (strcmp(argv[i], "--target") == 0) {
				x = atof(argv[i + 1]);
				y = atof(argv[i + 2]);
				z = atof(argv[i + 3]);
				sim_1xx_target = vec3(x,y,z);
				i += 3;
			}
			else if (strcmp(argv[i], "--pos") == 0) {
				x = atof(argv[i + 1]);
				y = atof(argv[i + 2]);
				z = atof(argv[i + 3]);
				sim_1xx_ini_pos = vec3(x,y,z);
				i += 3;
			}
			else if (strcmp(argv[i], "--vel") == 0) {
				x = atof(argv[i + 1]);
				y = atof(argv[i + 2]);
				z = atof(argv[i + 3]);
				sim_1xx_ini_vel = vec3(x,y,z);
				i += 3;
			}
			else if (strcmp(argv[i], "--seek-weight") == 0) {
				sim_1xx_seek_weight = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--max-speed") == 0) {
				sim_1xx_max_speed = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--max-force") == 0) {
				sim_1xx_max_force = atof(argv[i + 1]);
				++i;
			}
		}

		return 0;
	}

	void sim_103_exit() {
		exit_func();

		sim_1xx_agent = nullptr;
	}

	int sim_103_init(bool init_window) {
		width = 640;
		height = 480;

		special_key = 0;
		mouse_button = 0;

		last_mouse_moved = latticePoint(0,0);
		last_mouse_click = latticePoint(0,0);
		special_key_pressed = latticePoint(0,0);
		regular_key_pressed = latticePoint(0,0);

		move_x = 0.0f;
		move_z = 0.0f;
		bgd_color = glm::vec3(0.8f,0.8f,0.8f);

		FPS = 60;
		fps_count = 0;
		display_fps = false;
		sec = timing::now();

		render_base_spheres = true;
		render_grid = false;
		render_dist_func = false;
		render_targets = true;
		render_velocity_vector = true;
		render_target_vector = true;

		sim_1xx_ini_pos = vec3(0.0f,0.0f,0.0f);
		sim_1xx_ini_vel = vec3(0.1f, 0.0f, 0.1f);
		sim_1xx_target = vec3(-20.0f, 0.0f, 20.0f);

		float w = 1.0f/4.0f;

		sim_1xx_max_speed = 0.25f;
		sim_1xx_max_force = 103.0f;
		sim_1xx_seek_weight = 0.5f;
		sim_1xx_flee_weight = w;
		sim_1xx_arrival_weight = w;
		sim_1xx_coll_avoid_weight = w;
		sim_1xx_mass = 60.0f;

		/* PARSE ARGUMENTS */
		int arg_parse = sim_103_parse_arguments(_argc, _argv);
		if (arg_parse != 0) {
			return arg_parse;
		}

		/* INITIALISE GLUT WINDOW */
		if (init_window) {
			glutInit(&_argc, _argv);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowSize(width, height);
			window_id = glutCreateWindow("Character animation - Flee steering inspection");

			GLenum err = glewInit();
			if (err != 0) {
				cerr << "charanim::initialise_animation - Error:" << endl;
				cerr << "    when initialising glew: " << err << endl;
				return 1;
			}
		}

		glEnable(GL_DEPTH_TEST);

		sim_103_init_geometry();
		sim_103_init_simulation();

		bool success;
		success = load_shaders();
		if (not success) {
			cerr << "Error: error when loading shaders" << endl;
			return 1;
		}

		success = load_sphere();
		if (not success) {
			cerr << "Error: error when loading sphere" << endl;
			return 1;
		}

		return 0;
	}

	void sim_103_regular_keys_keyboard(unsigned char c, int x, int y) {
		charanim::regular_keys_keyboard(c, x, y);
		switch (c) {
		case 'h': sim_103_usage(); break;
		case 'r': sim_103_exit(); sim_103_init(false); break;
		case 'a': render_target_vector = not render_target_vector; break;
		case 'v': render_velocity_vector = not render_velocity_vector; break;
		}
	}

	void sim_103(int argc, char *argv[]) {
		_argc = argc;
		_argv = argv;
		int r = sim_103_init(true);
		if (r != 0) {
			cerr << "Error in initialisation of simulation 00" << endl;
			return;
		}

		atexit(sim_103_exit);
		glutDisplayFunc(sim_103_render);
		glutReshapeFunc(charanim::resize);
		glutMouseFunc(charanim::mouse_click);
		glutPassiveMotionFunc(charanim::mouse_passive);
		glutMotionFunc(charanim::mouse_drag);
		glutSpecialFunc(charanim::special_keys_keyboard);
		glutKeyboardFunc(sim_103_regular_keys_keyboard);

		glutTimerFunc(1030.0f/charanim::FPS, sim_103_timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
} // -- namespace charanim
