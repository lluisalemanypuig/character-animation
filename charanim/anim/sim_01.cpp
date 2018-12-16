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
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>
using namespace physim::init;
using namespace physim::particles;
using namespace physim::geometry;

// charanim includes
#include <render/include_gl.hpp>
#include <render/geometry/rplane.hpp>
#include <anim/charanim.hpp>
#include <anim/vec_helper.hpp>
#include <anim/terrain/terrain.hpp>
#include <anim/terrain/regular_grid.hpp>

#define input_2_points(p,q)					\
	cout << "Input two points:" << endl;	\
	cout << "    Point A:" << endl;			\
	cout << "        x:"; cin >> p.x;		\
	cout << "        y:"; cin >> p.y;		\
	cout << "    Point B:" << endl;			\
	cout << "        x:"; cin >> q.x;		\
	cout << "        y:"; cin >> q.y;

namespace charanim {
namespace study_cases {

	// Terrain of the simulation. Loaded from file.
	static terrain sim_01_T;

	// radius of the only agent.
	static float sim_01_R;
	// path of the only agent.
	static vector<vec2> sim_01_astar_path;
	static vector<vec2> sim_01_smoothed_path;

	// only agent in the simulation
	static agent_particle *sim_01_agent;

	// render stuff
	static bool sim_01_render_circles = false;
	static GLUquadric *sim_01_disk = nullptr;

	void sim_01_usage() {
		cout << "Simulation 00: for map editing and inspection" << endl;
		cout << endl;
		cout << "Specify a map file as parameter to visualise it." << endl;
		cout << "    --help : show the usage." << endl;
		cout << "    --map f: specify map file." << endl;
		cout << endl;
		cout << "Keyboard keys:" << endl;
		cout << "    h: show the usage." << endl;
		cout << "    r: reset simulation." << endl;
		cout << "    a: add segment." << endl;
		cout << "    p: find path between two 2d points." << endl;
		cout << "    c: render circles around path vertices." << endl;
		cout << "    d: render distance function for obstacle avoidance" << endl;
		cout << "    g: render grid for path finding" << endl;
		cout << endl;
	}

	void sim_01_render_a_path
	(const vector<vec2>& apath, const glm::vec3& pcol,
	 const glm::vec3& ccol, float yoff)
	{
		glBegin(GL_LINES);
		glColor3f(pcol.x, pcol.y, pcol.z);
		for (size_t i = 0; i < apath.size() - 1; ++i) {
			const vec2& p = apath[i];
			const vec2& q = apath[i + 1];
			glVertex3f(p.x, yoff, p.y);
			glVertex3f(q.x, yoff, q.y);
		}
		glEnd();
		if (sim_01_render_circles) {
			glColor3f(ccol.x, ccol.y, ccol.z);
			for (size_t i = 0; i < apath.size(); ++i) {
				const vec2& p = apath[i];
				glPushMatrix();
					glTranslatef(p.x, yoff, p.y);
					glRotatef(-90.0f, 1.0f,0.0f,0.0f);
					gluDisk(sim_01_disk, double(sim_01_R)*0.9, double(sim_01_R), 20, 20);
				glPopMatrix();
			}
		}
	}

	void sim_01_render() {
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

		// render path finder (on the xy plane)
		const regular_grid *rg = sim_01_T.get_regular_grid();
		render_regular_grid(rg);

		if (sim_01_astar_path.size() > 1) {
			sim_01_render_a_path
			(sim_01_astar_path, glm::vec3(1.0f,0.0f,0.0f),
			 glm::vec3(0.0f,1.0f,0.0f), 1.0f);
		}
		if (sim_01_smoothed_path.size() > 1) {
			sim_01_render_a_path
			(sim_01_smoothed_path, glm::vec3(0.0f,0.0f,1.0f),
			 glm::vec3(0.0f,1.0f,1.0f), 2.0f);
		}

		if (window_id != -1) {
			glutSwapBuffers();
		}
	}

	void sim_01_timed_refresh(int v) {
		sim_01_render();

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

		glutTimerFunc(1000/FPS, sim_01_timed_refresh, v);
	}

	void sim_01_init_simulation() {
		// add agent particles
		sim_01_agent = new agent_particle();
		sim_01_agent->lifetime = 9999.0f; // immortal agent
		sim_01_agent->R = 1.0f;
		S.add_agent_particle(sim_01_agent);
	}
	void sim_01_init_geometry() {
		// add the geometry read from the map

		const vector<segment>& sgs = sim_01_T.get_segments();
		for (const segment& s : sgs) {
			const vec2& A = s.first;
			const vec2& B = s.second;

			glm_vec3 p1( A.x, 0.0f,  A.y);
			glm_vec3 p2( B.x, 0.0f,  B.y);
			glm_vec3 p3(p2.x, 2.0f, p2.z);
			glm_vec3 p4(p1.x, 2.0f, p1.z);

			rplane *pl = new rplane();
			pl->set_points(p1, p2, p3, p4);
			geometry.push_back(pl);

			V.get_box().enlarge_box(p1);
			V.get_box().enlarge_box(p2);
			V.get_box().enlarge_box(p3);
			V.get_box().enlarge_box(p4);
		}

		V.get_box().make_buffers();
	}

