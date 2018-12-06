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
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>
using namespace physim::init;
using namespace physim::particles;
using namespace physim::geom;

// charanim includes
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>
#include <render/geometry/rplane.hpp>
#include <anim/charanim.hpp>
#include <anim/vec_helper.hpp>
#include <anim/terrain/terrain.hpp>

namespace charanim {
namespace study_cases {

	static terrain sim_00_T;

	void sim_00_usage() {

	}

	void sim_00_init_sim() {
		const vector<segment>& sgs = sim_00_T.get_segments();
		for (const segment& s : sgs) {
			const point2D& A = s.first;
			const point2D& B = s.second;

			pm_vec3 p1( A.x, 0.0f,  A.y);
			pm_vec3 p2( B.x, 0.0f,  B.y);
			pm_vec3 p3(p2.x, 2.0f, p2.z);
			pm_vec3 p4(p1.x, 2.0f, p1.z);

			rectangle *rl = new rectangle(p1,p2,p3,p4);
			S.add_geometry(rl);
		}

		sized_particle *p1 = new sized_particle();
		sized_particle *p2 = new sized_particle();
		sized_particle *p3 = new sized_particle();

		p1->lifetime = 99999999.0f;
		p1->bouncing = 1.0f;
		p1->friction = 0.0f;

		p2->lifetime = 99999999.0f;
		p2->bouncing = 1.0f;
		p2->friction = 0.0f;

		p3->lifetime = 99999999.0f;
		p3->bouncing = 1.0f;
		p3->friction = 0.0f;

		p1->cur_pos.x = p1->cur_pos.z = 2.0f;
		p1->cur_vel.x = p1->cur_vel.z = 1.0f;
		p1->R = 1.0f;

		p2->cur_pos.x = p2->cur_pos.z = 18.0f;
		p2->cur_vel.x = p2->cur_vel.z = -1.0f;
		p2->R = 1.0f;

		p3->cur_pos.x = 16.0f; p3->cur_pos.z = 4.0f;
		p3->cur_vel.x = p3->cur_vel.z = 1.0f;
		p3->R = 2.0f;

		S.set_particle_particle_collisions(true);
		S.set_viscous_drag(0.0f);

		S.add_sized_particle(p1);
		S.add_sized_particle(p2);
		S.add_sized_particle(p3);
	}

	void sim_00_init_geometry() {
		// add the geometry read from the map

		const vector<segment>& sgs = sim_00_T.get_segments();
		for (const segment& s : sgs) {
			const point2D& A = s.first;
			const point2D& B = s.second;

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

	int sim_00_init_shaders_models() {
		if (
			not flat_shader.init("../../charanim/shaders", "flat.vert", "flat.frag") or
			not material_shader.init("../../charanim/shaders", "materials.vert", "materials.frag") or
			not texture_shader.init("../../charanim/shaders", "textures.vert", "textures.frag")
		)
		{
			return 1;
		}

		sphere = new rendered_triangle_mesh();
		OBJ_reader obj;
		obj.load_object("../../models", "sphere.obj", *sphere);
		sphere->make_buffers();

		flat_shader.bind();
		flat_shader.set_bool("wireframe", true);
		flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));
		flat_shader.release();

		material_shader.bind();
		material_shader.set_vec3("light.diffuse", glm::vec3(1.0f,1.0f,1.0f));
		material_shader.set_vec3("light.ambient", glm::vec3(0.2f,0.2f,0.2f));
		material_shader.set_vec3("light.position", glm::vec3(0.f,0.f,0.f));
		material_shader.release();

		texture_shader.bind();
		texture_shader.set_vec3("light.diffuse", glm::vec3(1.0f,1.0f,1.0f));
		texture_shader.set_vec3("light.ambient", glm::vec3(0.2f,0.2f,0.2f));
		texture_shader.set_vec3("light.position", glm::vec3(0.f,0.f,0.f));
		texture_shader.release();

		return 0;
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

		if (map_file != "none") {
			bool r = sim_00_T.read_map(map_file);
			if (not r) {
				return 1;
			}
		}
		return 0;
	}

	int charanim_00(bool init_window) {
		width = 640;
		height = 480;

		special_key = 0;
		mouse_button = 0;

		last_mouse_moved = latticePoint(0,0);
		last_mouse_click = latticePoint(0,0);
		special_key_pressed = latticePoint(0,0);
		regular_key_pressed = latticePoint(0,0);

		bgd_color = glm::vec3(0.8f,0.8f,0.8f);

		FPS = 60;
		fps_count = 0;
		display_fps = false;
		sec = timing::now();

		draw_base_spheres = false;

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
			window_id = glutCreateWindow("Character animation");

			GLenum err = glewInit();
			if (err != 0) {
				cerr << "charanim::initialise_animation - Error:" << endl;
				cerr << "    when initialising glew: " << err << endl;
				return 1;
			}
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		V.get_box().set_min_max(glm::vec3(0,0,0), glm::vec3(20,5,20));
		V.set_window_dims(width, height);
		V.init_cameras();

		sim_00_init_sim();
		sim_00_init_geometry();
		int r = sim_00_init_shaders_models();
		if (r != 0) {
			return 1;
		}

		return 0;
	}

	void sim_00_regular_keys_keyboard(unsigned char c, int x, int y) {
		charanim::regular_keys_keyboard(c, x, y);
		switch (c) {
		case 'h':
			sim_00_usage();
			break;
		case 'r':
			exit_func();
			charanim_00(false);
			break;
		}
	}

	void sim_00(int argc, char *argv[]) {
		_argc = argc;
		_argv = argv;
		if (charanim_00(true) != 0) {
			cerr << "Error in initialisation of simulation 00" << endl;
			return;
		}

		atexit(charanim::exit_func);
		glutDisplayFunc(charanim::full_render);
		glutReshapeFunc(charanim::resize);
		glutMouseFunc(charanim::mouse_click);
		glutPassiveMotionFunc(charanim::mouse_passive);
		glutMotionFunc(charanim::mouse_drag);
		glutSpecialFunc(charanim::special_keys_keyboard);
		glutKeyboardFunc(sim_00_regular_keys_keyboard);

		glutTimerFunc(1000.0f/charanim::FPS, charanim::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
} // -- namespace charanim
