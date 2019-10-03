/*********************************************************************
 * charanim - Character Animation Project
 * Copyright (C) 2018 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
	cout << "    This object does not have a mesh and" << endl;
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
