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
