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

	void sim_103_usage() {
		cout << "Simulation 103: validation of collision avoidance behaviour" << endl;
		cout << endl;
		cout << "This simulated uses a 'pre-computed' path" << endl;
		cout << endl;
		cout << "Parameters:" << endl;
		cout << "    --help" << endl;
		cout << "    --seek-weight" << endl;
		cout << "    --max-speed" << endl;
		cout << "    --max-force" << endl;
		cout << "    --coll-avoid" << endl;
		cout << "    --ahead-distance" << endl;
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

		for (int i = 0; i < 100; ++i) {
			S.simulate_agent_particles();
		}

		if (sim_1xx_path_it < sim_1xx_path.size() - 1) {
			if (dist(sim_1xx_agent->cur_pos, sim_1xx_agent->target) <= 2.5f) {
				++sim_1xx_path_it;
				sim_1xx_agent->target = sim_1xx_path[sim_1xx_path_it];
				if (sim_1xx_path_it == sim_1xx_path.size() - 1) {
					sim_1xx_agent->unset_behaviour(agent_behaviour_type::seek);
					sim_1xx_agent->set_behaviour(agent_behaviour_type::arrival);
				}
			}
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
		agent_particle dummy;
		S.add_agent_particle(dummy);

		sim_1xx_agent = &S.get_agent_particle(0);
		
		sim_1xx_path.clear();
		sim_1xx_path = { vec3(25.0f, 0.0f, 12.0f),
						 vec3(42.0f, 0.0f, 25.0f),
						 vec3(25.0f, 0.0f, 40.0f) };

		// add agent particles
		
		sim_1xx_agent->lifetime = 9999.0f; // immortal agent
		sim_1xx_agent->R = 0.5f;

		sim_1xx_path_it = 0;
		sim_1xx_agent->target = sim_1xx_path[sim_1xx_path_it];

		sim_1xx_agent->cur_pos = sim_1xx_ini_pos;
		sim_1xx_agent->cur_vel = sim_1xx_ini_vel;
		sim_1xx_agent->orientation = physim::math::normalise(sim_1xx_ini_vel);

		sim_1xx_agent->max_speed = sim_1xx_max_speed;
		sim_1xx_agent->max_force = sim_1xx_max_force;
		sim_1xx_agent->align_weight = sim_1xx_alignment_weight;
		sim_1xx_agent->seek_weight = sim_1xx_seek_weight;
		sim_1xx_agent->arrival_weight = sim_1xx_arrival_weight;
		sim_1xx_agent->slowing_distance = sim_1xx_slowing_distance;
		sim_1xx_agent->coll_avoid_weight = sim_1xx_coll_avoid_weight;
		sim_1xx_agent->ahead_distance = sim_1xx_ahead_distance;

		sim_1xx_agent->mass = sim_1xx_mass;
		sim_1xx_agent->bouncing = 1.0f;
		sim_1xx_agent->friction = 0.0f;

		sim_1xx_agent->unset_all_behaviours();
		sim_1xx_agent->set_behaviour(agent_behaviour_type::seek);
		sim_1xx_agent->set_behaviour(agent_behaviour_type::collision_avoidance);

		S.set_time_step(0.001f);

		// add physical geometry
		rectangle *r = new rectangle();
		r->set_points(vec3(0.0f, 0.0f, 25.0f), vec3(40.0f, 0.0f, 25.0f),
					  vec3(40.0f, 1.0f, 25.0f), vec3(0.0f, 1.0f, 25.0f));
		S.add_geometry(r);
		physim::geometric::sphere *s1 = new physim::geometric::sphere();
		s1->set_position( (sim_1xx_path[0] + sim_1xx_path[1])/2.0f );
		s1->set_radius(1.0f);
		S.add_geometry(s1);

		physim::geometric::sphere *s2 = new physim::geometric::sphere();
		s2->set_position( (sim_1xx_path[1] + sim_1xx_path[2])/2.0f );
		s2->set_radius(1.5f);
		S.add_geometry(s2);
	}

	void sim_103_init_geometry() {
		// add the geometry read from the map

		rplane *rp = new rplane();
		rp->set_points(
			gvec3(0.0f,  -2.0f, 0.0f),  gvec3(0.0f, -2.0f, 50.0f),
			gvec3(50.0f, -2.0f, 50.0f), gvec3(50.0f, -2.0f, 0.0f)
		);
		rp->set_color(0.4f,0.4f,0.4f,0.5f);
		geometry.push_back(rp);

		V.get_box().set_min_max(
			gvec3(0.0f, -10.0f, 0.0f), gvec3(50.0f, 10.0f, 50.0f)
		);

		V.get_box().make_buffers();

		V.set_window_dims(width, height);
		V.init_cameras();

		// add physical geometry
		rrectangle *r = new rrectangle();
		r->set_points(gvec3(0.0f, 0.0f, 25.0f), gvec3(40.0f, 0.0f, 25.0f),
					  gvec3(40.0f, 1.0f, 25.0f), gvec3(0.0f, 1.0f, 25.0f));
		geometry.push_back(r);

		rsphere *rball1 = new rsphere();
		rball1->set_center( to_gvec3((sim_1xx_path[0] + sim_1xx_path[1])/2.0f) );
		rball1->set_radius(1.0f);
		rsphere *rball2 = new rsphere();
		rball2->set_center( to_gvec3((sim_1xx_path[1] + sim_1xx_path[2])/2.0f) );
		rball2->set_radius(1.5f);

		shared_ptr<rendered_triangle_mesh> sim_ball(new rendered_triangle_mesh);
		OBJ_reader obj;
		obj.load_object("../../charanim/models", "sphere.obj", *sim_ball);

		rball1->set_model(sim_ball);
		geometry.push_back(rball1);

		rball2->set_model(sim_ball);
		geometry.push_back(rball2);

		sim_ball->load_textures();
		sim_ball->make_buffers_materials_textures();
	}

	int sim_103_parse_arguments(int argc, char *argv[]) {
		string map_file = "none";

		float x,y,z;

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--help") == 0) {
				sim_103_usage();
				return 2;
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
			else if (strcmp(argv[i], "--coll-avoid") == 0) {
				sim_1xx_coll_avoid_weight = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--ahead-distance") == 0) {
				sim_1xx_ahead_distance = atof(argv[i + 1]);
				++i;
			}
		}

		return 0;
	}

	void sim_103_exit() {
		exit_func();
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

		sim_1xx_max_speed = 0.5f;
		sim_1xx_max_force = 100.0f;
		sim_1xx_alignment_weight = 0.001f;
		sim_1xx_seek_weight = 5.0f;
		sim_1xx_arrival_weight = 5.0f;
		sim_1xx_slowing_distance = 20.0f;
		sim_1xx_coll_avoid_weight = 1.0f;
		sim_1xx_ahead_distance = 10.0f;

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
			window_id =
			glutCreateWindow("Character animation - Collision avoidance steering inspection");

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

		sim_103_init_simulation();

		float zoomP = V.get_perspective_camera().get_zoom();
		float zoomC = V.get_orthogonal_camera().get_zoom();

		sim_103_init_geometry();

		if (not init_window) {
			V.get_perspective_camera().set_zoom(zoomP);
			V.get_orthogonal_camera().set_zoom(zoomC);
			move_x = _move_x;
			move_z = _move_z;
		}

		sim_103_usage();
		print_1xx_info();
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
			if (r == 1) {
				cerr << "Error in initialisation of simulation 00" << endl;
			}
			return;
		}

		sec = timing::now();
		exe_time = timing::now();

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
