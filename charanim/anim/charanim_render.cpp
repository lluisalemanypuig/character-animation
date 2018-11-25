#include <anim/charanim.hpp>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
using namespace physim::particles;

// charanim includes
#include <render/include_gl.hpp>
#include <render/shader/shader_helper.hpp>
#include <anim/vec_helper.hpp>

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

	for (int i = 0; i < 5; ++i) {
		S.apply_time_step();
	}

	glDisable(GL_LIGHTING);

	/* draw walls and stuff */
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (rgeom *r : geometry) {
		r->draw();
	}

	/* draw particles/spheres/models of characters */
	const vector<sized_particle *>& ps = S.get_sized_particles();
	if (draw_base_spheres) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 projection = V.make_projection_matrix();
		glm::mat4 view = V.make_view_matrix();

		flat_shader.bind();
		flat_shader.set_vec3("view_pos", glm::vec3(0.f,0.f,0.f));
		flat_shader.set_mat4("projection", projection);

		int i = 0;
		for (const sized_particle *p : ps) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, to_gvec3(p->cur_pos));
			model = glm::scale(model, glm::vec3(2.0f*p->R, 2.0f*p->R, 2.0f*p->R));

			glm::mat4 modelview = view*model;
			glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

			if (i == 0) {
				flat_shader.set_vec4("colour", glm::vec4(1.0f,0.0f,0.0f,1.0f));
			}
			else if (i == 1) {
				flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));
			}

			flat_shader.set_mat4("modelview", modelview);
			flat_shader.set_mat3("normal_matrix", normal_matrix);
			sphere->render();

			++i;
		}

		flat_shader.release();
	}
	else {
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glColor3f(0.0f,0.0f,1.0f);
		for (const sized_particle *p : ps) {
			glVertex3f(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
		}
		glEnd();
	}

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
	cout << "Clear memory..." << endl;

	sphere->clear();
	delete sphere;

	for (rgeom *r : geometry) {
		r->clear();
		delete r;
	}
	geometry.clear();

	flat_shader.clear();
	material_shader.clear();
	texture_shader.clear();
}

void special_keys_keyboard(int key, int x, int y) {

}

void regular_keys_keyboard(unsigned char c, int x, int y) {
	switch (c) {
		case ESC:
			glutDestroyWindow(window_id);
			break;

		case 's':
			draw_base_spheres = not draw_base_spheres;
			break;
	}
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
