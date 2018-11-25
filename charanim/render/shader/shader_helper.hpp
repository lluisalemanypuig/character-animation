#pragma once

// render includes
#include <render/mesh/rendered_tri_mesh.hpp>
#include <render/shader/shader.hpp>

namespace shader_helper {

	void set_materials_shader(const rendered_tri_mesh& M, shader& S);

	void activate_textures(const rendered_tri_mesh& M, shader& S);

} // -- namespace shader_helper
