#include <render/obj_reader.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

// glm includes
#include <glm/glm.hpp>

// PUBLIC

OBJ_reader::OBJ_reader() { }

void OBJ_reader::clean() {
	mesh_name = "";
	directory = filename = "";

	file_lines.clear();

	vertices.clear();
	normals.clear();
	triangles.clear();
	normal_idxs.clear();
	mat_ids.clear();
	materials.clear();
	textures_coords.clear();
}

OBJ_reader::~OBJ_reader() {
	clean();
}

bool OBJ_reader::load_file() {
	string full_name = directory + "/" + filename;

	ifstream obj;
	obj.open(full_name.c_str());
	if (not obj.is_open()) {
		cerr << "OBJ_reader::load_file: Error" << endl;
		cerr << "    An error occurred while trying to open the object file" << endl;
		cerr << "    '" << full_name << "'" << endl;
		return false;
	}
	// read all lines of the file and
	// dump them in 'file_lines'
	char buf[256];
	while (not obj.eof()) {
		obj.getline(buf, 256);
		buf[255] = '\0';
		file_lines.push_back(string(buf));
	}
	obj.close();
	return true;
}

bool OBJ_reader::load_material(const char *mtl_file) {
	string full_name = directory + "/" + string(mtl_file);

	ifstream mtl;
	mtl.open(full_name.c_str());
	if (not mtl.is_open()) {
		cerr << "OBJ_reader::load_material: Error" << endl;
		cerr << "    An error occurred while trying to open the material file" << endl;
		cerr << "    '" << full_name << "'" << endl;
		return false;
	}

	// read all lines of the file and
	// dump them in 'mat_lines'
	vector<string> mat_lines;
	char buf[256];
	while (not mtl.eof()) {
		mtl.getline(buf, 256);
		mat_lines.push_back(string(buf));
	}
	mtl.close();

	string mat_name, txt_name;
	bool valid_material = false;
	glm::vec3 amb, dif, spec;
	float Ns, Ni, d;
	int illum;
	unsigned int txt_id;

	Ns = Ni = d = 0.0f;
	illum = 0;
	txt_id = 0;

	for (size_t i = 0; i < mat_lines.size(); ++i) {
		// ignore comments
		if (mat_lines[i][0] == '#') {
			continue;
		}

		string tag;

		stringstream ss(mat_lines[i]);
		ss >> tag;

		if (tag == "") {
			continue;
		}

		// if line starts with 'new'
		if (tag == "newmtl") {
			if (valid_material) {
				// first, the contents of the variables do not
				// contain a valid material, so this if is
				// skipped in the first iteration it is found.
				materials.push_back(
					material(
						mat_name, txt_name, txt_id,
						amb, dif, spec, Ns, Ni, d, illum
					)
				);
			}
			valid_material = true;
			txt_name = __NULL_TEXTURE_NAME;
			txt_id = 0;
			ss >> mat_name;
			txt_id = 0;
		}
		else if (tag == "Ns") {
			ss >> Ns;
		}
		else if (tag == "Ka") {
			ss >> amb.x >> amb.y >> amb.z;
		}
		else if (tag == "Kd") {
			ss >> dif.x >> dif.y >> dif.z;
		}
		else if (tag == "Ks") {
			ss >> spec.x >> spec.y >> spec.z;
		}
		else if (tag == "Ni") {
			ss >> Ni;
		}
		else if (tag == "d") {
			ss >> d;
		}
		else if (tag == "illum") {
			ss >> illum;
		}
		else if (tag == "map_Kd") {
			ss >> txt_name;
			txt_name = directory + "/" + txt_name;
		}
		else {
			cerr << "    Tag '" << tag << "' not recognised" << endl;
		}
	}
	// load last batch of data read
	materials.push_back(
		material(mat_name, txt_name, txt_id, amb, dif, spec, Ns, Ni, d, illum)
	);

	return true;
}

