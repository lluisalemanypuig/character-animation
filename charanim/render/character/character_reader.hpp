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
#include <vector>
#include <string>
#include <memory>

// Cal3d includes
#include <cal3d/cal3d.h>
#include <cal3d/coremodel.h>
#include <cal3d/coresubmesh.h>

namespace character_reader {

	void parse_line(const std::string& line, std::string& option, std::string& value);
	bool parse_cfg_file
	(
		const std::string& dir, const std::string& file, float& render_scale,
		std::string& data_path, std::string& skeleton, std::vector<std::string>& animations,
		std::vector<std::string>& meshes, std::vector<std::string>& materials
	);

	bool load_anims_meshes_materials
	(
		const std::string& dir, const std::string& skeleton,
		const std::vector<std::string>& animations, const std::vector<std::string>& meshes,
		const std::vector<std::string>& materials,
		std::shared_ptr<CalCoreModel> core_model
	);

	bool make_model(
		const std::string& dir,
		std::shared_ptr<CalCoreModel>& core_model,
		std::shared_ptr<CalModel>& model
	);

	bool load_core_model(
		const std::string& dir, const std::string& file,
		const std::string& name,
		std::shared_ptr<CalCoreModel>& core_model,
		std::string& data_dir
	);

} // -- namespace character_reader
