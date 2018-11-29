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
#include <physim/geometry/plane.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/math/vec3.hpp>
typedef physim::math::vec3 pm_vec3;
using namespace physim::init;
using namespace physim::particles;
using namespace physim::geom;

// charanim includes
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>
#include <render/geometry/rplane.hpp>

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

	//physim::geom::sphere *ball = new physim::geom::sphere( pm_vec3(10.0f,0.0f,10.0f), 1);
	//S.add_geometry(ball);

	/*
	initialiser I;
	I.set_bounce_initialiser( [](free_particle *p) { p->bouncing = 1.0f; } );
	I.set_friction_initialiser( [](free_particle *p) { p->friction = 0.0f; } );
	I.set_lifetime_initialiser( [](free_particle *p) { p->lifetime = 999999999.0f; } );
	I.set_pos_initialiser(
		[](free_particle *p) {
			float x = 15.0f*float(rand())/RAND_MAX + 2.5f;
			float z = 15.0f*float(rand())/RAND_MAX + 2.5f;
			p->cur_pos = pm_vec3(x, 0.0f, z);
		}
	);
	I.set_vel_initialiser(
		[](free_particle *p) {
			float x = float(rand())/RAND_MAX;
			float z = float(rand())/RAND_MAX;
			cout << x << "0" << z << endl;
			p->cur_vel = pm_vec3(x, 0.0f, z);
		}
	);
	I.set_radius_initialiser(
		[](sized_particle *p) {
			p->R = 2.0f;
		}
	);

	S.set_viscous_drag(100.0f);
	S.set_particle_particle_collisions(true);
	S.set_time_step(0.0001f);
	S.set_initialiser(&I);
	S.add_sized_particles(10);
	*/


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

void initialise_renderer() {
	gvec3 A(  0.0f,  0.0f,  0.0f);
	gvec3 B(  0.0f,  5.0f,  0.0f);
	gvec3 C(  0.0f,  0.0f, 20.0f);
	gvec3 D(  0.0f,  5.0f, 20.0f);
	gvec3 E( 20.0f,  0.0f, 20.0f);
	gvec3 F( 20.0f,  5.0f, 20.0f);
	gvec3 G( 20.0f,  0.0f,  0.0f);
	gvec3 H( 20.0f,  5.0f,  0.0f);

	rplane *wall1 = new rplane();
	wall1->set_points(A,B,D,C);
	wall1->set_color(0.8f,0.0f,0.2f,1.0f);
	rplane *wall2 = new rplane();
	wall2->set_points(C,D,F,E);
	wall2->set_color(0.8f,0.0f,0.2f,1.0f);
	rplane *wall3 = new rplane();
	wall3->set_points(E,F,H,G);
	wall3->set_color(0.8f,0.0f,0.2f,1.0f);
	rplane *wall4 = new rplane();
	wall4->set_points(G,H,B,A);
	wall4->set_color(0.8f,0.0f,0.2f,1.0f);

	geometry.push_back(wall1);
	geometry.push_back(wall2);
	geometry.push_back(wall3);
	geometry.push_back(wall4);
}

int initialise_shaders_models() {
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

int parse_arguments(int argc, char *argv[]) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 2;
		}
	}
	return 0;
}

int init_anim(bool init_window) {
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

	/* PARSE ARGUMENTS */
	int arg_parse = parse_arguments(_argc, _argv);
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

	initialise_simulation();
	initialise_renderer();
	if (initialise_shaders_models() != 0) {
		return 1;
	}

	return 0;
}

int initialise_animation(int argc, char *argv[]) {
	_argc = argc;
	_argv = argv;
	init_anim(true);
}

} // -- namespace charanim
