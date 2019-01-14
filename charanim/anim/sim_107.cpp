#include <anim/charanim.hpp>

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
typedef glm::vec3 gvec3;

// physim includes
#include <physim/particles/agent_particle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>
using namespace physim::particles;
using namespace physim::geometric;
using namespace physim::math;

// render includes
#include <render/geometry/rplane.hpp>
#include <render/geometry/rsphere.hpp>
#include <render/geometry/rrectangle.hpp>
#include <render/obj_reader.hpp>
#include <render/shader/shader_helper.hpp>

// custom includes
#include <anim/sim_1xx.hpp>
#include <anim/vec_helper.hpp>

namespace charanim {
using namespace sim_1xx;

namespace study_cases {

	void sim_107_usage() {
		cout << "Simulation 107: validation of 'walk with me' behaviour" << endl;
		cout << endl;
		cout << "Parameters:" << endl;
		cout << "    --help" << endl;
		cout << "    --seek-weight" << endl;
		cout << "    --max-speed" << endl;
		cout << "    --max-force" << endl;
		cout << "    --wwm-weight" << endl;
		cout << "    --wwm-distance" << endl;
		cout << endl;
		cout << "Keyboard keys:" << endl;
		cout << "    h: show the usage." << endl;
		cout << "    r: reset simulation." << endl;
		cout << "    v: render velocity vector" << endl;
		cout << "    a: render attractor vector" << endl;
		cout << "    s: render wireframe spheres" << endl;
		cout << "    b: run simulation" << endl;
		cout << endl;
	}

	void sim_107_render() {
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

		base_render();
		render_agent_vectors();

		if (run) {
			agent_particle& sim_1xx_agent = S.get_agent_particle(0);
			if (sim_1xx_path_it < sim_1xx_path.size() - 1) {
				if (dist(sim_1xx_agent.cur_pos, sim_1xx_agent.target) <= 2.5f) {
					++sim_1xx_path_it;
					sim_1xx_agent.target = sim_1xx_path[sim_1xx_path_it];
					if (sim_1xx_path_it == sim_1xx_path.size() - 1) {
						sim_1xx_agent.unset_behaviour(agent_behaviour_type::seek);
						sim_1xx_agent.set_behaviour(agent_behaviour_type::arrival);
					}
				}
			}

			for (int i = 0; i < 100; ++i) {
				S.simulate_agent_particles();
			}
		}

		if (window_id != -1) {
			if (record) {
				record_screen();
			}
			glutSwapBuffers();
		}
	}

	void sim_107_timed_refresh(int v) {
		sim_107_render();

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

		glutTimerFunc(1000/FPS, sim_107_timed_refresh, v);
	}

	void sim_107_init_agent(agent_particle& a) {
		a.lifetime = 9999.0f; // immortal agent
		a.R = 0.75f;
		a.mass = 60.0f;
		a.bouncing = 1.0f;
		a.friction = 0.0f;
		a.orientation = physim::math::normalise(a.cur_vel);

		a.max_speed = sim_1xx_max_speed;
		a.max_force = sim_1xx_max_force;
		a.align_weight = sim_1xx_alignment_weight;
		a.arrival_weight = sim_1xx_arrival_weight;
		a.arrival_distance = sim_1xx_arrival_distance;
		a.ucoll_weight = sim_1xx_ucoll_weight;
		a.ucoll_distance = sim_1xx_ucoll_distance;
		a.wow_weight = sim_1xx_wwm_weight;
		a.wow_distance = sim_1xx_wwm_distance;

		a.unset_all_behaviours();
		a.set_behaviour(agent_behaviour_type::arrival);
		a.set_behaviour(agent_behaviour_type::walk_with_me);
	}

	void sim_107_init_simulation() {
		sim_1xx_path.push_back(vec3(0.75f, 0.0f, 25.0f));
		sim_1xx_path.push_back(vec3(0.75f, 0.0f, 50.0f));

		size_t idx;

		idx = S.add_agent_particle();
		agent_particle& a = S.get_agent_particle(idx);
		a.target = sim_1xx_path[0];
		a.cur_pos = vec3(12.0f, 0.0f, 10.0f);
		a.cur_vel = normalise(a.target - a.cur_pos);
		sim_107_init_agent(a);

		idx = S.add_agent_particle();
		agent_particle& b = S.get_agent_particle(idx);
		b.target = vec3(-0.75f, 0.0f, 50.0f);
		b.cur_pos = vec3(0.0f, 0.0f, 0.0f);
		b.cur_vel = vec3(0.0f,0,1);
		sim_107_init_agent(b);

		S.set_particle_particle_collisions(true);
		S.set_time_step(0.001f);
	}

	void sim_107_init_geometry() {
		rplane *rp = new rplane();
		rp->set_points(
			gvec3(-25.0f, -2.0f, -5.0f), gvec3(-25.0f, -2.0f, 55.0f),
			gvec3(25.0f, -2.0f, 55.0f), gvec3(25.0f, -2.0f, -5.0f)
		);
		rp->set_color(0.4f,0.4f,0.4f,0.5f);
		geometry.push_back(rp);

		V.get_box().set_min_max(
			gvec3(-50.0f, -10.0f, -50.0f), gvec3(50.0f, 10.0f, 50.0f)
		);

		V.get_box().make_buffers();

		V.set_window_dims(width, height);
		V.init_cameras();
	}

