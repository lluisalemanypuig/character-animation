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
		std::shared_ptr<CalCoreModel> core_model,
		int **animation_ids
	);

	bool make_model(
		const std::string& dir,
		int *anim_ids,
		std::shared_ptr<CalCoreModel>& core_model,
		std::shared_ptr<CalModel>& model
	);

	bool load_core_model(
		const std::string& dir, const std::string& file,
		const std::string& name,
		std::shared_ptr<CalCoreModel>& core_model,
		std::shared_ptr<CalModel>& model
	);

} // -- namespace character_reader
