#pragma once

// render includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader.hpp>

namespace shader_helper {

	void set_materials_shader(const rendered_triangle_mesh& M, shader& S);

	void activate_textures(const rendered_triangle_mesh& M, shader& S);

} // -- namespace shader_helper