	int sim_107_parse_arguments(int argc, char *argv[]) {
		string map_file = "none";

		float x,y,z;

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--help") == 0) {
				sim_107_usage();
				return 2;
			}
			else if (strcmp(argv[i], "--max-speed") == 0) {
				sim_1xx_max_speed = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--max-force") == 0) {
				sim_1xx_max_force = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--arrival-weight") == 0) {
				sim_1xx_arrival_weight = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--arrival-dist") == 0) {
				sim_1xx_arrival_distance = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--wwm-weight") == 0) {
				sim_1xx_wwm_weight = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--wwm-distance") == 0) {
				sim_1xx_wwm_distance = atof(argv[i + 1]);
				++i;
			}
		}

		return 0;
	}

	void sim_107_exit() {
		exit_func();
		sim_1xx_path.clear();
	}

	int sim_107_init(bool init_window) {
		width = 640;
		height = 480;

		special_key = 0;
		mouse_button = 0;

		last_mouse_moved = latticePoint(0,0);
		last_mouse_click = latticePoint(0,0);
		special_key_pressed = latticePoint(0,0);
		regular_key_pressed = latticePoint(0,0);

		float _move_x = move_x;
		float _move_z = move_z;

		move_x = 0.0f;
		move_z = 0.0f;
		bgd_color = glm::vec3(0.8f,0.8f,0.8f);

		FPS = 60;
		fps_count = 0;
		display_fps = false;

		render_base_spheres = true;
		render_grid = false;
		render_dist_func = false;
		render_targets = true;
		render_velocity_vector = true;
		render_target_vector = true;
		render_orientation_vector = true;

		sim_1xx_ini_pos = vec3(25.0f,0.0f,0.0f);
		sim_1xx_ini_vel = vec3(0.5f, 0.0f, 0.5f);
		sim_1xx_target = vec3(-20.0f, 0.0f, 20.0f);

		sim_1xx_mass = 60.0f;

		sim_1xx_max_speed = 1.0f;
		sim_1xx_max_force = 100.0f;
		sim_1xx_alignment_weight = 0.001f;
		sim_1xx_arrival_weight = 5.0f;
		sim_1xx_arrival_distance = 20.0f;
		sim_1xx_ucoll_weight = 7.5f;
		sim_1xx_ucoll_distance = 2.5f;
		sim_1xx_wwm_weight = 1.0f;
		sim_1xx_wwm_distance = 10.0f;

		/* PARSE ARGUMENTS */
		int arg_parse = sim_107_parse_arguments(_argc, _argv);
		if (arg_parse != 0) {
			return arg_parse;
		}

		/* INITIALISE GLUT WINDOW */
		if (init_window) {
			glutInit(&_argc, _argv);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowSize(width, height);

			string title =
			"Character animation - Walk with me steering inspection 1";
			window_id = glutCreateWindow(title.c_str());

			GLenum err = glewInit();
			if (err != 0) {
				cerr << "charanim::initialise_animation - Error:" << endl;
				cerr << "    when initialising glew: " << err << endl;
				return 1;
			}
		}

		glEnable(GL_DEPTH_TEST);

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

		success = load_characters(
			{"../../characters", "../../characters"},
			{"paladin.cfg", "cally.cfg"}
		);
		if (not success) {
			cerr << "Error: error when loading characters" << endl;
			return 1;
		}
		characters[0].set_anim_weight(2, 0.2f, 0.0f);
		characters[0].set_anim_weight(6, 0.8f, 0.0f);
		characters[1].set_anim_weight(2, 0.2f, 0.0f);
		characters[1].set_anim_weight(6, 0.8f, 0.0f);

		sim_107_init_simulation();

		float zoomP = V.get_perspective_camera().get_zoom();
		float zoomC = V.get_orthogonal_camera().get_zoom();

		sim_107_init_geometry();

		if (not init_window) {
			V.get_perspective_camera().set_zoom(zoomP);
			V.get_orthogonal_camera().set_zoom(zoomC);
			move_x = _move_x;
			move_z = _move_z;
		}

		sim_107_usage();
		for (const agent_particle& a : S.get_agent_particles()) {
			print_1xx_info(a);
		}
		return 0;
	}

	void sim_107_regular_keys_keyboard(unsigned char c, int x, int y) {
		charanim::regular_keys_keyboard(c, x, y);
		switch (c) {
		case 'h': sim_107_usage(); break;
		case 'r': sim_107_exit(); sim_107_init(false); break;
		}
	}

	void sim_107(int argc, char *argv[]) {
		_argc = argc;
		_argv = argv;
		int r = sim_107_init(true);
		if (r != 0) {
			if (r == 1) {
				cerr << "Error in initialisation of simulation 107" << endl;
			}
			return;
		}

		sec = timing::now();
		exe_time = timing::now();

		atexit(sim_107_exit);
		glutDisplayFunc(sim_107_render);
		glutReshapeFunc(charanim::resize);
		glutMouseFunc(charanim::mouse_click);
		glutPassiveMotionFunc(charanim::mouse_passive);
		glutMotionFunc(charanim::mouse_drag);
		glutSpecialFunc(charanim::special_keys_keyboard);
		glutKeyboardFunc(sim_107_regular_keys_keyboard);

		glutTimerFunc(1030.0f/charanim::FPS, sim_107_timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
} // -- namespace charanim
