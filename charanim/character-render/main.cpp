#include <assert.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#include <cal3d/cal3d.h>
#include <cal3d/coremodel.h>
#include <cal3d/coresubmesh.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

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
	const string& top_dir, const string& sub_dir, const string& file,
	float& render_scale,
	string& data_path,
	string& skeleton,
	vector<string>& animations,
	vector<string>& meshes,
	vector<string>& materials
)
{
	string cfg_filename = top_dir + "/" + sub_dir + "/" + file;

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

		cout << "option: '" << option << "'" << endl;
		cout << "    value: '" << value << "'" << endl;

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
	const string& top_dir,
	const string& data_path,
	const string& skeleton,
	const vector<string>& animations,
	const vector<string>& meshes,
	const vector<string>& materials,
	CalCoreModel *core_model
)
{
	#if defined(DEBUG)
	cout << "Loading skeleton..." << endl;
	#endif

	bool success =
	core_model->loadCoreSkeleton(top_dir + "/" + data_path + "/" + skeleton);
	assert(success);

	#if defined(DEBUG)
	cout << "Loading animations..." << endl;
	#endif

	int *animation_id =
		static_cast<int *>(malloc(animations.size()*sizeof(int)));

	for (size_t i = 0; i < animations.size(); ++i) {
		const string& anim = animations[i];
		string anim_filename = top_dir + "/" + data_path + "/" + anim;

		#if defined(DEBUG)
		cout << "    Loading animation: " << anim_filename << endl;
		#endif

		animation_id[i] = core_model->loadCoreAnimation(anim_filename);

		if (animation_id[i] == -1) {
			CalError::printLastError();
			return false;
		}
	}

	#if defined(DEBUG)
	cout << "Loading meshes..." << endl;
	#endif

	for (size_t i = 0; i < meshes.size(); ++i) {
		const string& mesh = meshes[i];
		string mesh_filename = top_dir + "/" + data_path + "/" + mesh;
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
	cout << "Loading materials..." << endl;
	#endif

	for (size_t i = 0; i < meshes.size(); ++i) {
		const string& mat = materials[i];
		string mat_filename = top_dir + "/" + data_path + "/" + mat;
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

bool load_model(
	const string& dir,
	CalCoreModel *core_model,
	CalModel **model
)
{
	cout << "Loading textures..." << endl;

	// load all textures and store the opengl texture id in the
	// corresponding map in the material
	int mat_id;
	for (mat_id = 0; mat_id < core_model->getCoreMaterialCount(); ++mat_id) {
		// get the core material
		CalCoreMaterial *pCoreMaterial;
		pCoreMaterial = core_model->getCoreMaterial(mat_id);

		// loop through all maps of the core material
		for (int map_id = 0; map_id < pCoreMaterial->getMapCount(); ++map_id) {
			// get the filename of the texture
			string map_filename = pCoreMaterial->getMapFilename(map_id);
			string map_full_filename = dir + "/" + map_filename;

			cout << "    material filename: " << map_full_filename << endl;

			// load the texture from the file
			//GLuint tex_id = load_texture(strPath + strFilename);

			// store the opengl texture id in the user data of the map
			//pCoreMaterial->setMapUserData(map_id, (Cal::UserData)tex_id);
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

	core_model->getCoreSkeleton()->calculateBoundingBoxes(core_model);
	*model = new CalModel(core_model);

	// attach all meshes to the model
	int mesh_id;
	for (mesh_id = 0; mesh_id < core_model->getCoreMeshCount(); ++mesh_id) {
		(*model)->attachMesh(mesh_id);
	}

	// set the material set of the whole model
	(*model)->setMaterialSet(0);

	/*
	// set initial animation state
	m_state = STATE_MOTION;
	(*model)->getMixer()->blendCycle(m_animationId[STATE_MOTION], m_motionBlend[0], 0.0f);
	(*model)->getMixer()->blendCycle(m_animationId[STATE_MOTION + 1], m_motionBlend[1], 0.0f);
	(*model)->getMixer()->blendCycle(m_animationId[STATE_MOTION + 2], m_motionBlend[2], 0.0f);
	*/

	return true;
}

bool load_core_model(
	const string& top_dir,
	const string& sub_dir,
	const string& file,
	const string& name,
	CalCoreModel **core_model,
	CalModel **model
)
{
	// data from the configuration file
	float render_scale;
	string skeleton;
	string data_path;
	vector<string> animations, materials, meshes;

	string cfg_filename = top_dir + "/" + sub_dir + "/" + file;

	bool parsed = parse_cfg_file(
		top_dir, sub_dir, file,
		render_scale, data_path, skeleton, animations, meshes, materials
	);

	if (not parsed) {
		cerr << "Error when parsing file " << cfg_filename << endl;
		return false;
	}

	*core_model = new CalCoreModel(name);

	#if defined(DEBUG)
	cout << "In configuration file: " << cfg_filename << endl;
	cout << "    Found skeleton in " << skeleton << " animations" << endl;
	cout << "    Found: " << animations.size() << " animations" << endl;
	cout << "    Found: " << meshes.size() << " meshes" << endl;
	cout << "    Found: " << materials.size() << " materials" << endl;
	#endif

	bool lamm = load_anims_meshes_materials(
		top_dir, data_path, skeleton,
		animations, meshes, materials,
		*core_model
	);
	if (not lamm) {
		return false;
	}

	bool ltm = load_model(
		top_dir + "/" + sub_dir,
		*core_model, model
	);
	if (not ltm) {
		return false;
	}

	return true;
}

int main() {
	CalCoreModel *cal_core_model = nullptr;
	CalModel *cal_model = nullptr;

	bool res = load_core_model(
		"../cally", "data", "paladin.cfg",
		"dummy",
		&cal_core_model, &cal_model
	);

	if (not res) {
		cerr << "Error: when loading model 'paladin'" << endl;
		return 1;
	}
	if (cal_core_model == nullptr) {
		cerr << "Error: CalCoreModel not created" << endl;
		return 1;
	}
	if (cal_model == nullptr) {
		cerr << "Error: CalModel not created" << endl;
		return 1;
	}

	cout << "Is cal_core_model pointer null? " << (cal_core_model == nullptr ? "Yes" : "No") << endl;
	cout << "Is cal_model pointer null? " << (cal_model == nullptr ? "Yes" : "No") << endl;

	cout << "Information about the cal_core_model:" << endl;
	cout << "    It has: " << cal_core_model->getCoreMeshCount() << " meshes" << endl;
	for (int i = 0; i < cal_core_model->getCoreMeshCount(); ++i) {
		CalCoreMesh *mesh = cal_core_model->getCoreMesh(i);
		cout << "    mesh " << i << " has " << mesh->getCoreSubmeshCount() << " submeshes" << endl;
		for (int j = 0; j < mesh->getCoreSubmeshCount(); ++j) {
			CalCoreSubmesh *submesh = mesh->getCoreSubmesh(j);
			cout << "        submesh " << j << " has:" << endl;
			cout << "            " << submesh->getVertexCount() << " vertices:" << endl;
			cout << "            With coordinates: ";
			const vector<CalCoreSubmesh::Vertex>& verts = submesh->getVectorVertex();
			for (const CalCoreSubmesh::Vertex& v : verts) {
				const CalVector& pos = v.position;
				cout << "(" << pos[0] << "," << pos[1] << "," << pos[2] << "),";
			}
			cout << endl;

			cout << "            " << submesh->getVectorFace().size() << " triangles:" << endl;
			cout << "            With vertex indices: ";
			const vector<CalCoreSubmesh::Face>& faces = submesh->getVectorFace();
			for (const CalCoreSubmesh::Face& f : faces) {
				cout << "(" << f.vertexId[0] << "," << f.vertexId[1] << "," << f.vertexId[2] << "),";
			}
			cout << endl;
		}
	}

	delete cal_model;
	delete cal_core_model;
}
