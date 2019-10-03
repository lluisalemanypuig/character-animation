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

// use the center to translate a mesh
// of a sphere to the right position.
class rsphere : public rgeom {
	private:
		glm::vec3 C;
		float R;

	public:
		rsphere();
		~rsphere();

		// SETTERS

		void set_center(const glm::vec3& _c);
		void set_radius(float _r);

		// GETETRS

		const glm::vec3& center() const;
		float radius() const;

		// OTHERS

		void translate_object() const;
		void draw_geometry() const;
		void make_model_matrix(glm::mat4& mat) const;
		void make_box(box& b) const;
};
