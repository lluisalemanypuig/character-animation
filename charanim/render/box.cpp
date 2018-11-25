#include <render/box.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
#include <limits>
#include <cmath>
using namespace std;

// glm includes
#include <glm/glm.hpp>

// render includes
#include <render/include_gl.hpp>

// PRIVATE

static inline
void draw_edge(const glm::vec3& a, const glm::vec3& b) {
	glBegin(GL_LINES);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
	glEnd();
}

glm::vec3 box::make_vi(int i) const {
	switch (i) {
		case 0: return glm::vec3(min.x, min.y, max.z);
		case 1: return glm::vec3(max.x, min.y, max.z);
		case 2: return glm::vec3(max.x, min.y, min.z);
		case 3: return glm::vec3(min.x, min.y, min.z);
		case 4: return glm::vec3(min.x, max.y, max.z);
		case 5: return glm::vec3(max.x, max.y, max.z);
		case 6: return glm::vec3(max.x, max.y, min.z);
		case 7: return glm::vec3(min.x, max.y, min.z);
	}

	assert(i < 8);
	return glm::vec3(0.0f,0.0f,0.0f);
}

void box::update_vi() {
	for (int i = 0; i < 8; ++i) {
		vs[i] = make_vi(i);
	}
}

// PUBLIC

box::box() {
	float M = std::numeric_limits<float>::max();
	float m = -M;

	min = glm::vec3(M,M,M);
	max = glm::vec3(m,m,m);
	update_vi();

	VAO = VBO = EBO = 0;
}
box::~box() {
	clear();
}

void box::clear() {
	if (VAO > 0) {
		cout << "box::~box() - delete VAO " << VAO << endl;
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO > 0) {
		cout << "box::~box() - delete VBO " << VBO << endl;
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (EBO > 0) {
		cout << "box::~box() - delete EBO " << EBO << endl;
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
}

void box::set_min_max(const glm::vec3& m, const glm::vec3& M) {
	min = m;
	max = M;
	update_vi();
}

void box::enlarge_box(const glm::vec3& v) {
	min = glm::min(min, v);
	max = glm::max(max, v);
	update_vi();
}

void box::enlarge_box(const box& b) {
	min = glm::min(min, b.min);
	max = glm::max(max, b.max);
	update_vi();
}

void box::translate_box(const glm::vec3& t) {
	min += t;
	max += t;
	update_vi();
}

glm::vec3 box::get_min() const { return min; }
glm::vec3 box::get_max() const { return max; }

float box::length_x() const {return std::abs(max.x - min.x);}
float box::length_y() const {return std::abs(max.y - min.y);}
float box::length_z() const {return std::abs(max.z - min.z);}

// punt mig de la capsa
glm::vec3 box::get_middle_point() const {
	return glm::vec3((min.x + max.x)/2.0f,
				(min.y + max.y)/2.0f,
				(min.z + max.z)/2.0f);
}

// punt mig del pla inferior
glm::vec3 box::get_middle_bottom_point() const {
	return glm::vec3((min.x + max.x)/2.0f,
				 min.y,
				(min.z + max.z)/2.0f);
}

// punt mig del pla frontal
// una vegada rotada la capsa respecte l'eix vertical, no s'assegura
// que el punt frontal sigui el que es correspon amb la "part del davant" de la capsa
glm::vec3 box::get_middle_front_point() const {
	return glm::vec3((min.x + max.x)/2.0f,
				(min.y + max.y)/2.0f,
				 max.z);
}

float box::get_diag_length() const {
	return glm::distance(min, max);
}

float box::get_half_diag_length() const {
	return get_diag_length()/2.0f;
}

void box::make_buffers() {
	unsigned int indices[] = {
		0, 1, 2, 3,
		1, 2, 6, 5,
		2, 3, 7, 6,
		3, 0, 4, 7,
		4, 5, 6, 7
	};

	glGenVertexArrays(1, &VAO);
	assert(glGetError() == GL_NO_ERROR);

	GLuint buffs[2];
	glGenBuffers(2, buffs);
	assert(glGetError() == GL_NO_ERROR);

	VBO = buffs[0];
	EBO = buffs[1];

	glBindVertexArray(VAO);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(float), &vs[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);

	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);

	#if defined (DEBUG)
	cout << "box::make_buffers() - buffers made" << endl;
	cout << "    VAO: " << VAO << endl;
	cout << "    VBO: " << VBO << endl;
	cout << "    EBO: " << EBO << endl;
	#endif
}

void box::slow_render() const {
	draw_edge(vs[0], vs[1]);
	draw_edge(vs[1], vs[2]);
	draw_edge(vs[2], vs[3]);
	draw_edge(vs[3], vs[0]);

	draw_edge(vs[4], vs[5]);
	draw_edge(vs[5], vs[6]);
	draw_edge(vs[6], vs[7]);
	draw_edge(vs[7], vs[4]);

	for (int i = 0; i < 4; ++i) {
		draw_edge(vs[i], vs[i + 4]);
	}
}

void box::fast_render() const {
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 20, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

