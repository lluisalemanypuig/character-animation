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
#include <memory>
#include <vector>

// Cal3d includes
#include <cal3d/cal3d.h>
#include <cal3d/coremodel.h>
#include <cal3d/coresubmesh.h>

// render includes
#include <render/triangle_mesh/triangle_mesh_utils.hpp>

class rendered_character {
	private:
		std::shared_ptr<CalCoreModel> core_model;
		std::shared_ptr<CalModel> model;

		// bounding box
		glm::vec3 vmin, vmax;

		std::vector<material> all_mats;

		// data for buffers
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> tex_coords;
		std::vector<uint> mat_idxs;
		std::vector<uint> tex_idxs;
		std::vector<uint> corners;

		std::vector<int> faces_per_submesh;

		/// Vertex Array Object index
		uint VAO;
		/// Vertex Buffer Objects
		uint VBO_verts, VBO_normals, VBO_tex_coords;
		/// Indices Buffer Object index.
		uint IBO_mats, IBO_texs;
		/// Element Buffer Object (indices).
		uint EBO;

		void clear_buffer(uint& buf_id);

	public:
		rendered_character();
		~rendered_character();

		void clear_buffers();

		void set_cal_info
		(std::shared_ptr<CalCoreModel> core_model,
		 std::shared_ptr<CalModel> model);

		// model->getMixer()->blendCycle(id, w, s);
		void set_anim_weight(int id, float w, float s);
		void clear_anim(int id);

		// needs set_cal_info
		void initialise_buffers();
		// needs set_cal_info
		bool flatten_data();
		// needs buffers initialised, and data flattened
		void fill_buffers();

		void render() const;
		void draw() const;

		// GETTERS

		void get_bounding_box(glm::vec3& vmin, glm::vec3& vmax) const;
		glm::vec3 get_center() const;

		const std::vector<material>& get_materials() const;

		std::shared_ptr<CalCoreModel> get_core_model();
		const std::shared_ptr<CalCoreModel> get_core_model() const;

		std::shared_ptr<CalModel> get_model();
		const std::shared_ptr<CalModel> get_model() const;
};