void OBJ_reader::parse_file_lines(size_t A, size_t B) {
	assert(A < B and B < file_lines.size());

	// current material's name
	string current_material = __NULL_MATERIAL_NAME;
	char buf[256];
	// the current face should be smoothed
	bool smooth_face = false;
	// amount of smoothed faces
	size_t S = 0;

	// face information
	int NI[4], VN[4], TC[4];
	// vertex information
	float x, y, z;
	// texture coordinate
	float u, v;

	for (size_t i = A; i <= B; ++i) {
		if (file_lines[i][0] == 'v') {
			// parse vertex information
			if (file_lines[i][1] == ' ') {
				// vertex coordinate
				sscanf(file_lines[i].c_str(), "v %f %f %f", &x, &y, &z);
				vertices.push_back(glm::vec3(x, y, z));
			}
			else if (file_lines[i][1] == 'n') {
				// vertex normal vector
				sscanf(file_lines[i].c_str(), "vn %f %f %f", &x, &y, &z);
				normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			}
			else if (file_lines[i][1] == 't') {
				// vertex texture coordinate
				sscanf(file_lines[i].c_str(), "vt %f %f", &u, &v);
				textures_coords.push_back(glm::vec2(u, v));
			}
		}
		else if (file_lines[i][0] == 'u' and file_lines[i][1] == 's' and file_lines[i][2] == 'e') {
			// use material
			sscanf(file_lines[i].c_str(),"usemtl %s", buf);
			current_material = string(buf);
		}
		else if (file_lines[i][0] == 'f' and file_lines[i][1] == ' ') {

			NI[0] = NI[1] = NI[2] = NI[3] = -1;
			VN[0] = VN[1] = VN[2] = VN[3] = -1;
			TC[0] = TC[1] = TC[2] = TC[3] = -1;

			// parse face information
			size_t C = std::count(file_lines[i].begin(), file_lines[i].end(),' ');
			if (C == 3) {
				// triangle
				if (file_lines[i].find("//") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d//%d %d//%d %d//%d",
						&VN[0],&NI[0], &VN[1],&NI[1], &VN[2],&NI[2]
					);
				}
				else if (file_lines[i].find("/") != string::npos) {
					sscanf(
						file_lines[i].c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&VN[0],&TC[0],&NI[0], &VN[1],&TC[1],&NI[1],
						&VN[2],&TC[2],&NI[2]
					);
				}
				else {
					sscanf(file_lines[i].c_str(),"f %d %d %d",&VN[0],&VN[1],&VN[2]);
				}

				//faces.push_back(face(NI,VN,TC,current_material,true,smooth_face));

				// make the face
				for (int i = 0; i < 3; ++i) {
					triangles.push_back(VN[i]);
					normal_idxs.push_back(NI[i]);
					texture_coord_idxs.push_back(TC[i]);
				}
				mat_ids.push_back(current_material);
			}
			else if (C == 4) {
				// quad
				if (file_lines[i].find("//") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d//%d %d//%d %d//%d %d//%d",
						&VN[0],&NI[0], &VN[1],&NI[1], &VN[2],&NI[2], &VN[3],&NI[3]
					);
				}
				else if (file_lines[i].find("/") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
						&VN[0],&TC[0],&NI[0], &VN[1],&TC[1],&NI[1],
						&VN[2],&TC[2],&NI[2], &VN[3],&TC[3],&NI[3]
					);
				}
				else {
					sscanf(
						file_lines[i].c_str(),"f %d %d %d %d",
						&VN[0],&VN[1],&VN[2],&VN[3]
					);
				}

				//faces.push_back(face(NI,VN,TC,current_material,true,smooth_face));

				// make two triangular faces
				// use the first 3 vertices: 0,1,2
				// gather info
				for (int i = 0; i < 3; ++i) {
					triangles.push_back(VN[i]);
					normal_idxs.push_back(NI[i]);
					texture_coord_idxs.push_back(TC[i]);
				}
				mat_ids.push_back(current_material);

				// use the other vertices
				NI[1] = NI[2]; NI[2] = NI[3];
				VN[1] = VN[2]; VN[2] = VN[3];
				TC[1] = TC[2]; TC[2] = TC[3];
				// gather info
				for (int i = 0; i < 3; ++i) {
					triangles.push_back(VN[i]);
					normal_idxs.push_back(NI[i]);
					texture_coord_idxs.push_back(TC[i]);
				}
				mat_ids.push_back(current_material);
			}
			else {
				cerr << "OBJ_reader::get_vertices_faces_normals: Error" << endl;
				cerr << "    Line " << i << " does not contain a triangle or a quad" << endl;
				cerr << "    Found: " << file_lines[i] << endl;
			}

			// if the current face needs smoothing
			// increment amount of smoothed faces
			if (smooth_face) {
				++S;
			}
		}
		else if (file_lines[i][0] == 's' and file_lines[i][1] == ' ') {
			if (file_lines[i][2] == '1') {
				smooth_face = true;
			}
			else if (file_lines[i][2] == 'o') {
				smooth_face = false;
			}
		}
	}
}

