#pragma once

// C++ includes
#include <vector>
#include <string>
#include <set>

// glm includes
#include <glm/vec2.hpp>

// render includes
#include <render/model/model_utils.hpp>
#include <render/model/model.hpp>

/**
 * @brief Rendered model class.
 *
 * This is an abstraction of a model that has to be rendered somehow.
 *
 * This means having to draw its geometry with flat colours, lighting,
 * textures, ...
 *
 * Rendering this model requires calling the appropriate methods
 * within a valid OpenGL context. For example, if one wants to render
 * this model with textures method @ref load_textures must be loaded
 * within an OpenGL context. Same for faster rendering using OpenGL lists
 * (see @ref copmile).
 */
class rendered_model : public model {
	private:
		/**
		 * @brief Materials used in the mesh.
		 *
		 * Material per triangle, not per vertex.
		 */
		std::vector<material> materials;

		/// Material idx per face.
		std::vector<int> mat_idxs;
		std::set<int> unique_mat_idxs;

		/// Texture coordinates.
		std::vector<glm::vec2> texture_coords;
		/// Texture coordinate indices per vertex.
		std::vector<int> texture_coord_idxs;
		/// OpenGL indexes of the textures.
		std::vector<uint> texture_openGL_idxs;

		/// Index of the compiled list.
		uint list_index;

		/// Vertex Array Object index
		uint VAO;
		/// Vertex Buffer Object index.
		uint VBO;
		/**
		 * @brief Indices Buffer Object index.
		 *
		 * May contain only material indices or both
		 * material and texture indices.
		 */
		uint IBO;
		/// Element Buffer Object (indices).
		uint EBO;

	public:
		/// Constructor.
		rendered_model();
		/**
		 * @brief Copy constructor
		 *
		 * The index for the glList is not copied. Therefore, it
		 * should be compiled again.
		 */
		rendered_model(const rendered_model& m);
		/// Destructor.
		~rendered_model();

		// SETTERS

		/// Sets the materials used in this mesh.
		void set_materials
		(const std::vector<material>& mats, const std::vector<std::string>& mat_idxs);
		/// Sets the texture coordinates.
		void set_texture_coords(const std::vector<glm::vec2>& texts);
		/// Sets the texture coordinates indices.
		void set_texture_coord_idxs(const std::vector<int>& text_coord_idxs);

		// GETTERS

		mesh_state state(const mesh_state& ignore = mesh_state::correct) const;

		const std::vector<int>& get_material_idxs() const;
		const std::set<int>& get_unique_material_idxs() const;

		const std::vector<material>& get_materials() const;

		// MODIFIERS

		/**
		 * @brief Loads the textures for the model.
		 *
		 * In case this model has textures, they should be loaded here.
		 */
		void load_textures();

		/**
		 * @brief Clears the memory occupied by the model.
		 *
		 * This means clearing not only the memory for the vertices,
		 * normals, ..., but also the compiled models.
		 */
		void clear();

		/**
		 * @brief Returns whether glLists are used for rendering.
		 * @return Returns @ref list_index > 0.
		 */
		bool uses_lists() const;

		/**
		 * @brief Returns whether buffers are used for rendering.
		 *
		 * In this case, a shader program wil be used.
		 * @return Returns @ref VBO > 0 and @ref EBO > 0.
		 */
		bool uses_buffers() const;

		// OTHERS

		/**
		 * @brief Outputs information about the mesh.
		 *
		 * Outputs the following information into standard
		 * output:
		 * - Number of vertices.
		 * - Number of faces.
		 * - Number of normals.
		 * - Number of materials.
		 * - Number of texture coordinates.
		 * - Number of textures used to texturise this mesh.
		 * - The definition of the materials used.
		 */
		void display_mesh_info() const;

		/**
		 * @brief Renders the mesh.
		 *
		 * Renders the faces one by one calling
		 \verbatim
		 glBegin();
		 ...
		 glEnd();
		 \endverbatim
		 * This method assumes that the result
		 * of method @ref is_valid is 'true'.
		 */
		void slow_render() const;

		/**
		 * @brief Compiles this object into a list.
		 *
		 * Uses glNewList, glEndList to compile the object.
		 * Between these two calls, the function @ref slow_render()
		 * is called.
		 * @return Returns the list index.
		 */
		uint compile();

		/**
		 * @brief Builds buffer objects for fast rendering.
		 *
		 * Builds the necessary buffer objects to render the model
		 * more efficiently than with GL lists.
		 */
		void make_buffers();

		/**
		 * @brief Builds buffer objects for fast rendering.
		 *
		 * Builds the necessary buffer objects to render the model
		 * more efficiently than with GL lists. This function adds
		 * materials to the buffers.
		 */
		void make_buffers_materials();

		/**
		 * @brief Builds buffer objects for fast rendering.
		 *
		 * Builds the necessary buffer objects to render the model
		 * more efficiently than with GL lists. This function adds
		 * materials to the buffers.
		 */
		void make_buffers_materials_textures();

		/**
		 * @brief Chooses the best way of rendering this model.
		 *
		 * If no buffer is available, then uses a glList.
		 * If no list is available (@ref list_index is -1) then
		 * @ref slow_render() is used.
		 */
		void render() const;
};
