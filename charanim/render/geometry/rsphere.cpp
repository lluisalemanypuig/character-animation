#include <render/geometry/rsphere.hpp>

// C++ includes
#include <iostream>
using namespace std;

// glm inlcudes
#include <glm/gtc/matrix_transform.hpp>

// render includes
#include <render/include_gl.hpp>

rsphere::rsphere() : rgeom() {
	t = rendered_geometry_type::sphere;
}
rsphere::~rsphere() {}

// SETTERS

void rsphere::set_center(const glm::vec3& _c) {
	C = _c;
}

void rsphere::set_radius(float _r) {
	R = _r;
}

// GETETRS

const glm::vec3& rsphere::center() const {
	return C;
}

float rsphere::radius() const {
	return R;
}

// OTHERS

void rsphere::translate_object() const {
	glTranslatef(C.x,C.y,C.z);
	glScalef(R*2.0f,R*2.0f,R*2.0f);
}

void rsphere::draw_geometry() const {
	cout << "rsphere::draw_geometry() - Error!" << endl;
	cout << "    This object does not have a model and" << endl;
	cout << "    I don't know how to draw a sphere" << endl;
	cout << "    using glBegin(), glEnd()" << endl;
}

void rsphere::make_model_matrix(glm::mat4& mat) const {
	mat = glm::translate(mat, C);
	mat = glm::scale(mat, glm::vec3(R,R,R)*2.0f);
}

void rsphere::make_box(box& b) const {
	if (_model != nullptr) {
		b.set_min_max(C - glm::vec3(R,R,R), C + glm::vec3(R,R,R));
	}
	else {
		_model->make_box(b);
	}
}
