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

#include <render/geometry/rtriangle.hpp>

// render includes
#include <render/include_gl.hpp>

rtriangle::rtriangle() : rgeom() {
	t = rendered_geometry_type::triangle;
}
rtriangle::~rtriangle() {}

// SETTERS

void rtriangle::set_points
(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	_p1 = p1;
	_p2 = p2;
	_p3 = p3;
}

// MODIFIERS

const glm::vec3& rtriangle::p1() const { return _p1; }
const glm::vec3& rtriangle::p2() const { return _p2; }
const glm::vec3& rtriangle::p3() const { return _p3; }

// OTHERS

void rtriangle::draw_geometry() const {
	glColor4f(r,g,b,a);
	glBegin(GL_TRIANGLES);
		glVertex3f(_p1.x, _p1.y, _p1.z);
		glVertex3f(_p2.x, _p2.y, _p2.z);
		glVertex3f(_p3.x, _p3.y, _p3.z);
	glEnd();
}

void rtriangle::make_model_matrix(glm::mat4& ) const {}

void rtriangle::make_box(box& b) const {
	b.set_min_max(_p1,_p1);
	b.enlarge_box(_p2);
	b.enlarge_box(_p3);
}
