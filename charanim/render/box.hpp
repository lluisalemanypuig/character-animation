#pragma once

// C++ includes
#include <fstream>
#include <vector>

// glm includes
#include <glm/vec3.hpp>

class box {
	private:
		// all the vertices of the box
		// for faster drawing
		glm::vec3 vs[8];
		// Minimum and maximum coordinates
		glm::vec3 min, max;

		glm::vec3 make_vi(int i) const;
		void update_vi();

		/// vertex array object.
		uint VAO;
		/// vertices buffer object index
		uint VBO;
		/// indices object
		uint EBO;

	public:
		box();
		~box();

		void clear();

		inline friend
		std::ostream& operator<< (std::ostream& os, const box& b) {
			os << "(" << b.min.x << "," << b.min.y << "," << b.min.z << "),"
			   << "(" << b.max.x << "," << b.max.y << "," << b.max.z << ")";
			return os;
		}

		void set_min_max(const glm::vec3& m, const glm::vec3& M);
		void enlarge_box(const glm::vec3& v);
		void enlarge_box(const box& b);
		void translate_box(const glm::vec3& t);

		glm::vec3 get_min() const;
		glm::vec3 get_max() const;

		float length_x() const;
		float length_y() const;
		float length_z() const;

		glm::vec3 get_middle_point() const;
		glm::vec3 get_middle_bottom_point() const;
		glm::vec3 get_middle_front_point() const;
		float get_diag_length() const;
		float get_half_diag_length() const;

		void make_buffers();
		void slow_render() const;
		void fast_render() const;
};
