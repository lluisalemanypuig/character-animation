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

// C++ includes
#include <memory>

// glm includes
#include <glm/mat4x4.hpp>

// render includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/box.hpp>

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

enum class rendered_geometry_type : int8_t {
	none = -1,
	plane,
	triangle,
	rectangle,
	sphere
};

class rgeom {
	protected:
		bool render;
		rendered_geometry_type t;
		std::shared_ptr<rendered_triangle_mesh> _model;

		// flat color of the mesh
		float r,g,b,a;

		/**
		 * @brief Translational operations
		 *
		 * An object may be needed to be translated,
		 * like a sphere. Therefore, this method is
		 * implemented using glTranslate and/or glRotate.
		 *
		 * Planes, triangles, and rectangles need not
		 * implement this method since they are given
		 * their coordinates and are generated using
		 * glBegin, glEnd.
		 */
		virtual void translate_object() const;

	public:
		rgeom();
		virtual ~rgeom();

		// MODIFIERS

		void clear();

		// SETTERS

		void set_render(bool r);
		void set_color(float _r, float _g, float _b, float _a);
		void set_model(const std::shared_ptr<rendered_triangle_mesh>& _model);

		// GETTERS

		bool should_render() const;
		rendered_geometry_type get_type() const;
		std::shared_ptr<rendered_triangle_mesh> get_model();
		const std::shared_ptr<rendered_triangle_mesh> get_model() const;

		float red() const;
		float green() const;
		float blue() const;
		float alpha() const;

		// OTHERS

		/**
		 * @brief Draws the geometry.
		 *
		 * In case this geometry has a rendered mesh associated
		 * (see @ref mesh) then its method
		 * @ref rendered_mesh::render() is called. Otherwise,
		 * the function calls the pure virtual method
		 * @ref draw_geometry().
		 *
		 * For the former case, an extra function is called:
		 * @ref translate_object. This function contains
		 * calls to glTranslatef and glRotatef to place the
		 * object where it should be.
		 */
		void draw() const;

		/**
		 * @brief Draws the geometry.
		 *
		 * Using glBegin, glEnd, for example.
		 */
		virtual void draw_geometry() const = 0;

		virtual void make_model_matrix(glm::mat4& mat) const = 0;

		/// Makes the bounding box of this mesh.
		virtual void make_box(box& b) const = 0;
};





