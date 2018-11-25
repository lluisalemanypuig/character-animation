#include <anim/charanim.hpp>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// physim includes
#include <physim/particles/free_particle.hpp>
using namespace physim::particles;

// charanim includes
#include <render/include_gl.hpp>

namespace charanim {

static inline
bool inside_window(int x, int y) {
	int w = V.window_width();
	int h = V.window_height();
	return ((0 <= x) and (x <= w))
	   and ((0 <= y) and (y <= h));
}

void refresh() {
	glClearColor(bgd_color.x, bgd_color.y, bgd_color.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// no shader for all
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	V.apply_projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	V.apply_view();

	for (int i = 0; i < 10; ++i) {
		S.apply_time_step();
	}

	const vector<sized_particle *>& ps = S.get_sized_particles();
	glBegin(GL_POINTS);
	for (const sized_particle *p : ps) {
		glVertex3f(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
	}
	glEnd();

	glutSwapBuffers();
}

void timed_refresh(int v) {
	refresh();

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

	glutTimerFunc(1000.0f/FPS, timed_refresh, v);
}

void exit_func() {
}

void special_keys_keyboard(int key, int x, int y) {

}

void regular_keys_keyboard(unsigned char c, int x, int y) {

}

void mouse_passive(int x, int y) {

	last_mouse_moved = point(x,y);
}

void mouse_drag(int x, int y) {
	int dx = x - last_mouse_moved.first;
	int dy = y - last_mouse_moved.second;
	last_mouse_moved = point(x,y);

	if (pressed_button == GLUT_LEFT_BUTTON) {
		if (V.is_inspecting()) {
			V.increment_psi(-0.3f*dx);
			V.increment_theta(0.3f*dy);
		}
	}
	else if (pressed_button == GLUT_RIGHT_BUTTON) {
		V.increment_zoom(0.75f*dy);
	}
}

void mouse_click(int button, int state, int x, int y) {

	pressed_button = button;
	last_mouse_click = point(x,y);
}

void resize(int w, int h) {
	width = w;
	height = h;

	float pzoom = V.get_perspective_camera().get_zoom();
	float ozoom = V.get_orthogonal_camera().get_zoom();

	V.set_window_dims(width, height);
	V.init_cameras();

	V.get_perspective_camera().set_zoom(pzoom);
	V.get_orthogonal_camera().set_zoom(ozoom);

	glViewport(0, 0, width, height);
}

} // -- namespace charanim
