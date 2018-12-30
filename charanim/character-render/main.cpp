// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <utility>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Cal3d includes
#include <cal3d/cal3d.h>
#include <cal3d/coremodel.h>
#include <cal3d/coresubmesh.h>

// render includes
#include <render/include_gl.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/shader/shader.hpp>
#include <render/scene/viewer.hpp>
#include <render/character/rendered_character.hpp>
#include <render/character/character_reader.hpp>

#include "utils.hpp"

typedef pair<int,int> point;

// ------------------
// global variables
// ------------------

static rendered_character C;
static viewer V;

static timing::time_point sec;
static int FPS;
static int fps_count;
static bool display_fps_count;

static int pressed_button;
static point last_mouse;

static bool lock_mouse;
static int window_id;

static shader character_shader;

// glut helpers
#define ESC 27
#define LEFT_ARROW 100
#define UP_ARROW 101
#define RIGHT_ARROW 102
#define DOWN_AROW 103

// -----------------------
// Global helper functions
// -----------------------

template<typename T>
static inline void UNUSED(const T& x) { (void)x; }

inline static
void draw_box(const glm::vec3& vmin, const glm::vec3& vmax) {
	glBegin(GL_LINES);
		// -----------------------------
		glVertex3f(vmin.x, vmin.y, vmin.z);
		glVertex3f(vmax.x, vmin.y, vmin.z);

		glVertex3f(vmax.x, vmin.y, vmin.z);
		glVertex3f(vmax.x, vmin.y, vmax.z);

		glVertex3f(vmax.x, vmin.y, vmax.z);
		glVertex3f(vmin.x, vmin.y, vmax.z);

		glVertex3f(vmin.x, vmin.y, vmax.z);
		glVertex3f(vmin.x, vmin.y, vmin.z);

		// -----------------------------
		glVertex3f(vmin.x, vmax.y, vmin.z);
		glVertex3f(vmax.x, vmax.y, vmin.z);

		glVertex3f(vmax.x, vmax.y, vmin.z);
		glVertex3f(vmax.x, vmax.y, vmax.z);

		glVertex3f(vmax.x, vmax.y, vmax.z);
		glVertex3f(vmin.x, vmax.y, vmax.z);

		glVertex3f(vmin.x, vmax.y, vmax.z);
		glVertex3f(vmin.x, vmax.y, vmin.z);

		// -----------------------------
		glVertex3f(vmin.x, vmin.y, vmin.z);
		glVertex3f(vmin.x, vmax.y, vmin.z);

		glVertex3f(vmax.x, vmin.y, vmin.z);
		glVertex3f(vmax.x, vmax.y, vmin.z);

		glVertex3f(vmax.x, vmin.y, vmax.z);
		glVertex3f(vmax.x, vmax.y, vmax.z);

		glVertex3f(vmin.x, vmin.y, vmax.z);
		glVertex3f(vmin.x, vmax.y, vmax.z);
	glEnd();
}

// -----------
// Exit viewer
// -----------

void exit_func() {

}

// -----------------
// INITIALISE OpenGL
// -----------------

