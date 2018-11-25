#include <render/scene/viewer.hpp>

// C incldues
#include <assert.h>

// C++ includes
#include <iostream>
#include <cmath>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// render includes
#include <render/include_gl.hpp>
#include <render/shader/shader_helper.hpp>

// PRIVATE

#define TO_RAD float(M_PI)/180.0f

// PROTECTED

float viewer::get_aspect_ratio() const {
	return float(win_width)/float(win_height);
}

// PUBLIC

viewer::viewer() {
	cam_pos = glm::vec3(0.0f,0.0f,0.0f);
	pitch = 0.0f;
	yaw = 0.0f;

	pitch = 0.0f;
	yaw = 0.0f;

	use_perspective = true;
	use_orthogonal = false;

	inspect = true;
	fly = false;
}

viewer::~viewer() {
	clear();
}

void viewer::clear() { }

void viewer::set_window_dims(int w, int h) {
	win_width = w;
	win_height = h;
}

void viewer::init_cameras() {
	orth_cam.init_camera(B);
	pers_cam.init_camera(B);
	orth_cam.adapt_camera_to_viewport(B, win_width, win_height);
	pers_cam.adapt_camera_to_viewport(win_width, win_height);

	orth_cam.set_znear(0.1f);
	orth_cam.set_zfar(1.5f*orth_cam.get_zfar());

	pers_cam.set_znear(0.1f);
	pers_cam.set_zfar(1.5f*pers_cam.get_zfar());

	VRP = B.get_middle_point();
	diag_length = B.get_diag_length();

	cam_pos = VRP;
	cam_pos.z += 5.0f;
}

void viewer::increment_theta(float i)	{ theta += i; }
void viewer::increment_psi(float i)	{ psi += i; }
void viewer::increment_pitch(float i)	{ pitch += i; }
void viewer::increment_yaw(float i)	{ yaw += i; }

void viewer::increment_zoom(float i) {
	pers_cam.zoom(i);
	orth_cam.zoom(0.142f*i);
}

void viewer::move_camera(float vel, float dir) {
	float rad = (yaw + dir)*TO_RAD;
	cam_pos.x -= sin(rad)*vel;
	cam_pos.z -= cos(rad)*vel;
}

void viewer::tilt_camera_up(float vel, float dir) {
	float rad = (pitch + dir)*TO_RAD;
	cam_pos.y += sin(rad)*vel;
}

void viewer::camera_forwards(float vel) {
	if (pitch != 90.0f and pitch != -90.0f) {
		move_camera(vel, 0.0f);
	}
	tilt_camera_up(vel, 0.0f);
}

void viewer::camera_backwards(float vel) {
	if (pitch != 90.0f and pitch != -90.0f) {
		move_camera(vel, 180.0f);
	}
	tilt_camera_up(vel, 180.0f);
}

void viewer::camera_sideways_left(float vel) {
	move_camera(vel, 90.0);
}

void viewer::camera_sideways_right(float vel) {
	move_camera(vel, 270.0);
}

void viewer::switch_to_perspective(){ use_perspective = true; use_orthogonal = false; }
void viewer::switch_to_orthogonal()	{ use_perspective = false; use_orthogonal = true; }

void viewer::switch_to_inspection()	{ inspect = true; fly = false; }
void viewer::switch_to_flight()		{ inspect = false; fly = true; }

// SETTERS

void viewer::set_perspective(const perspective& p) { pers_cam = p; }
void viewer::set_orthogonal(const orthogonal& o) { orth_cam = o; }
void viewer::set_VRP(const glm::vec3& _VRP) { VRP = _VRP; }
void viewer::set_theta(float t) { theta = t; }
void viewer::set_psi(float p) { psi = p; }
void viewer::set_viewer_pos(const glm::vec3& pos) { cam_pos = pos; }
void viewer::set_yaw(float y) { yaw = y; }
void viewer::set_pitch(float p) { pitch = p; }

// GETTERS

