#pragma once

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rgeometry.hpp>
#include <render/model/rendered_model.hpp>
#include <render/box.hpp>

/*
 * Simple wrapper on the geometry of the
 * simulator for easy rendering.
 *
 */

// use the center to translate a model
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
