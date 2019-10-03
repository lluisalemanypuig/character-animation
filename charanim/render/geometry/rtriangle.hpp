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

#pragma once

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rgeometry.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/box.hpp>

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

// use the three points (they should be
// on the triangle) to render the plane.
class rtriangle : public rgeom {
	private:
		glm::vec3 _p1,_p2,_p3;

	public:
		rtriangle();
		~rtriangle();

		// SETTERS

		void set_points(const glm::vec3& p1, const glm::vec3& p2,
						const glm::vec3& p3);

		// GETTERS

		const glm::vec3& p1() const;
		const glm::vec3& p2() const;
		const glm::vec3& p3() const;

		// OTHERS

		void draw_geometry() const;
		void make_model_matrix(glm::mat4& triangle_mesh) const;
		void make_box(box& b) const;
};
