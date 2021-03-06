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

#include <render/shader/shader_helper.hpp>

// C++ includes
#include <numeric>
#include <iostream>
#include <string>
#include <set>
using namespace std;

// render includes
#include <render/triangle_mesh/triangle_mesh_utils.hpp>

namespace shader_helper {

static inline
void set_mat_shader
(const material& mat, const string& name, shader& S)
{
	S.set_vec4(name + ".ambient", mat.Ka);
	S.set_vec4(name + ".diffuse", mat.Kd);
	S.set_vec4(name + ".specular", mat.Ks);
	S.set_float(name + ".shininess", mat.Ns);
}

void set_materials_shader
(const rendered_triangle_mesh& M, shader& S, size_t max_mat)
{
	const set<int>& unique_mat_idxs = M.get_unique_material_idxs();
	int inf = static_cast<int>(unique_mat_idxs.size()) + 1;

	if (unique_mat_idxs.size() > max_mat) {
		cerr << "shader_helper::set_materials_shaders - Error:" << endl;
		cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
		return;
	}

	vector<int> idxs(max_mat, inf);
	auto it = unique_mat_idxs.begin();
	if (*it == -1) {
		++it;
	}

	for (size_t i = 0; i < max_mat and it != unique_mat_idxs.end(); ++i, ++it) {
		idxs[i] = *it;
	}

	const vector<material>& all_mats = M.get_materials();

	for (size_t i = 0; i < idxs.size(); ++i) {
		if (idxs[i] == inf) {
			continue;
		}
		const material& mat = all_mats[idxs[i]];
		set_mat_shader(mat, "material[" + std::to_string(i) + "]", S);
	}
}

void activate_textures
(const rendered_triangle_mesh& M, shader& S, size_t max_mat)
{
	const set<int>& unique_mat_idxs = M.get_unique_material_idxs();
	int inf = static_cast<int>(unique_mat_idxs.size()) + 1;

	if (unique_mat_idxs.size() > max_mat) {
		cerr << "shader_helper::activate_textures - Error:" << endl;
		cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
		return;
	}

	vector<int> idxs(max_mat, inf);
	auto it = unique_mat_idxs.begin();
	if (*it == -1) {
		++it;
	}

	for (size_t i = 0; i < max_mat and it != unique_mat_idxs.end(); ++i, ++it) {
		idxs[i] = *it;
	}

	const vector<material>& all_mats = M.get_materials();

	for (size_t i = 0; i < idxs.size(); ++i) {
		if (idxs[i] == inf) {
			continue;
		}
		const material& mat = all_mats[idxs[i]];
		if (mat.txt_id > __NULL_TEXTURE_INDEX) {
			// these two lines are important
			glActiveTexture(GL_TEXTURE0 + mat.txt_id);
			glBindTexture(GL_TEXTURE_2D, mat.txt_id);

			string texname = "tex" + std::to_string(mat.txt_id);
			S.set_int(texname, mat.txt_id);
		}
	}
}

void activate_materials_textures
(const rendered_triangle_mesh& M, shader& S, size_t max_mat)
{
	const set<int>& unique_mat_idxs = M.get_unique_material_idxs();
	int inf = static_cast<int>(unique_mat_idxs.size()) + 1;

	if (unique_mat_idxs.size() > max_mat) {
		cerr << "shader_helper::activate_textures - Error:" << endl;
		cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
		return;
	}

	vector<int> idxs(max_mat, inf);
	auto it = unique_mat_idxs.begin();
	if (*it == -1) {
		++it;
	}

	for (size_t i = 0; i < max_mat and it != unique_mat_idxs.end(); ++i, ++it) {
		idxs[i] = *it;
	}

	const vector<material>& all_mats = M.get_materials();

	for (size_t i = 0; i < idxs.size(); ++i) {
		if (idxs[i] == inf) {
			continue;
		}
		const material& mat = all_mats[idxs[i]];
		set_mat_shader(mat, "material[" + std::to_string(i) + "]", S);
		if (mat.txt_id > __NULL_TEXTURE_INDEX) {
			// these two lines are important
			glActiveTexture(GL_TEXTURE0 + mat.txt_id);
			glBindTexture(GL_TEXTURE_2D, mat.txt_id);

			string texname = "tex" + std::to_string(mat.txt_id);
			S.set_int(texname, mat.txt_id);
		}
	}
}

#define out_vec4(v) v.x << "," << v.y << "," << v.z << "," << v.w

void activate_materials_textures
(const rendered_character& M, shader& S, size_t max_mat)
{
	const vector<material>& all_mats = M.get_materials();

	if (all_mats.size() > max_mat) {
		cerr << "shader_helper::activate_textures - Error:" << endl;
		cerr << "    Too many materials (" << all_mats.size() << ")." << endl;
		return;
	}

	for (size_t i = 0; i < all_mats.size(); ++i) {
		const material& mat = all_mats[i];
		set_mat_shader(mat, "material[" + std::to_string(i) + "]", S);

		if (mat.txt_id > __NULL_TEXTURE_INDEX) {
			// these two lines are important
			glActiveTexture(GL_TEXTURE0 + mat.txt_id);
			glBindTexture(GL_TEXTURE_2D, mat.txt_id);

			string texname = "tex" + std::to_string(mat.txt_id);
			S.set_int(texname, mat.txt_id);
		}
	}
}

} // -- namespace shader_helper
