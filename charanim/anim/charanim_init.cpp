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

#include <anim/charanim.hpp>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
using namespace physim::particles;

// charanim includes
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>
#include <render/shader/shader_helper.hpp>
#include <anim/vec_helper.hpp>
#include <render/character/character_reader.hpp>

namespace charanim {

	bool load_shaders() {
		bool r;

		r = flat_shader.init("../../charanim/shaders", "flat.vert", "flat.frag");
		if (not r) { return false; }

		r = material_shader.init("../../charanim/shaders", "materials.vert", "materials.frag");
		if (not r) { return false; }

		r = texture_shader.init("../../charanim/shaders", "textures.vert", "textures.frag");
		if (not r) { return false; }

		flat_shader.bind();
		flat_shader.set_vec3("view_pos", glm::vec3(0.0f,0.0f,0.0f));
		flat_shader.set_bool("wireframe", false);
		flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));
		flat_shader.release();

		texture_shader.bind();
		texture_shader.set_vec3("view_pos", glm::vec3(0.0f,0.0f,0.0f));
		texture_shader.release();

		return true;
	}

	bool load_sphere() {
		sphere = new rendered_triangle_mesh();
		OBJ_reader obj;
		bool r = obj.load_object("../../charanim/models", "sphere-wireframe.obj", *sphere);
		if (not r) {
			return false;
		}
		sphere->make_buffers();
		return true;
	}

	bool load_characters(const vector<string>& dirs, const vector<string>& names)
	{
		assert(dirs.size() == names.size());

		// data dir, core model
		map<string, pair<string, shared_ptr<CalCoreModel> > > relation;

		for (size_t i = 0; i < dirs.size(); ++i) {
			// load models for as many agent particles there are
			characters.push_back(rendered_character());

			bool res;
			shared_ptr<CalCoreModel> core_model = nullptr;
			string data_dir;

			auto it = relation.find(names[i]);
			if (it == relation.end()) {
				res = character_reader::load_core_model(
					dirs[i], names[i], "dummy", core_model, data_dir
				);
				if (not res) {
					cerr << "Error: when loading core model" << endl;
					return false;
				}
			}
			else {
				data_dir = it->second.first;
				core_model = it->second.second;
			}

			shared_ptr<CalModel> model = nullptr;
			res = character_reader::make_model(
				data_dir, core_model, model
			);
			if (not res) {
				cerr << "Error: when making model" << endl;
				return false;
			}

			characters.back().set_cal_info(core_model, model);
			characters.back().initialise_buffers();
		}
		return true;
	}

} // -- namespace charanim
