#pragma once

// C++ includes
#include <vector>

// glm includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// render includes
#include <render/box.hpp>
#include <render/triangle_mesh/triangle_mesh_utils.hpp>

/**
 * @brief Implements a mesh that need not be triangular.
 */
class triangle_mesh {
	protected:
		/// Mesh name. Used for debugging purposes.
		std::string mesh_name;

		// mesh data
		/// Vertices of the mesh.
		std::vector<glm::vec3> vertices;
		/// Normals of the mesh.
		std::vector<glm::vec3> normals;
		/**
		 * @brief Vertices indices of each face.
		 *
		 * The number of triangles is, then: triangles.size()/3
		 */
		std::vector<int> triangles;
		/**
		 * @brief Normal indices of each face.
		 *
		 * There must be as many normal indices as triangle indices.
		 */
		std::vector<int> normal_idxs;

	protected:
		/// Computes the normal of the plane containing triangle @e f.
		glm::vec3 triangle_normal(int F) const;

	public:
		/// Default constructor.
		triangle_mesh();
		/// Copy constructor.
		triangle_mesh(const triangle_mesh& m);
		/// Destructor.
		virtual ~triangle_mesh();

		// SETTERS

		/// Sets the name of the mesh.
		void set_name(const std::string& name);
		/// Sets the vertices of the mesh.
		void set_vertices(const std::vector<glm::vec3>& verts);
		/// Sets the normals of the mesh.
		void set_normals(const std::vector<glm::vec3>& nrmls);
		/// Sets the triangles of the mesh.
		void set_triangles(const std::vector<int>& tris);
		/// Sets the normals of the mesh.
		void set_normal_idxs(const std::vector<int>& nrmls_idxs);

		// GETTERS

		/**
		 * @brief Returns the state of the mesh.
		 *
		 * See @ref mesh_state for details.
		 */
		mesh_state state(const mesh_state& ignore = mesh_state::correct) const;

		/// Returns a constant reference to this mesh's vertices.
		const std::vector<glm::vec3>& get_vertices() const;

		/**
		 * @brief Make the bounding box of the mesh @e m.
		 *
		 * The box has its faces parallel to the axis.
		 * @param[out] b Bounding box
		 */
		void make_box(box& b) const;

		// MODIFIERS

		/**
		 * @brief Computes normals for the faces.
		 *
		 * Compute the normals of the faces. Replaces any
		 * existent normals in the mesh with the new ones.
		 *
		 * Since the faces' vertices are sorted in counterclockwise
		 * order, it is easy to compute normals using the
		 * cross product so that the normal points 'outside'
		 * the mesh.
		 *
		 * The normals are normalised.
		 */
		void make_normals_flat();
		/**
		 * @brief Computes normals for the vertices.
		 *
		 * Compute the normals of the vertices. Replaces any
		 * existent normals in the mesh with the new ones.
		 *
		 * The normal for a vertex is computed as the
		 * average of the normals of the neighbouring faces.
		 * A face's normal is the normal of the plane that
		 * contains a face.
		 *
		 * The normals are normalised.
		 */
		void make_normals_smooth();
		/**
		 * @brief Scales this mesh so that it fits in a unit length box.
		 *
		 * The mesh need not be centered at the (0,0,0).
		 */
		void scale_to_unit();

		/**
		 * @brief Clears the memory occupied by this mesh.
		 *
		 * Clears the contents of @ref vertices, @ref normals,
		 * @ref faces, @ref materials, @ref textures_coords,
		 * @ref textures_indexes.
		 *
		 * It also clears the memory occupied by the loaded
		 * textures, identified with the indexes in @ref texture_indexes.
		 */
		virtual void clear();

		// OTHERS

		/**
		 * @brief Outputs information about the mesh.
		 *
		 * Outputs the following information into standard output:
		 * - Number of vertices.
		 * - Number of triangles.
		 * - Number of normals.
		 * - Number of normals indices.
		 */
		virtual void display_mesh_info() const;
};