bool OBJ_reader::load_object(const string& dir, const string& fname, rendered_triangle_mesh& M) {
	directory = dir;
	filename = fname;

	#if defined (DEBUG)
	cout << "OBJ_reader::load_object: Loading file '" << directory + "/" + filename << "'" << endl;
	#endif

	if (not load_file()) {
		cerr << "OBJ_reader::load_object: Error" << endl;
		cerr << "    File '" << filename << "' could not be loaded" << endl;
		return false;
	}

	// Find the line starting with 'mtl'
	size_t aux = 0;
	while (file_lines[aux].substr(0,3) != "mtl") {
		++aux;
	}
	// We have found the name of the material and its file name.
	// Read the material file now
	if (file_lines[aux].substr(0,3) == "mtl") {
		// parse the file name using sscanf
		char mtl_filename[256];
		sscanf(file_lines[aux].c_str(), "mtllib %s", mtl_filename);

		// parse the material file
		bool r = load_material(mtl_filename);
		if (not r) {
			cerr << "OBJ_reader::load_object: Warning" << endl;
			cerr << "    Material file '" << mtl_filename << "' was found" << endl;
			cerr << "    at line " << aux << " but could not be read." << endl;
		}
	}
	// Reading object name.
	// If this name can't be read -> error
	++aux;
	char buffer_object_name[128];
	if (file_lines[aux][0] == 'o') {
		sscanf(file_lines[aux].c_str(),"o %s", buffer_object_name);
		mesh_name = buffer_object_name;
	}
	else {
		cerr << "OBJ_reader::load_object: Error" << endl;
		cerr << "    Bad .obj file format." << endl;
		cerr << "    At line " << aux << " expected 'o %s', but found" << endl;
		cerr << "        '" << file_lines[aux] << "'" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << "    Parsing object '" << mesh_name << "'" << endl;
	cout << "    Object '" << mesh_name << "' is described from line "
		 << aux << " to line " << file_lines.size() << endl;
	#endif

	// parse the vertices and faces information
	parse_file_lines(4, file_lines.size() - 1);

	#if defined (DEBUG)
	cout << "    file '" << filename << "' "
		 << "completely parsed" << endl;
	#endif

	// clear file contents now to avoid potential
	// excessive memory consumption
	file_lines.clear();

	// Vertices are denoted as indices greater than 0
	// we want them greater than or EQUAL TO 0.
	// Same for normal indices and txeture coordinates
	for (size_t i = 0; i < triangles.size(); ++i) {
		if (triangles[i] != -1) {
			--triangles[i];
		}
		if (normal_idxs[i] != -1) {
			--normal_idxs[i];
		}
		if (texture_coord_idxs[i] != -1) {
			--texture_coord_idxs[i];
		}
	}

	M.set_name(mesh_name);

	M.set_vertices(vertices);
	M.set_triangles(triangles);
	M.set_normals(normals);
	M.set_normal_idxs(normal_idxs);

	M.set_materials(materials, mat_ids);
	M.set_texture_coords(textures_coords);
	M.set_texture_coord_idxs(texture_coord_idxs);

	#if defined (DEBUG)
	cout << "    object '" << mesh_name << "' ";
	cout << "from file '" << filename << "' was loaded" << endl;

	mesh_state state = M.state();
	if (state != mesh_state::correct) {
		cout << "    the mesh contains errors" << endl;
		return false;
	}
	else {
		cout << "    the mesh is in a correct state" << endl;
	}
	#endif

	clean();

	return true;
}