perspective& viewer::get_perspective_camera() {
	return pers_cam;
}

orthogonal& viewer::get_orthogonal_camera() {
	return orth_cam;
}

const glm::vec3& viewer::get_viewer_pos() const {
	return cam_pos;
}

box& viewer::get_box() {
	return B;
}

const box& viewer::get_box() const {
	return B;
}

int viewer::window_width() const {
	return win_width;
}
int viewer::window_height() const {
	return win_height;
}

bool viewer::is_flying() const {
	return fly;
}
bool viewer::is_inspecting() const {
	return inspect;
}

float viewer::get_yaw() const	{ return yaw; }
float viewer::get_pitch() const{ return pitch; }

const glm::vec3& viewer::get_VRP() const { return VRP; }
float viewer::get_theta() const { return theta; }
float viewer::get_psi() const { return psi; }

// OpenGL

void viewer::apply_projection() const {
	if (use_perspective) {
		gluPerspective(
			pers_cam.get_FOV(), pers_cam.getRAw(),
			pers_cam.get_znear(), pers_cam.get_zfar()
		);
	}
	else if (use_orthogonal) {
		glOrtho(
			orth_cam.get_left(), orth_cam.get_right(),
			orth_cam.get_bottom(), orth_cam.get_top(),
			orth_cam.get_znear(), orth_cam.get_zfar()
		);
	}
	else {
		cerr << "simulation_renderer::apply_view_mode() - Error:" << endl;
		cerr << "    Something went wrong with the cameras." << endl;
		cerr << "    No perspective or orthogonal camera activated" << endl;
		assert(false);
	}
}

glm::mat4 viewer::make_projection_matrix() const {
	glm::mat4 proj(1.0f);

	if (use_perspective) {
		proj = glm::perspective(
			pers_cam.get_FOV()*TO_RAD, pers_cam.getRAw(),
			pers_cam.get_znear(), pers_cam.get_zfar()
		);
	}
	else if (use_orthogonal) {
		proj = glm::ortho(
			orth_cam.get_left(), orth_cam.get_right(),
			orth_cam.get_bottom(), orth_cam.get_top(),
			orth_cam.get_znear(), orth_cam.get_zfar()
		);
	}
	else {
		cerr << "simulation_renderer::apply_view_mode() - Error:" << endl;
		cerr << "    Something went wrong with the cameras." << endl;
		cerr << "    No perspective or orthogonal camera activated" << endl;
		assert(false);
	}

	return proj;
}

void viewer::apply_view() const {
	if (inspect) {
		glTranslatef(0.0f, 0.0f, -diag_length);
		glRotatef(theta, 1.0f, 0.0f, 0.0f);
		glRotatef(-psi, 0.0f, 1.0f, 0.0f);
		glTranslatef(-VRP.x, -VRP.y, -VRP.z);
	}
	else if (fly) {
		glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
		glTranslatef(-cam_pos.x, -cam_pos.y, -cam_pos.z);
	}
	else {
		cerr << "simulation_renderer::apply_camera() - Error:" << endl;
		cerr << "    Something went wrong with the cameras." << endl;
		cerr << "    No inspect or fly mode activated" << endl;
		assert(false);
	}
}

glm::mat4 viewer::make_view_matrix() const {
	glm::mat4 view(1.0f);
	if (inspect) {
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -diag_length));
		view = glm::rotate(view, theta*TO_RAD, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -psi*TO_RAD, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(-VRP.x, -VRP.y, -VRP.z));
	}
	else if (fly) {
		view = glm::rotate(view, -pitch*TO_RAD, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -yaw*TO_RAD, glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(-cam_pos.x, -cam_pos.y, -cam_pos.z));
	}
	else {
		cerr << "simulation_renderer::apply_camera() - Error:" << endl;
		cerr << "    Something went wrong with the cameras," << endl;
		cerr << "    No inspect or fly mode activated" << endl;
		assert(false);
	}

	return view;
}