int initGL(int argc, char *argv[]) {

	// initial window size
	int iw = 640;
	int ih = 480;

	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(iw, ih);
	window_id = glutCreateWindow("Basic viewer");

	GLenum err = glewInit();
	if (err != 0) {
		cerr << "initGL - Error:" << endl;
		cerr << "    when initialising glew: " << err << endl;
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	float diff[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	float spec[] = {0.1f, 0.1f, 0.1f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
	float amb[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	float pos[] = {1.0f, -1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// --------------------------- //
	/* initialise global variables */
	pressed_button = 0;
	last_mouse = point(0,0);
	lock_mouse = false;

	display_fps_count = false;
	FPS = 60;
	fps_count = 0;

	shared_ptr<CalCoreModel> core_model = nullptr;
	shared_ptr<CalModel> model = nullptr;

	bool res = character_reader::load_core_model(
		"../../characters", "paladin.cfg", "dummy",
		core_model, model
	);

	if (not res) {
		cerr << "Error: when loading model" << endl;
		return 1;
	}

	C.set_cal_info(core_model, model);
	C.initialise_buffers();

	bool r = character_shader.init
			("../../charanim/shaders", "character.vert", "character.frag");
	if (not r) { return false; }

	/*
	character_shader.bind();
	shader_helper::activate_materials_textures(C, character_shader);
	character_shader.release();
	*/

	glm::vec3 vmin, vmax;
	C.get_bounding_box(vmin, vmax);

	V.set_window_dims(iw, ih);
	V.get_box().set_min_max(vmin, vmax);
	V.init_cameras();

	sec = timing::now();

	return 0;
}

// ------------
// RENDER SCENE
// ------------

void refresh() {
	++fps_count;
	C.get_model()->update(0.01f);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* render character */

	/*
	glm::mat4 projection(1.0f), view(1.0f);
	V.make_projection_matrix(projection);
	V.make_view_matrix(view);
	glm::mat4 model(1.0f);
	glm::mat4 modelview = view*model;
	glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

	character_shader.bind();
	character_shader.set_vec3("view_pos", glm::vec3(0.0f,0.0f,0.0f));
	character_shader.set_mat4("projection", projection);
	character_shader.set_mat4("modelview", modelview);
	character_shader.set_mat3("normal_matrix", normal_matrix);
	C.fill_buffers();
	C.render();
	character_shader.release();
	*/

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	V.apply_projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	V.apply_view();

	glm::vec3 vmin, vmax;
	C.get_bounding_box(vmin, vmax);

	glPushMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//C.flatten_data();
		C.draw();

		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 0.0f, 0.0f);
		draw_box(vmin, vmax);
	glPopMatrix();

	glutSwapBuffers();
}

void timed_refresh(int value) {
	UNUSED(value);

	refresh();

	timing::time_point here = timing::now();
	double elapsed = timing::elapsed_seconds(sec, here);
	if (elapsed >= 1.0) {
		if (display_fps_count) {
			cout << "fps " << fps_count << " (" << FPS << ")" << endl;
		}
		fps_count = 0;
		sec = timing::now();
	}

	glutTimerFunc(1000.0f/FPS, timed_refresh, 0);
}

// ---------------
// RESIZE viewport
// ---------------

void reshape(int w, int h) {
	float pzoom = V.get_perspective_camera().get_zoom();
	float ozoom = V.get_orthogonal_camera().get_zoom();

	V.set_window_dims(w, h);
	V.init_cameras();

	V.get_perspective_camera().set_zoom(pzoom);
	V.get_orthogonal_camera().set_zoom(ozoom);

	glViewport(0, 0, w, h);
}

// ++++++++++++++++++
// EVENTS
// ++++++++++++++++++

// -------------
// MOUSE HANDLER
// -------------

void mouse_click_event(int button, int state, int x, int y) {
	UNUSED(x);
	UNUSED(y);
	UNUSED(state);

	pressed_button = button;
}

void mouse_movement(int x, int y) {
	//int dx = x - last_mouse.first;
	//int dy = y - last_mouse.second;
	last_mouse = point(x,y);
}

void mouse_drag_event(int x, int y) {
	int dx = x - last_mouse.first;
	int dy = y - last_mouse.second;
	last_mouse = point(x,y);

	if (pressed_button == GLUT_LEFT_BUTTON) {
		V.increment_psi(-0.3f*dx);
		V.increment_theta(0.3f*dy);
	}
	else if (pressed_button == GLUT_RIGHT_BUTTON) {
		V.increment_zoom(0.75f*dy);
	}
}

// ----------------
// KEYBOARD HANDLER
// ----------------

void special_keys(int key, int x, int y) {
	UNUSED(key);
	UNUSED(x);
	UNUSED(y);
}

void keyboard_event(unsigned char c, int x, int y) {
	UNUSED(x);
	UNUSED(y);

	if (c == ESC) {
		glutDestroyWindow(window_id);
	}
	else if (c == 'p') {
		V.switch_to_perspective();
	}
	else if (c == 'o') {
		V.switch_to_orthogonal();
	}
	else if (c == '+') {
		if (FPS < 59) {
			++FPS;
		}
	}
	else if (c == '-') {
		if (FPS > 1) {
			--FPS;
		}
	}
	else if (c == 'z') {
		display_fps_count = not display_fps_count;
	}
}

int main(int argc, char* argv[]) {
	int r = initGL(argc, argv);
	if (r != 0) {
		return r;
	}

	atexit(exit_func);

	glutDisplayFunc(refresh);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_event);
	glutSpecialFunc(special_keys);
	glutMouseFunc(mouse_click_event);
	glutPassiveMotionFunc(mouse_movement);
	glutMotionFunc(mouse_drag_event);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, timed_refresh, 0);

	glutMainLoop();
}



