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

// C++ includes
#include <string>

// glm includes
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define __NULL_MATERIAL_NAME "material-without-material"
#define __NULL_MATERIAL_INDEX -1
#define __NULL_TEXTURE_NAME "material-without-texture"
#define __NULL_TEXTURE_INDEX 0
#define __MAX_TEXTURES_LOADED 4

/**
 * @brief The material struct.
 */
struct material {
	/// Identifier of this material.
	std::string id;
	/// Filename of the texture material.
	std::string txt_name;
	/// Texture (OpenGL) id used on this material.
	uint txt_id;
	/// Illumination mesh.
	int illum;
	/// Specular exponent (shininess).
	float Ns;
	/// ¿?
	float Ni;
	/// Transparency.
	float d;
	/// Ambient colour, with transparency.
	glm::vec4 Ka;
	/// Diffuse colour, with transparency.
	glm::vec4 Kd;
	/// Specular colour, with transparency.
	glm::vec4 Ks;

	/// Default constructor
	material();
	/**
	 * @brief Constructor.
	 * @param amb Ambient colour, without transparency.
	 * @param dif Diffuse colour, without transparency.
	 * @param spec Specular colour, without transparency.
	 * @param ns Specular exponent.
	 * @param ni ¿?
	 * @param D Transparency
	 * @param ill Illumination mesh.
	 * @param textID Texture identifier.
	 * @param id Material's identifier.
	 */
	material
	(
		const std::string& _id,
		const std::string& _txt_name,
		uint _txt_id,
		const glm::vec3& amb,
		const glm::vec3& dif,
		const glm::vec3& spec,
		float ns, float ni,float D,
		int ill
	);

	/// Copy constructor.
	material(const material& m);
};

/**
 * @brief Types of states in which the mesh can be in.
 *
 * These are used to inform the user of the type of errors
 * that are in the structure of the mesh.
 */
enum mesh_state {
	/// No errors in the mesh.
	correct					= 0x000,
	/// The mesh has no vertices.
	no_vertices				= 0x001,
	/// The mesh has no faces.
	no_triangles			= 0x002,
	/// The mesh has no normals.
	no_normals				= 0x004,
	/// The mesh has no normal indexes.
	no_normal_idxs			= 0x008,
	/**
	 * @brief There is a triangle that has a vertex
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than
	 * the amount of vertices.
	 */
	vertex_idx_ob			= 0x010,
	/**
	 * @brief There is a triangle that has a normal
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than the
	 * amount of normal vectors.
	 */
	normal_idx_ob			= 0x020,
	/**
	 * @brief There is a triangle that has a texture coordinate
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than the amount
	 * of texture coordinates.
	 */
	texture_coord_idx_ob	= 0x040
};

