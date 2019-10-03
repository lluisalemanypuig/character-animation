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

// render includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/character/rendered_character.hpp>
#include <render/shader/shader.hpp>

namespace shader_helper {

	void set_materials_shader
	(const rendered_triangle_mesh& M, shader& S);

	void activate_textures
	(const rendered_triangle_mesh& M, shader& S, size_t max_material = 32);

	void activate_materials_textures
	(const rendered_triangle_mesh& M, shader& S, size_t max_material = 32);

	void activate_materials_textures
	(const rendered_character& M, shader& S, size_t max_material = 32);

} // -- namespace shader_helper
