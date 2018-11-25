#pragma once

// C++ includes
#include <vector>
#include <string>

// glm includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// Custom includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/triangle_mesh/triangle_mesh_utils.hpp>

/**
 * @brief Wavefront format file reader class.
 *
 * This class is used to parse files describing meshes
 * in wavefront format. This is done via the
 * @ref load_object method.
 *
 * This reader does not modify the data read,
 * and does not store the mesh. Instead, the data
 * is dumped into a data structure.
 *
 * Finally, note that this reader is rather simplified.
 * It is not guaranteed that all .obj can be correctly
 * read by this class.
 */
class OBJ_reader {
	private:
		/**
		 * @brief Name of the mesh begin parsed.
		 *
		 * Only used for debugging purposes.
		 */
		std::string mesh_name;
		/**
		 * @brief Directory of the .obj file.
		 *
		 * This directory should also contain the
		 * corresponding .mtl and texture files.
		 */
		std::string directory;
		/// Name of the .obj file to read.
		std::string filename;
		/// Contents of the file, line by line.
		std::vector<std::string> file_lines;

		// mesh data
		/// Vertices of the mesh.
		std::vector<glm::vec3> vertices;
		/// Normals per vertex.
		std::vector<glm::vec3> normals;
		/// Vertex indices.
		std::vector<int> triangles;
		/// Normal indices.
		std::vector<int> normal_idxs;
		/// Material ids per face.
		std::vector<std::string> mat_ids;

		// materials and textures
		/// Set of materials found in the file.
		std::vector<material> materials;
		/// Texture coordinates.
		std::vector<glm::vec2> textures_coords;
		/// Texture indices per triangle.
		std::vector<int> texture_coord_idxs;

		/// Frees the memory occupied by the loader.
		void clean();

		#if defined (DEBUG)
		void display_object_info();
		#endif

		/// Reads every line of the file and dumps them into @ref file_lines.
		bool load_file();
		/// Loads the material @e mtl_file in @ref directory.
		bool load_material(const char *mtl_file);

		/**
		 * @brief Retrieves the vertex coordinates, the faces and
		 * their normals from the file.
		 *
		 * The file's lines are stored in @ref file_lines.
		 *
		 * The data parsed goes from line @e A to line @e B.
		 * @param A Start at line @e A.
		 * @param B Start at line @e B.
		 * @pre 0 <= @e A < @e B < @ref file_lines.size()
		 */
		void parse_file_lines(size_t A, size_t B);

	public:
		/// Default constructor.
		OBJ_reader();
		/// Destructor.
		~OBJ_reader();

		/**
		 * @brief Loads one object stored in a .obj file.
		 * @param dir The directory where the file is.
		 * @param name The name of the .obj file.
		 * @param[out] M The mesh constructed using the data in the file.
		 * @return Returns true on success.
		 */
		bool load_object
		(const std::string& dir, const std::string& name, rendered_triangle_mesh& M);

};

