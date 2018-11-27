#include <render/shader/shader_helper.hpp>

// C++ includes
#include <iostream>
#include <string>
#include <set>
using namespace std;

// render includes
#include <render/triangle_mesh/triangle_mesh_utils.hpp>

namespace shader_helper {

	static inline
	glm::vec3 to_vec3(const glm::vec4& v) {
		return glm::vec3(v[0], v[1], v[2]);
	}

	static inline
	void set_mat_shader(const material& mat, const string& name, shader& S) {
		S.set_vec3(name + ".ambient", to_vec3(mat.Ka));
		S.set_vec3(name + ".diffuse", to_vec3(mat.Kd));
		S.set_vec3(name + ".specular", to_vec3(mat.Ks));
		S.set_float(name + ".shininess", mat.Ns);
	}

	void set_materials_shader(const rendered_triangle_mesh& M, shader& S) {
		const set<int>& unique_mat_idxs = M.get_unique_material_idxs();
		size_t inf = unique_mat_idxs.size() + 1;

		if (unique_mat_idxs.size() > 5) {
			cerr << "shader_helper::set_materials_shaders - Error:" << endl;
			cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
			return;
		}

		size_t idxs[4] = {inf, inf, inf, inf};
		auto it = unique_mat_idxs.begin();
		if (*it == -1) {
			++it;
		}

		for (int i = 0; i < 4 and it != unique_mat_idxs.end(); ++i, ++it) {
			idxs[i] = *it;
		}

		const vector<material>& all_mats = M.get_materials();

		for (int i = 0; i < 4; ++i) {
			if (idxs[i] != inf) {
				const material& mat = all_mats[idxs[i]];
				set_mat_shader(mat, "material[" + std::to_string(i) + "]", S);
			}
		}
	}

	void activate_textures(const rendered_triangle_mesh& M, shader& S) {
		const set<int>& unique_mat_idxs = M.get_unique_material_idxs();
		size_t inf = unique_mat_idxs.size() + 1;

		if (unique_mat_idxs.size() > 5) {
			cerr << "shader_helper::activate_textures - Error:" << endl;
			cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
			return;
		}

		size_t idxs[4] = {inf, inf, inf, inf};
		auto it = unique_mat_idxs.begin();
		if (*it == -1) {
			++it;
		}

		for (int i = 0; i < 4 and it != unique_mat_idxs.end(); ++i, ++it) {
			idxs[i] = *it;
		}

		const vector<material>& all_mats = M.get_materials();

		for (int i = 0; i < 4; ++i) {
			if (idxs[i] != inf) {
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
	}

	void activate_materials_textures(const rendered_triangle_mesh& M, shader& S) {
		const set<int>& unique_mat_idxs = M.get_unique_material_idxs();
		size_t inf = unique_mat_idxs.size() + 1;

		if (unique_mat_idxs.size() > 5) {
			cerr << "shader_helper::activate_textures - Error:" << endl;
			cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
			return;
		}

		size_t idxs[4] = {inf, inf, inf, inf};
		auto it = unique_mat_idxs.begin();
		if (*it == -1) {
			++it;
		}

		for (int i = 0; i < 4 and it != unique_mat_idxs.end(); ++i, ++it) {
			idxs[i] = *it;
		}

		const vector<material>& all_mats = M.get_materials();

		for (int i = 0; i < 4; ++i) {
			if (idxs[i] != inf) {
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
	}

} // -- namespace shader_helper
