#include <anim/charanim.hpp>

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>
typedef physim::math::vec3 pm_vec3;
using namespace physim::init;
using namespace physim::particles;
using namespace physim::geom;

// charanim includes
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

namespace charanim {

void print_usage() {

}

void initialise_simulation() {

	plane *wall1 = new plane( pm_vec3( 1.0f, 0.0f,  0.0f), pm_vec3( 0.0f, 0.0f,  0.0f) );
	plane *wall2 = new plane( pm_vec3(-1.0f, 0.0f,  0.0f), pm_vec3(20.0f, 0.0f,  0.0f) );
	plane *wall3 = new plane( pm_vec3( 0.0f, 0.0f,  1.0f), pm_vec3( 0.0f, 0.0f,  0.0f) );
	plane *wall4 = new plane( pm_vec3( 0.0f, 0.0f, -1.0f), pm_vec3( 0.0f, 0.0f, 20.0f) );

	S.add_geometry(wall1);
	S.add_geometry(wall2);
	S.add_geometry(wall3);
	S.add_geometry(wall4);

	initialiser I;
	I.set_bounce_initialiser( [](free_particle *p) { p->bouncing = 1.0f; } );
	I.set_friction_initialiser( [](free_particle *p) { p->friction = 0.0f; } );
	I.set_pos_initialiser(
		[](free_particle *p) {
			float x = 20.0f*float(rand())/RAND_MAX;
			float z = 20.0f*float(rand())/RAND_MAX;
			p->cur_pos = pm_vec3(x, 0.0f, z);
		}
	);
	I.set_vel_initialiser(
		[](free_particle *p) {
			float x = 5.0f*float(rand())/RAND_MAX;
			float z = 5.0f*float(rand())/RAND_MAX;
			p->cur_vel = pm_vec3(x, 0.0f, z);
		}
	);
	I.set_radius_initialiser(
		[](sized_particle *p) {
			p->R = 2.0f;
		}
	);

	S.set_time_step(0.001f);
	S.set_initialiser(&I);
	S.add_sized_particles(10);
}

void initialise_shaders_models() {
	sphere = new rendered_tri_mesh();
	OBJ_reader obj;
	obj.load_object("../../models/", "sphere.obj", *sphere);

	sphere->compile();
}

int initialise_animation(int argc, char *argv[]) {
	width = 640;
	height = 480;

	special_key = 0;
	pressed_button = 0;

	last_mouse_moved = point(0,0);
	last_mouse_click = point(0,0);
	special_key_pressed = point(0,0);
	regular_key_pressed = point(0,0);

	bgd_color = glm::vec3(0.8f,0.8f,0.8f);

	FPS = 60;
	fps_count = 0;
	display_fps = false;
	sec = timing::now();

	draw_base_spheres = false;

	// parse arguments
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 2;
		}
	}

	// initialise GLUT window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	window_id = glutCreateWindow("Character animation");

	GLenum err = glewInit();
	if (err != 0) {
		cerr << "charanim::initialise_animation - Error:" << endl;
		cerr << "    when initialising glew: " << err << endl;
		return 1;
	}

	V.get_box().set_min_max(glm::vec3(-5,-5,-5), glm::vec3(25,5,25));
	V.set_window_dims(width, height);
	V.init_cameras();

	initialise_simulation();
	initialise_shaders_models();

	return 0;
}

} // -- namespace charanim
