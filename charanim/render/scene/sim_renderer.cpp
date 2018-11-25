#include <render/scene/sim_renderer.hpp>

// C includes
#include <assert.h>

// render includes
#include <render/include_gl.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/meshes/mesh.hpp>
#include <physim/meshes/mesh1d.hpp>
#include <physim/meshes/mesh2d_regular.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace meshes;
using namespace particles;
typedef physim::math::vec3 pm_vec3;

inline
void rainbow(float v, float m, float M, float& r, float& g, float& b) {
	float s = (v - m)/(M - m);

	if (s <= 0.0f) {
		// RED
		// below 0.0
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}
	else if (s <= 0.2f) {
		// RED to YELLOW
		// from 0.0 to 0.2
		r = 1.0f;
		g = 5.0f*(s - 0.0f);
		b = 0.0f;
	}
	else if (s <= 0.4f) {
		// YELLOW to GREEN
		// from 0.2 to 0.4
		r = -5.0f*s + 2.0f;
		g = 1.0f;
		b = 0.0f;
	}
	else if (s <= 0.6f) {
		// GREEN to CYAN
		// from 0.4 to 0.6
		r = 0.0f;
		g = 1.0f;
		b = 5.0f*(s - 0.4f);
	}
	else if (s <= 0.8f) {
		// CYAN to BLUE
		// from 0.6 to 0.8
		r = 0.0f;
		g = -5.0f*s + 4.0f;
		b = 1.0f;
	}
	else if (s <= 1.0f) {
		// BLUE to PURPLE
		// from 0.8 to 1.0
		r = 5.0f*(s - 0.8f);
		g = 0.0f;
		b = 1.0f;
	}
	else if (1.0f <= s) {
		// PURPLE
		// above 1.0
		r = 1.0f;
		g = 0.0f;
		b = 1.0f;
	}
}

void render_mesh1d(const mesh1d *m) {
	particles::mesh_particle *const *ps = m->get_particles();

	float r, g, b;
	glBegin(GL_LINES);
	for (size_t i = 0; i < m->size() - 1; ++i) {
		const pm_vec3& pos1 = ps[i]->cur_pos;
		const pm_vec3& pos2 = ps[i + 1]->cur_pos;

		rainbow(i, 0,m->size(), r,g,b);
		glColor3f(r,g,b);
		glVertex3f(pos1.x, pos1.y, pos1.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);
	}
	glEnd();
}

void render_mesh2d_regular(const mesh2d_regular *m) {
	particles::mesh_particle *const *ps = m->get_particles();

	size_t N,M;
	m->get_dimensions(N,M);

	glBegin(GL_LINES);
	glColor3f(1.0f,1.0f,1.0f);
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			if (i + 1 < N) {
				const pm_vec3& pos1 = ps[m->get_global_index(i,j)]->cur_pos;
				glVertex3f(pos1.x, pos1.y, pos1.z);
				const pm_vec3& pos2 = ps[m->get_global_index(i + 1,j)]->cur_pos;
				glVertex3f(pos2.x, pos2.y, pos2.z);
			}
			if (j + 1 < M) {
				const pm_vec3& pos1 = ps[m->get_global_index(i,j)]->cur_pos;
				glVertex3f(pos1.x, pos1.y, pos1.z);
				const pm_vec3& pos2 = ps[m->get_global_index(i,j + 1)]->cur_pos;
				glVertex3f(pos2.x, pos2.y, pos2.z);
			}
		}
	}
	glEnd();
}

// PRIVATE

// PUBLIC

sim_renderer::sim_renderer() : viewer() { }
sim_renderer::~sim_renderer() {
	clear();
}

void sim_renderer::clear() {
	viewer::clear();
	for (rgeom *g : geometry) {
		g->clear();
		delete g;
	}
	geometry.clear();
	B.clear();
	S.clear_simulation();
}

void sim_renderer::set_particle_size(float s) {
	glPointSize(s);
}

void sim_renderer::set_spring_width(float w) {
	glLineWidth(w);
}

void sim_renderer::add_geometry(rgeom *r) {
	assert(r != nullptr);

	geometry.push_back(r);
	box b;
	r->make_box(b);
	B.enlarge_box(b);
}

const std::vector<rgeom *>& sim_renderer::get_geometry() const {
	return geometry;
}

physim::simulator& sim_renderer::get_simulator() {
	return S;
}

void sim_renderer::apply_time_step() {
	S.apply_time_step();
}

void sim_renderer::render_geometry() const {
	for (rgeom *r : geometry) {
		r->draw();
	}
}

void sim_renderer::render_simulation() const {
	glDisable(GL_LIGHTING);

	// render particles
	glBegin(GL_POINTS);
	const std::vector<free_particle *>& ps = S.get_free_particles();
	for (const free_particle *p : ps) {
		if (p->lifetime < 0.2f) {
			glColor3f(0.0f,0.0f,0.0f);
		}
		else {
			glColor3f(1.0f,1.0f,1.0f);
		}
		glVertex3f(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
	}
	glEnd();

	// render springs

	const std::vector<mesh *>& mss = S.get_meshes();
	for (const mesh *m : mss) {

		if (m->get_type() == mesh_type::d1) {
			render_mesh1d(static_cast<const mesh1d *>(m));
		}
		else if (m->get_type() == mesh_type::d2_regular) {
			render_mesh2d_regular(static_cast<const mesh2d_regular *>(m));
		}
	}
}
