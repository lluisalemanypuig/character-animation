#include <anim/charanim.hpp>

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/vec3.hpp>
typedef glm::vec3 gvec3;

// physim includes
#include <physim/particles/sized_particle.hpp>
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
	static terrain sim_00_T;

	// radius of the hypothetic agent
	static float sim_00_R;
	// non-smoothed path in the grid
	static vector<vec2> sim_00_astar_path;
	// grid's smoothed path (the one supposed to be followed)
	static vector<vec2> sim_00_smoothed_path;

	// render stuff
	static bool sim_00_render_circles = false;
	static GLUquadric *sim_00_disk = nullptr;

	void sim_00_usage() {
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

	void sim_00_render_a_path
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
		if (sim_00_render_circles) {
			glColor3f(ccol.x, ccol.y, ccol.z);
			for (size_t i = 0; i < apath.size(); ++i) {
				const vec2& p = apath[i];
				glPushMatrix();
					glTranslatef(p.x, yoff, p.y);
					glRotatef(-90.0f, 1.0f,0.0f,0.0f);
					gluDisk(sim_00_disk, double(sim_00_R)*0.9, double(sim_00_R), 20, 20);
				glPopMatrix();
			}
		}
	}

	void sim_00_render() {
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
		const regular_grid *rg = sim_00_T.get_regular_grid();
		render_regular_grid(rg);

		if (sim_00_astar_path.size() > 1) {
			sim_00_render_a_path
			(sim_00_astar_path, glm::vec3(1.0f,0.0f,0.0f),
			 glm::vec3(0.0f,1.0f,0.0f), 1.0f);
		}
		if (sim_00_smoothed_path.size() > 1) {
			sim_00_render_a_path
			(sim_00_smoothed_path, glm::vec3(0.0f,0.0f,1.0f),
			 glm::vec3(0.0f,1.0f,1.0f), 2.0f);
		}

		if (window_id != -1) {
			glutSwapBuffers();
		}
	}

	void sim_00_timed_refresh(int v) {
		sim_00_render();

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

		glutTimerFunc(1000/FPS, sim_00_timed_refresh, v);
	}

	void sim_00_init_geometry() {
		// add the geometry read from the map

		const vector<segment>& sgs = sim_00_T.get_segments();
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
	}

	int sim_00_parse_arguments(int argc, char *argv[]) {
		string map_file = "none";

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--help") == 0) {
				sim_00_usage();
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

		bool r = sim_00_T.read_map(map_file);
		if (not r) {
			return 1;
		}
		return 0;
	}

	int sim_00_init(bool init_window) {
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
		draw_base_spheres = false;
		render_grid = false;
		render_dist_func = false;

		/* PARSE ARGUMENTS */
		int arg_parse = sim_00_parse_arguments(_argc, _argv);
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

		sim_00_init_geometry();
		load_shaders();

		return 0;
	}

	void sim_00_add_segment() {
		vec2 A, B;
		input_2_points(A,B);

		segment s(A,B);
		regular_grid *rg = sim_00_T.get_regular_grid();
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

	void sim_00_compute_path() {
		vec2 start, goal;
		input_2_points(start,goal);
		cout << "Input radius: "; cin >> sim_00_R;

		if (sim_00_disk == nullptr) {
			sim_00_disk = gluNewQuadric();
		}

		sim_00_astar_path.clear();
		sim_00_smoothed_path.clear();
		regular_grid *rg = sim_00_T.get_regular_grid();
		timing::time_point begin = timing::now();
		rg->find_path(
			start, goal, sim_00_R,
			sim_00_astar_path, sim_00_smoothed_path
		);
		timing::time_point end = timing::now();

		cout << "Path computed in " << timing::elapsed_seconds(begin, end)
			 << " seconds" << endl;
	}

	void sim_00_exit() {
		exit_func();

		if (sim_00_disk != nullptr) {
			gluDeleteQuadric(sim_00_disk);
		}
	}

	void sim_00_regular_keys_keyboard(unsigned char c, int x, int y) {
		charanim::regular_keys_keyboard(c, x, y);
		switch (c) {
		case 'h': sim_00_usage(); break;
		case 'a': sim_00_add_segment(); break;
		case 'p': sim_00_compute_path(); break;
		case 'c': sim_00_render_circles = not sim_00_render_circles; break;
		case 'd': render_dist_func = not render_dist_func; break;
		case 'g': render_grid = not render_grid; break;
		}
	}

	void sim_00(int argc, char *argv[]) {
		_argc = argc;
		_argv = argv;
		if (sim_00_init(true) != 0) {
			cerr << "Error in initialisation of simulation 00" << endl;
			return;
		}

		atexit(sim_00_exit);
		glutDisplayFunc(sim_00_render);
		glutReshapeFunc(charanim::resize);
		glutMouseFunc(charanim::mouse_click);
		glutPassiveMotionFunc(charanim::mouse_passive);
		glutMotionFunc(charanim::mouse_drag);
		glutSpecialFunc(charanim::special_keys_keyboard);
		glutKeyboardFunc(sim_00_regular_keys_keyboard);

		glutTimerFunc(1000.0f/charanim::FPS, sim_00_timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
} // -- namespace charanim
