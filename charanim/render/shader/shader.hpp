#pragma once

// C++ includes
#include <string>

// render includes
#include <render/include_gl.hpp>

// glm includes
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

class shader {
	private:
		/// The program ID.
		unsigned int ID;

	public:
		/// Default constructor.
		shader();
		/// Destructor constructor.
		~shader();

		// MODIFIERS

		/**
		 * @brief Initialise a shader program.
		 *
		 * Loads a vertex and shader GLSL programs found in @e vertex and
		 * @e fragment files.
		 * @param dir Directory where the shaders are found.
		 * @param vertex Filename of the vertex shader program.
		 * @param fragment Filename of the fragment shader program.
		 * @returns Returns true on success. Returns false if otherwise.
		 */
		bool init(const std::string& dir, const std::string& vertex, const std::string& fragment);

		/// Frees the shader program's memory.
		void clear();

		// SETTERS

		/// Sets a Boolean value to the specified uniform.
		void set_bool(const std::string& name, bool value) const;
		/// Sets an integer value to the specified uniform.
		void set_int(const std::string& name, int value) const;
		/// Sets a floating-point value to the specified uniform.
		void set_float(const std::string& name, float value) const;
		/// Sets a vec2 value to the specified uniform.
		void set_vec2(const std::string& name, const glm::vec2& v) const;
		/// Sets a vec3 value to the specified uniform.
		void set_vec3(const std::string& name, const glm::vec3& v) const;
		/// Sets a vec4 value to the specified uniform.
		void set_vec4(const std::string& name, const glm::vec4& v) const;
		/// Sets a mat3 value to the specified uniform.
		void set_mat3(const std::string& name, const glm::mat3& m, bool transpose = false) const;
		/// Sets a mat4 value to the specified uniform.
		void set_mat4(const std::string& name, const glm::mat4& m, bool transpose = false) const;

		// GETTERS

		GLuint get_id() const;

		// OTHERS

		/**
		 * @brief Use/activate the shader.
		 *
		 * Calls glUseProgram(@ref ID)
		 */
		void bind() const;

		/**
		 * @brief Deactivate the shader.
		 *
		 * Calls glUseProgram(0)
		 */
		void release() const;
};
