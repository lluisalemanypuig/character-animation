// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <utility>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// render includes
#include <render/include_gl.hpp>
#include <render/scene/viewer.hpp>

// custom includes
#include "utils.hpp"

typedef pair<int,int> point;

// ------------------
// global variables
// ------------------

static viewer V;
static timing::time_point sec;

static int FPS;
static int fps_count;
static bool display_fps_count;

static int pressed_button;
static point last_mouse;

static bool lock_mouse;
static int window_id;

static int iw, ih;
static float dispx, dispy;

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

static inline
bool inside_window(int x, int y) {
	return ((0 <= x) and (x <= V.window_width()))
	   and ((0 <= y) and (y <= V.window_height()));
}

// -----------
// Exit viewer
// -----------

void exit_func() {

}

// -----------------
// INITIALISE OpenGL
// -----------------

void initGL(int argc, char *argv[]) {
	// initial window size
	iw = 640;
	ih = 480;

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
	float col[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
	float pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	float amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

	// --------------------------- //
	/* initialise global variables */
	pressed_button = 0;
	last_mouse = point(0,0);
	lock_mouse = false;

	sec = timing::now();
	display_fps_count = false;
	FPS = 60;
	fps_count = 0;

	dispx = dispy = 0.0f;

	V.get_box().set_min_max(
		glm::vec3(-4.0f,-1.0f,-1.0f),
		glm::vec3( 4.0f, 8.0f, 1.0f)
	);

	V.set_window_dims(iw, ih);
	V.init_cameras();

	glPointSize(7);
}

// ------------
// RENDER SCENE
// ------------

void refresh() {
	++fps_count;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	V.apply_projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	V.apply_view();

	glDisable(GL_LIGHTING);
	glTranslatef(dispx, dispy, 0.0f);

	glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(-1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 0.0f, 0.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f);

		glColor3f(0.2f, 1.0f, 0.2f);
		glVertex3f( 2.0f, 0.0f, 0.0f);
		glVertex3f( 3.0f, 0.0f, 0.0f);
		glVertex3f( 2.5f, 1.0f, 0.0f);

		glColor3f(1.0f, 1.0f, 0.2f);
		glVertex3f(-4.0f, 5.0f, 0.0f);
		glVertex3f(-2.0f, 5.0f, 0.0f);
		glVertex3f(-3.0f, 7.0f, 0.0f);
	glEnd();

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

	cout << "device coordinates: " << x << "," << y << endl;
	float dx = (2.0f*x)/iw - 1.0f;
	float dy = 1.0f - ((2.0f*y)/ih);

	glm::mat4 proj(1.0f), view(1.0f);
	V.make_projection_matrix(proj);
	V.make_view_matrix(view);
	view = glm::translate(view, glm::vec3(dispx, dispy, 0.0f));

	glm::mat4 proj_modelview = view*proj;
	glm::mat4 inv_mat = glm::inverse(proj_modelview);

	glm::vec4 v(dx, dy, 1.0f, 1.0f);

	// device = proj*view*model*world
	// world = modl_inv*view_inv*proj_inv*device

	glm::vec4 world = v*inv_mat;
	world.x /= world.w;
	world.y /= world.w;
	world.z /= world.w;

	cout << "world: " << world.x << "," << world.y << "," << world.z << endl;

	pressed_button = button;
}

void mouse_movement(int x, int y) {
	int dx = x - last_mouse.first;
	int dy = y - last_mouse.second;
	last_mouse = point(x,y);

}

void mouse_drag_event(int x, int y) {
	int dx = x - last_mouse.first;
	int dy = y - last_mouse.second;
	last_mouse = point(x,y);

	if (pressed_button == GLUT_LEFT_BUTTON) {
		dispx += (dx*float(iw)/ih)/10.0f;
		dispy -= (dy*float(iw)/ih)/10.0f;
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

	switch (c) {
	case 'q':
		exit_func();
		glutDestroyWindow(window_id);
		break;
	case 'x':
		display_fps_count = not display_fps_count;
		break;
	}
}

int main(int argc, char* argv[]) {
	initGL(argc, argv);

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
