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
#include <physim/particles/agent_particle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>
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

	void sim_100_usage() {
		cout << "Simulation 100: validation of seek behaviour" << endl;
		cout << endl;
		cout << "    --help : show the usage." << endl;
		cout << "    --target" << endl;
		cout << "    --pos" << endl;
		cout << "    --vel" << endl;
		cout << "    --seek-weight" << endl;
		cout << "    --max-speed" << endl;
		cout << "    --max-force" << endl;
		cout << endl;
		cout << "Keyboard keys:" << endl;
		cout << "    h: show the usage." << endl;
		cout << "    r: reset simulation." << endl;
		cout << "    v: render velocity vector" << endl;
		cout << "    a: render attractor vector" << endl;
		cout << endl;
	}

	void sim_100_render() {
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

		glDisable(GL_LIGHTING);
		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_LINES);
			glVertex3f(sim_1xx_target.x, sim_1xx_target.y, sim_1xx_target.z);
			glVertex3f(sim_1xx_ini_pos.x, sim_1xx_ini_pos.y, sim_1xx_ini_pos.z);
		glEnd();

		for (int i = 0; i < 100; ++i) {
			S.simulate_agent_particles();
		}

		if (window_id != -1) {
			glutSwapBuffers();
		}
	}

	void sim_100_timed_refresh(int v) {
		sim_100_render();

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

		glutTimerFunc(1000/FPS, sim_100_timed_refresh, v);
	}

	void sim_100_init_simulation() {
		agent_particle dummy;
		S.add_agent_particle(dummy);

		sim_1xx_agent = &S.get_agent_particle(0);

		sim_1xx_agent->lifetime = 9999.0f; // immortal agent
		sim_1xx_agent->R = 1.0f;

		sim_1xx_agent->target = sim_1xx_target;

		sim_1xx_agent->cur_pos = sim_1xx_ini_pos;
		sim_1xx_agent->cur_vel = sim_1xx_ini_vel;
		sim_1xx_agent->orientation = physim::math::normalise(sim_1xx_ini_vel);

		sim_1xx_agent->max_speed = sim_1xx_max_speed;
		sim_1xx_agent->max_force = sim_1xx_max_force;
		sim_1xx_agent->align_weight = sim_1xx_alignment_weight;
		sim_1xx_agent->seek_weight = sim_1xx_seek_weight;

		sim_1xx_agent->mass = sim_1xx_mass;
		sim_1xx_agent->bouncing = 1.0f;
		sim_1xx_agent->friction = 0.0f;

		sim_1xx_agent->unset_all_behaviours();
		sim_1xx_agent->set_behaviour(agent_behaviour_type::seek);

		S.set_time_step(0.001f);
	}

	void sim_100_init_geometry() {
		// add the geometry read from the map

		rplane *rp = new rplane();
		rp->set_points(
			gvec3(-25.0f, -2.0f, -25.0f), gvec3(-25.0f, -2.0f,  25.0f),
			gvec3( 25.0f, -2.0f,  25.0f), gvec3( 25.0f, -2.0f, -25.0f)
		);
		rp->set_color(0.4f,0.4f,0.4f,0.5f);
		geometry.push_back(rp);

		V.get_box().set_min_max(
			gvec3(-25.0f, -10.0f, -25.0f), gvec3(25.0f, 10.0f, 25.0f)
		);

		V.get_box().make_buffers();

		V.set_window_dims(width, height);
		V.init_cameras();
	}

	int sim_100_parse_arguments(int argc, char *argv[]) {
		string map_file = "none";

		float x,y,z;

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--help") == 0) {
				sim_100_usage();
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

	void sim_100_exit() {
		exit_func();
	}

	int sim_100_init(bool init_window) {
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
		sec = timing::now();

		render_base_spheres = true;
		render_grid = false;
		render_dist_func = false;
		render_targets = true;
		render_velocity_vector = true;
		render_target_vector = true;

		sim_1xx_ini_pos = vec3(0.0f,0.0f,0.0f);
		sim_1xx_ini_vel = vec3(0.5f, 0.0f, 0.5f);
		sim_1xx_target = vec3(-20.0f, 0.0f, 20.0f);

		sim_1xx_max_speed = 0.5f;
		sim_1xx_max_force = 100.0f;
		sim_1xx_alignment_weight = 0.001f;
		sim_1xx_seek_weight = 5.0f;
		sim_1xx_mass = 60.0f;

		/* PARSE ARGUMENTS */
		int arg_parse = sim_100_parse_arguments(_argc, _argv);
		if (arg_parse != 0) {
			return arg_parse;
		}

		/* INITIALISE GLUT WINDOW */
		if (init_window) {
			glutInit(&_argc, _argv);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowSize(width, height);
			window_id = glutCreateWindow("Character animation - Seek steering inspection");

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
		success = load_characters({"../../characters"}, {"paladin.cfg"});
		if (not success) {
			cerr << "Error: error when loading characters" << endl;
			return 1;
		}

		float zoomP = V.get_perspective_camera().get_zoom();
		float zoomC = V.get_orthogonal_camera().get_zoom();

		sim_100_init_geometry();

		if (not init_window) {
			V.get_perspective_camera().set_zoom(zoomP);
			V.get_orthogonal_camera().set_zoom(zoomC);
			move_x = _move_x;
			move_z = _move_z;
		}

		sim_100_init_simulation();

		sim_100_usage();
		print_1xx_info();

		return 0;
	}

	void sim_100_regular_keys_keyboard(unsigned char c, int x, int y) {
		charanim::regular_keys_keyboard(c, x, y);
		switch (c) {
		case 'h': sim_100_usage(); break;
		case 'r': sim_100_exit(); sim_100_init(false); break;
		case 'a': render_target_vector = not render_target_vector; break;
		case 'v': render_velocity_vector = not render_velocity_vector; break;
		}
	}

	void sim_100(int argc, char *argv[]) {
		_argc = argc;
		_argv = argv;
		int r = sim_100_init(true);
		if (r != 0) {
			if (r == 1) {
				cerr << "Error in initialisation of simulation 00" << endl;
			}
			return;
		}

		atexit(sim_100_exit);
		glutDisplayFunc(sim_100_render);
		glutReshapeFunc(charanim::resize);
		glutMouseFunc(charanim::mouse_click);
		glutPassiveMotionFunc(charanim::mouse_passive);
		glutMotionFunc(charanim::mouse_drag);
		glutSpecialFunc(charanim::special_keys_keyboard);
		glutKeyboardFunc(sim_100_regular_keys_keyboard);

		glutTimerFunc(1000.0f/charanim::FPS, sim_100_timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
} // -- namespace charanim