	int sim_01_parse_arguments(int argc, char *argv[]) {
		string map_file = "none";

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--help") == 0) {
				sim_01_usage();
				return 2;
			}
			else if (strcmp(argv[i], "--map") == 0) {
				map_file = string(argv[i + 1]);
				++i;
			}
		}

		if (map_file == "none") {
			cerr << "Error: no map file specified. Use" << endl;
			cerr << "    ./anim 00 --help" << endl;
			cerr << "to see the usage" << endl;
			return 1;
		}

		bool r = sim_01_T.read_map(map_file);
		if (not r) {
			return 1;
		}
		return 0;
	}

	int charanim_01(bool init_window) {
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

		window_id = -1;
		draw_base_spheres = true;
		render_grid = false;
		render_dist_func = false;

		/* PARSE ARGUMENTS */
		int arg_parse = sim_01_parse_arguments(_argc, _argv);
		if (arg_parse != 0) {
			return arg_parse;
		}

		/* INITIALISE GLUT WINDOW */
		if (init_window) {
			glutInit(&_argc, _argv);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowSize(width, height);
			window_id = glutCreateWindow("Character animation - Map editor and visualiser");

			GLenum err = glewInit();
			if (err != 0) {
				cerr << "charanim::initialise_animation - Error:" << endl;
				cerr << "    when initialising glew: " << err << endl;
				return 1;
			}
		}

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		V.get_box().set_min_max(glm::vec3(0,0,0), glm::vec3(20,5,20));
		V.set_window_dims(width, height);
		V.init_cameras();

		sim_01_init_geometry();
		sim_01_init_simulation();

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

	void sim_01_add_segment() {
		vec2 A, B;
		input_2_points(A,B);

		segment s(A,B);
		regular_grid *rg = sim_01_T.get_regular_grid();
		rg->rasterise_segment(s);
		rg->expand_function_distance(s);
		rg->make_final_state();

		rplane *pl = new rplane();

		glm_vec3 p1( A.x, 0.0f,  A.y);
		glm_vec3 p2( B.x, 0.0f,  B.y);
		glm_vec3 p3(p2.x, 2.0f, p2.z);
		glm_vec3 p4(p1.x, 2.0f, p1.z);
		pl->set_points(p1, p2, p3, p4);
		geometry.push_back(pl);

		V.get_box().enlarge_box(p1);
		V.get_box().enlarge_box(p2);
		V.get_box().enlarge_box(p3);
		V.get_box().enlarge_box(p4);
	}

	void sim_01_compute_path() {
		vec2 start, goal;
		input_2_points(start,goal);
		cout << "Input radius: "; cin >> sim_01_R;

		if (sim_01_disk == nullptr) {
			sim_01_disk = gluNewQuadric();
		}

		sim_01_astar_path.clear();
		sim_01_smoothed_path.clear();
		regular_grid *rg = sim_01_T.get_regular_grid();
		timing::time_point begin = timing::now();
		rg->find_path(
			start, goal, sim_01_R,
			sim_01_astar_path, sim_01_smoothed_path
		);
		timing::time_point end = timing::now();

		cout << "Path computed in " << timing::elapsed_seconds(begin, end)
			 << " seconds" << endl;

		/* initialise agent's movement */

		// 1. set current position
		sim_01_agent->cur_pos.x = sim_01_smoothed_path[0].x;
		sim_01_agent->cur_pos.y = 1.0f;
		sim_01_agent->cur_pos.z = sim_01_smoothed_path[0].y;

		// 2. set first attractor
		sim_01_agent->attractor.x = sim_01_smoothed_path[1].x;
		sim_01_agent->attractor.y = 1.0f;
		sim_01_agent->attractor.z = sim_01_smoothed_path[1].y;

		// 3. set maximum desired velocity
		sim_01_agent->max_vel = 1.0f; // 1 m/s
		sim_01_agent->attractor_acceleration = 1.0f; // 1 m/s^2
	}

	void sim_01_regular_keys_keyboard(unsigned char c, int x, int y) {
		charanim::regular_keys_keyboard(c, x, y);
		switch (c) {
		case 'h': sim_01_usage(); break;
		case 'r': exit_func(); charanim_01(false); break;
		case 'a': sim_01_add_segment(); break;
		case 'p': sim_01_compute_path(); break;
		case 'c': sim_01_render_circles = not sim_01_render_circles; break;
		case 'd': render_dist_func = not render_dist_func; break;
		case 'g': render_grid = not render_grid; break;
		}
	}

	void sim_01_exit() {
		exit_func();

		if (sim_01_disk != nullptr) {
			gluDeleteQuadric(sim_01_disk);
		}
	}

	void sim_01(int argc, char *argv[]) {
		_argc = argc;
		_argv = argv;
		int r = charanim_01(true);
		if (r != 0) {
			cerr << "Error in initialisation of simulation 00" << endl;
			return;
		}

		atexit(sim_01_exit);
		glutDisplayFunc(sim_01_render);
		glutReshapeFunc(charanim::resize);
		glutMouseFunc(charanim::mouse_click);
		glutPassiveMotionFunc(charanim::mouse_passive);
		glutMotionFunc(charanim::mouse_drag);
		glutSpecialFunc(charanim::special_keys_keyboard);
		glutKeyboardFunc(sim_01_regular_keys_keyboard);

		glutTimerFunc(1000.0f/charanim::FPS, sim_01_timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
} // -- namespace charanim
