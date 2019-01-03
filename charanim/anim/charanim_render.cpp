#include <anim/charanim.hpp>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// glm includes
#define GLM_FORCE_RADIANS
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

	static const double PI_1_4 = 1.0/4.0*M_PI;
	static const double PI_1_2 = 1.0/2.0*M_PI;
	static const double PI_3_4 = 3.0/4.0*M_PI;
	static const double PI_1_1 = M_PI;
	static const double PI_5_4 = 5.0/4.0*M_PI;
	static const double PI_3_2 = 3.0/2.0*M_PI;
	static const double PI_7_4 = 7.0/4.0*M_PI;
	static const double PI_2_1 = 2.0*M_PI;

	static inline
	bool inside_window(int x, int y) {
		int w = V.window_width();
		int h = V.window_height();
		return ((0 <= x) and (x <= w))
		   and ((0 <= y) and (y <= h));
	}

	void render_agent_vectors() {
		const vector<agent_particle>& as = S.get_agent_particles();
		for (const agent_particle& a : as) {
			if (render_velocity_vector) {
				glLineWidth(2.0f);
				glDisable(GL_LIGHTING);
				glColor3f(0.0f,1.0f,0.0f);
				glBegin(GL_LINES);
					glVertex3f(
						a.cur_pos.x,
						a.cur_pos.y + 1.5f*a.R,
						a.cur_pos.z);
					glVertex3f(
						a.cur_pos.x + a.cur_vel.x,
						a.cur_pos.y + a.cur_vel.y + 1.5f*a.R,
						a.cur_pos.z + a.cur_vel.z);
				glEnd();

				glLineWidth(2.0f);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f,0.0f,1.0f);
				glBegin(GL_LINES);
					glVertex3f(
						a.cur_pos.x,
						a.cur_pos.y + a.R,
						a.cur_pos.z);
					glVertex3f(
						a.cur_pos.x + 10.0f*a.cur_vel.x,
						a.cur_pos.y + 10.0f*a.cur_vel.y + a.R,
						a.cur_pos.z + 10.0f*a.cur_vel.z);
				glEnd();
			}
			if (render_target_vector) {
				glLineWidth(5.0f);
				glDisable(GL_LIGHTING);
				glColor3f(0.0f,1.0f,1.0f);
				glBegin(GL_LINES);
					glVertex3f(a.cur_pos.x, a.cur_pos.y, a.cur_pos.z);
					glVertex3f(a.target.x, a.target.y, a.target.z);
				glEnd();
			}
			if (render_orientation_vector) {
				glLineWidth(2.5f);
				glDisable(GL_LIGHTING);
				glColor3f(1.0f,1.0f,0.0f);
				glBegin(GL_LINES);
					glVertex3f(
						a.cur_pos.x,
						a.cur_pos.y + 2.0f*a.R,
						a.cur_pos.z);
					glVertex3f(
						a.cur_pos.x + 10.0f*a.orientation.x,
						a.cur_pos.y + 10.0f*a.orientation.y + 2.0f*a.R,
						a.cur_pos.z + 10.0f*a.orientation.z);
				glEnd();
			}
		}
	}

	void base_render() {
		glm::mat4 projection(1.0f), view(1.0f);
		V.make_projection_matrix(projection);
		V.make_view_matrix(view);
		view = glm::translate(view, glm::vec3(move_x, 0.0f, move_z));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);

		flat_shader.bind();
		flat_shader.set_bool("wireframe", false);
		flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));
		flat_shader.set_mat4("projection", projection);

		/* render sized particles */
		const vector<sized_particle>& ps = S.get_sized_particles();
		if (render_base_spheres and ps.size() > 0) {
			for (const sized_particle& p : ps) {
				glm::mat4 model(1.0f);
				model = glm::translate(model, to_gvec3(p.cur_pos));
				float R = 2.0f*p.R;
				model = glm::scale(model, glm::vec3(R, R, R));

				glm::mat4 modelview = view*model;
				glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				flat_shader.set_mat4("modelview", modelview);
				flat_shader.set_mat3("normal_matrix", normal_matrix);
				sphere->render();
			}
		}

		/* render agent particles */
		const vector<agent_particle>& as = S.get_agent_particles();
		if (render_base_spheres and as.size() > 0) {
			flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));

			for (const agent_particle& a : as) {
				glm::mat4 model(1.0f);
				model = glm::translate(model, to_gvec3(a.cur_pos));
				float R = 2.0f*a.R;
				model = glm::scale(model, glm::vec3(R, R, R));

				glm::mat4 modelview = view*model;
				glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				flat_shader.set_mat4("modelview", modelview);
				flat_shader.set_mat3("normal_matrix", normal_matrix);
				sphere->render();
			}
		}

		/* render attractors of agent particles */
		if (render_targets) {
			flat_shader.set_vec4("colour", glm::vec4(1.0f,0.0f,0.0f,1.0f));
			for (const agent_particle& a : as) {
				const vec3& att = a.target;
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(att.x, att.y, att.z));
				model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

				glm::mat4 modelview = view*model;
				glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				flat_shader.set_mat4("modelview", modelview);
				flat_shader.set_mat3("normal_matrix", normal_matrix);
				sphere->render();
			}
		}
		flat_shader.release();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (not render_base_spheres and as.size() > 0) {
			float ninety = static_cast<float>(M_PI)/2.0f;

			character_shader.bind();
			character_shader.set_mat4("projection", projection);
			for (size_t i = 0; i < as.size(); ++i) {
				rendered_character& C = characters[i];
				C.get_model()->update(0.01f);

				shader_helper::activate_materials_textures(C, character_shader);

				glm::mat4 model(1.0f);
				model = glm::translate(model, to_gvec3(as[i].cur_pos));

				vec2 pointy(0.0f, 1.0f);
				vec2 ori(as[i].orientation.x, as[i].orientation.z);
				float angle = dot(pointy, ori)/(norm(ori)) - ninety;
				model = glm::rotate(model, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, -ninety, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

				glm::mat4 modelview = view*model;
				glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				character_shader.set_mat4("modelview", modelview);
				character_shader.set_mat3("normal_matrix", normal_matrix);
				C.flatten_data();
				C.fill_buffers();
				C.render();
			}
			character_shader.release();
		}

		/* draw walls and stuff */
		glDisable(GL_LIGHTING);
		for (const rgeom *r : geometry) {
			if (r->get_model() == nullptr) {
				r->draw_geometry();
			}
		}

		glEnable(GL_LIGHTING);
		texture_shader.bind();
		texture_shader.set_vec3("view_pos", glm::vec3(0.0f,0.0f,0.0f));
		texture_shader.set_mat4("projection", projection);
		for (const rgeom *r : geometry) {
			if (r->get_model() != nullptr) {
				glm::mat4 model(1.0f);
				r->make_model_matrix(model);

				glm::mat4 modelview = view*model;
				glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				texture_shader.set_mat4("modelview", modelview);
				texture_shader.set_mat3("normal_matrix", normal_matrix);
				r->draw();
			}
		}
		texture_shader.release();
	}

	void full_render() {
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

		for (int i = 0; i < 10; ++i) {
			S.apply_time_step();
		}

		if (window_id != -1) {
			glutSwapBuffers();
		}
	}

	void timed_refresh(int v) {
		full_render();

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
		if (sphere != nullptr) {
			sphere->clear();
			delete sphere;
			sphere = nullptr;
		}

		for (rgeom *r : geometry) {
			r->clear();
			delete r;
		}
		geometry.clear();

		flat_shader.clear();
		material_shader.clear();
		texture_shader.clear();

		V.get_box().clear();
		S.clear_simulation();

		for (rendered_character& C : characters) {
			C.clear_buffers();
		}
		characters.clear();
	}

	void special_keys_keyboard(int key, int x, int y) {
		special_key = key;
		special_key_pressed = latticePoint(x,y);
	}

	void regular_keys_keyboard(unsigned char c, int x, int y) {
		switch (c) {
		case ESC:
			glutDestroyWindow(window_id);
			window_id = -1;
			break;

		case 's':
			render_base_spheres = not render_base_spheres;
			break;
		}
	}

	void mouse_passive(int x, int y) {
		last_mouse_moved = latticePoint(x,y);
	}

	void mouse_drag(int x, int y) {
		int dx = x - last_mouse_moved.x();
		int dy = y - last_mouse_moved.y();
		last_mouse_moved = latticePoint(x,y);

		double fov_deg = static_cast<double>(V.get_perspective_camera().get_FOV());
		double fov_rad = (M_PI/180.0)*fov_deg;
		double F = std::cos(fov_rad/2.0)/2.0;

		double psi_rad = (M_PI/180.0)*static_cast<double>(V.get_psi());
		if (psi_rad < 0.0) {
			psi_rad += 2*M_PI;
		}
		psi_rad = 2*M_PI - psi_rad;

		if (mouse_button == GLUT_LEFT_BUTTON) {
			V.increment_psi(-F*dx);
			V.increment_theta(F*dy);
		}
		else if (mouse_button == GLUT_RIGHT_BUTTON) {
			V.increment_zoom(F*dy);
		}
		else if (mouse_button == GLUT_MIDDLE_BUTTON) {
			if (0 <= psi_rad and psi_rad < PI_1_4) {
				move_x += F*dx;
				move_z += F*dy;
			}
			else if (PI_1_4 <= psi_rad and psi_rad < PI_1_2) {
				move_x -= F*dy;
				move_z += F*dx;
			}
			else if (PI_1_2 <= psi_rad and psi_rad < PI_3_4) {
				move_x -= F*dy;
				move_z += F*dx;
			}
			else if (PI_3_4 <= psi_rad and psi_rad < PI_1_1) {
				move_x -= F*dx;
				move_z -= F*dy;
			}
			else if (PI_1_1 <= psi_rad and psi_rad < PI_5_4) {
				move_x -= F*dx;
				move_z -= F*dy;
			}
			else if (PI_5_4 <= psi_rad and psi_rad < PI_3_2) {
				move_x += F*dy;
				move_z -= F*dx;
			}
			else if (PI_3_2 <= psi_rad and psi_rad < PI_7_4) {
				move_x += F*dy;
				move_z -= F*dx;
			}
			else if (PI_7_4 <= psi_rad and psi_rad < PI_2_1) {
				move_x += F*dx;
				move_z += F*dy;
			}
		}
	}

	void mouse_click(int button, int state, int x, int y) {
		mouse_button = button;
		mouse_state = state;
		last_mouse_click = latticePoint(x,y);
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

	/* render non-GLUT */

	void render_regular_grid(const regular_grid *r) {

		const float *cells = r->get_grid();
		const size_t rX = r->get_resX();
		const size_t rY = r->get_resY();

		const float dX = r->get_dimX();
		const float dY = r->get_dimY();
		const float max_dist = r->get_max_dist();

		const float lX = dX/rX;
		const float lY = dY/rY;

		glColor3f(1.0f,1.0f,0.0f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.5f, 0.0f);
			glVertex3f(5.0f, 0.5f, 0.0f);
			glVertex3f(0.0f, 0.5f, 5.0f);
		glEnd();

		if (render_grid) {
			glBegin(GL_LINES);
			glColor3f(0.7f, 0.7f, 0.7f);
			for (size_t x = 0; x < rX; ++x) {
				for (size_t y = 0; y < rY; ++y) {
					glVertex3f(x*lX, 0.2f, 0.0f);
					glVertex3f(x*lX, 0.2f, dY);

					glVertex3f(0.0f, 0.2f, y*lY);
					glVertex3f(dX, 0.2f, y*lY);
				}
			}
			glEnd();
		}
		else {
			glColor3f(0.2f,0.2f,0.2f);
			glBegin(GL_QUADS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(dX, 0.0f, 0.0f);
				glVertex3f(dX, 0.0f, dY);
				glVertex3f(0.0f, 0.0f, dY);
			glEnd();
		}

		if (render_dist_func) {
			float col;
			for (size_t cy = 0; cy < rY; ++cy) {
				for (size_t cx = 0; cx < rX; ++cx) {
					if (cy + 1 < rY and cx + 1 < rX) {
						// draw triangles
						glBegin(GL_TRIANGLES);
							col = cells[cy*rX + cx]/max_dist;
							glColor3f(col,col,col);
							glVertex3f(cx*lX, 0.1f, cy*lY);

							col = cells[cy*rX + cx + 1]/max_dist;
							glColor3f(col,col,col);
							glVertex3f((cx + 1)*lX, 0.1f, cy*lY);

							col = cells[(cy + 1)*rX + cx + 1]/max_dist;
							glColor3f(col,col,col);
							glVertex3f((cx + 1)*lX, 0.1f, (cy + 1)*lY);
						glEnd();
						glBegin(GL_TRIANGLES);
							col = cells[cy*rX + cx]/max_dist;
							glColor3f(col,col,col);
							glVertex3f(cx*lX, 0.1f, cy*lY);

							col = cells[(cy + 1)*rX + cx + 1]/max_dist;
							glColor3f(col,col,col);
							glVertex3f((cx + 1)*lX, 0.1f, (cy + 1)*lY);

							col = cells[(cy + 1)*rX + cx]/max_dist;
							glColor3f(col,col,col);
							glVertex3f(cx*lX, 0.1f, (cy + 1)*lY);
						glEnd();
					}
				}
			}
		}
	}

} // -- namespace charanim
