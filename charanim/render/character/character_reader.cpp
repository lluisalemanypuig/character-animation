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

#include <render/character/character_reader.hpp>

// C++ includes
#include <iostream>
using namespace std;

// render includes
#include <render/include_gl.hpp>
#include <render/textures/texture_loader.hpp>

namespace character_reader {

void parse_line(const string& line, string& option, string& value) {
	size_t eq = line.find('=');
	option = line.substr(0, eq);
	value = line.substr(eq + 1, line.length() - eq - 1);
}

// Parses the configuration file.
// Constructs the core model with the animations,
// materials and meshes
bool parse_cfg_file
(
	const string& dir, const string& file,
	float& render_scale,
	string& data_path,
	string& skeleton,
	vector<string>& animations,
	vector<string>& meshes,
	vector<string>& materials
)
{
	string cfg_filename = dir + "/" + file;

	// open the model configuration file
	ifstream fin;
	fin.open(cfg_filename.c_str());
	if (not fin.is_open()) {
		cerr << "Failed to open model configuration file '"
			 << cfg_filename << "'." << endl;
		return false;
	}

	string line;
	while (getline(fin, line)) {
		// ignore commented lines
		if (line[0] == '#') {
			continue;
		}
		// ignore empty lines
		if (line == "") {
			continue;
		}

		string option, value;
		parse_line(line, option, value);

		#if defined(DEBUG)
		cout << "option: '" << option << "' -- "
			 << "value: '" << value << "'" << endl;
		#endif

		if (option == "path") {
			data_path = value;
			if (data_path[data_path.length() - 1] == '/') {
				data_path = data_path.substr(0, data_path.length() - 1);
			}
		}
		else if (option == "skeleton") {
			skeleton = value;
		}
		else if (option == "scale") {
			render_scale = atof(value.c_str());
		}
		else if (option == "animation") {
			animations.push_back(value);
		}
		else if (option == "mesh") {
			meshes.push_back(value);
		}
		else if (option == "material") {
			materials.push_back(value);
		}
	}

	fin.close();
	return true;
}

bool load_anims_meshes_materials
(
	const string& dir,
	const string& skeleton,
	const vector<string>& animations,
	const vector<string>& meshes,
	const vector<string>& materials,
	std::shared_ptr<CalCoreModel> core_model
)
{
	#if defined(DEBUG)
	cout << "Loading skeleton..." << endl;
	#endif

	bool success = core_model->loadCoreSkeleton(dir + "/" + skeleton);
	assert(success);

	#if defined(DEBUG)
	cout << "Loading meshes..." << endl;
	#endif

	for (size_t i = 0; i < meshes.size(); ++i) {
		const string& mesh = meshes[i];
		string mesh_filename = dir + "/" + mesh;
		#if defined(DEBUG)
		cout << "    Loading mesh: " << mesh_filename << endl;
		#endif

		int r = core_model->loadCoreMesh(mesh_filename);
		if (r == -1) {
			CalError::printLastError();
			return false;
		}
	}

	#if defined(DEBUG)
	cout << "Loading animations..." << endl;
	#endif

	for (size_t i = 0; i < animations.size(); ++i) {
		const string& anim = animations[i];
		string anim_filename = dir + "/" + anim;

		#if defined(DEBUG)
		cout << "    Loading animation: " << anim_filename << endl;
		#endif

		int id = core_model->loadCoreAnimation(anim_filename);

		CalCoreAnimation *animation = core_model->getCoreAnimation(id);
		cout << "Duration of " << id << ": " << animation->getDuration() << endl;

		if (id == -1) {
			CalError::printLastError();
			return false;
		}
	}

	#if defined(DEBUG)
	cout << "Loading materials..." << endl;
	#endif

	for (size_t i = 0; i < materials.size(); ++i) {
		const string& mat = materials[i];
		string mat_filename = dir + "/" + mat;
		#if defined(DEBUG)
		cout << "    Loading material: " << mat_filename << endl;
		#endif

		int r = core_model->loadCoreMaterial(mat_filename);
		if (r == -1) {
			CalError::printLastError();
			return false;
		}
	}

	return true;
}

bool make_model(
	const string& dir,
	std::shared_ptr<CalCoreModel>& core_model,
	std::shared_ptr<CalModel>& model
)
{
	#if defined (DEBUG)
	cout << "Loading textures (if any) ..." << endl;
	#endif

	texture_loader& tex_loader = texture_loader::get_loader();

	// load all textures and store the opengl texture id in the
	// corresponding map in the material
	int mat_id;
	for (mat_id = 0; mat_id < core_model->getCoreMaterialCount(); ++mat_id) {
		// get the core material
		CalCoreMaterial *core_material = core_model->getCoreMaterial(mat_id);

		// loop through all maps of the core material
		for (int map_id = 0; map_id < core_material->getMapCount(); ++map_id) {

			// get the filename of the texture
			string map_filename = core_material->getMapFilename(map_id);
			string map_full_filename = dir + "/" + map_filename;

			#if defined (DEBUG)
			cout << "    texture filename: " << map_full_filename << endl;
			#endif

			// load the texture from the file
			GLuint tex_id = tex_loader.load_texture(map_full_filename);

			// store the opengl texture id in the user data of the map
			core_material->setMapUserData(map_id, (Cal::UserData)tex_id);
		}
	}

	// make one material thread for each material
	// NOTE: this is not the right way to do it, but this viewer
	// can't do the right mapping without further information on
	// the model etc.
	for (mat_id = 0; mat_id < core_model->getCoreMaterialCount(); ++mat_id) {
		// create the a material thread
		core_model->createCoreMaterialThread(mat_id);

		// initialize the material thread
		core_model->setCoreMaterialId(mat_id, 0, mat_id);
	}

	// Calculate Bounding Boxes

	core_model->getCoreSkeleton()->calculateBoundingBoxes(core_model.get());
	model = shared_ptr<CalModel>(new CalModel(core_model.get()));

	// attach all meshes to the model
	int mesh_id;
	for (mesh_id = 0; mesh_id < core_model->getCoreMeshCount(); ++mesh_id) {
		model->attachMesh(mesh_id);
	}

	// set the material set of the whole model
	model->setMaterialSet(0);
	return true;
}

bool load_core_model(
	const string& dir,
	const string& file,
	const string& name,
	shared_ptr<CalCoreModel>& core_model,
	string& data_dir
)
{
	// data from the configuration file
	string skeleton;
	string data_path;
	vector<string> animations, materials, meshes;
	float render_scale;

	string cfg_filename = dir + "/" + file;

	// load configuration file
	bool parsed = parse_cfg_file(
		dir, file,
		render_scale, data_path, skeleton,
		animations, meshes, materials
	);

	if (not parsed) {
		cerr << "Error when parsing file " << cfg_filename << endl;
		return false;
	}

	core_model = shared_ptr<CalCoreModel>(new CalCoreModel(name));

	#if defined(DEBUG)
	cout << "In configuration file: " << cfg_filename << endl;
	cout << "    Found skeleton in " << skeleton << " animations" << endl;
	cout << "    Found: " << animations.size() << " animations" << endl;
	cout << "    Found: " << meshes.size() << " meshes" << endl;
	cout << "    Found: " << materials.size() << " materials" << endl;
	#endif

	data_dir = dir + "/" + data_path;
	bool lamm = load_anims_meshes_materials(
		data_dir, skeleton,
		animations, meshes, materials,
		core_model
	);
	if (not lamm) {
		return false;
	}

	return true;
}

} // -- namespace character_reader
