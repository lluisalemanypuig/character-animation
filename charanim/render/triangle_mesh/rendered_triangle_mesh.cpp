#include <render/triangle_mesh/rendered_triangle_mesh.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/glm.hpp>

// render includes
#include <render/textures/texture_loader.hpp>
#include <render/err_war_utils.hpp>
#include <render/include_gl.hpp>

// PRIVATE

void rendered_triangle_mesh::fill_buffers
(const vector<float>& data, const vector<uint>& indices)
{
	// bind VAO
	glBindVertexArray(VAO);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// VBO fill
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	// vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3*sizeof(float)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// EBO fill
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint), &indices[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
	// ---------------------

	// VAO release
	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);
}

void rendered_triangle_mesh::fill_buffers_materials
(const std::vector<float>& data, const std::vector<int>& flat_idxs,
 const std::vector<uint>& indices)
{
	// bind VAO
	glBindVertexArray(VAO);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// VBO fill
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	// vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3*sizeof(float)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// IBO fill
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, flat_idxs.size()*sizeof(int), &flat_idxs[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	// indices (materials)
	glVertexAttribIPointer(2, 1, GL_INT, 0, (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// EBO fill
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint), &indices[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
	// ---------------------

	// VAO release
	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);
}

void rendered_triangle_mesh::fill_buffers_materials_textures
(const std::vector<float>& data, const std::vector<int>& flat_idxs,
 const std::vector<uint>& indices)
{
	// bind VAO
	glBindVertexArray(VAO);

	// ---------------------
	// VBO fill
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	// vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(3*sizeof(float)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);

	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *)(6*sizeof(float)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// IBO fill
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, flat_idxs.size()*sizeof(int), &flat_idxs[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	// indices (materials + textures)
	// -- materials
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribIPointer(3, 1, GL_INT, 2*sizeof(int), (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(3);
	assert(glGetError() == GL_NO_ERROR);

	// -- textures
	glVertexAttribIPointer(4, 1, GL_INT, 2*sizeof(int), (void *)(1*sizeof(int)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(4);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// EBO fill
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint), &indices[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
	// ---------------------

	// VAO release
	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);
}

// PUBLIC

rendered_triangle_mesh::rendered_triangle_mesh() : triangle_mesh() {
	list_index = 0;
	VAO = VBO = IBO = EBO = 0;
}

rendered_triangle_mesh::rendered_triangle_mesh(const rendered_triangle_mesh& m) : triangle_mesh(m) {
	materials = m.materials;
	mat_idxs = m.mat_idxs;
	texture_coords = m.texture_coords;
	texture_coord_idxs = m.texture_coord_idxs;

	list_index = 0;
	VAO = VBO = IBO = EBO = 0;
}

rendered_triangle_mesh::~rendered_triangle_mesh() {
	clear();
}

// SETTERS

void rendered_triangle_mesh::set_materials
(const vector<material>& mats, const vector<string>& material_ids)
{
	materials = mats;

	mat_idxs = vector<int>(triangles.size()/3, materials.size() + 1);
	for (size_t t = 0; t < triangles.size(); t += 3) {

		// material of (t/3)-th triangle
		const string& tri_mat = material_ids[t/3];
		if (tri_mat == __NULL_MATERIAL_NAME) {
			mat_idxs[t/3] = __NULL_MATERIAL_INDEX;
			continue;
		}

		int i = 0;
		bool finish = false;
		while (i < materials.size() and not finish) {
			if (materials[i].id == tri_mat) {
				mat_idxs[t/3] = i;
				finish = true;
			}
			++i;
		}
	}

	unique_mat_idxs.insert(mat_idxs.begin(), mat_idxs.end());
}

void rendered_triangle_mesh::set_texture_coords(const vector<glm::vec2>& texts) {
	texture_coords = texts;
}

void rendered_triangle_mesh::set_texture_coord_idxs(const vector<int>& text_coord_idxs) {
	texture_coord_idxs = text_coord_idxs;
}

// GETTERS

mesh_state rendered_triangle_mesh::state(const mesh_state& ignore) const {
	mesh_state s = triangle_mesh::state(ignore);
	if (s != mesh_state::correct) {
		return s;
	}

	// check that indexes are correct.
	for (size_t t = 0; t < triangles.size(); ++t) {
		if ((ignore & mesh_state::texture_coord_idx_ob) == 0) {
			if (texture_coord_idxs[t] != -1 and texture_coord_idxs[t] > texture_coords.size()) {
				cerr << "mesh::is_valid - " << ERR << endl;
				cerr << "    Triangle " << t/3 << " has " << t%3 << "-th "
					 << "texture index (" << texture_coord_idxs[t]
					 << ") out of bounds." << endl;
				return mesh_state::texture_coord_idx_ob;
			}
		}
	}

	return mesh_state::correct;
}

const std::vector<int>& rendered_triangle_mesh::get_material_idxs() const {
	return mat_idxs;
}

const std::set<int>& rendered_triangle_mesh::get_unique_material_idxs() const {
	return unique_mat_idxs;
}

const std::vector<material>& rendered_triangle_mesh::get_materials() const {
	return materials;
}

void rendered_triangle_mesh::load_textures() {
	texture_loader& load = texture_loader::get_loader();
	load.load_textures(materials, texture_openGL_idxs);
}

// MODIFIERS

void rendered_triangle_mesh::clear_graphics() {
	if (list_index > 0) {
		#if defined(DEBUG)
		cout << line << " rendered_mesh::clear() - delete OpenGL list" << endl;
		#endif
		glDeleteLists(list_index, 1);
		list_index = 0;
	}

	if (VAO > 0) {
		#if defined(DEBUG)
		cout << line << " rendered_mesh::clear() - delete VAO " << VAO << endl;
		#endif
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO > 0) {
		#if defined(DEBUG)
		cout << line << " rendered_mesh::clear() - delete VBO " << VBO << endl;
		#endif
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (IBO > 0) {
		#if defined(DEBUG)
		cout << line << " rendered_mesh::clear() - delete IBO " << IBO << endl;
		#endif
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (EBO > 0) {
		#if defined(DEBUG)
		cout << line << " rendered_mesh::clear() - delete EBO " << EBO << endl;
		#endif
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
}

void rendered_triangle_mesh::clear() {
	triangle_mesh::clear();
	clear_graphics();

	materials.clear();
	mat_idxs.clear();
	unique_mat_idxs.clear();

	texture_coords.clear();
	texture_coord_idxs.clear();
	texture_openGL_idxs.clear();
}

bool rendered_triangle_mesh::uses_lists() const {
	return list_index > 0;
}

bool rendered_triangle_mesh::uses_buffers() const {
	return VBO > 0 and EBO > 0;
}

// OTHERS

void rendered_triangle_mesh::display_mesh_info() const {
	triangle_mesh::display_mesh_info();
	cout << "    # Materials= " << materials.size() << endl;
	cout << "    # Texture coordinates= " << texture_coords.size() << endl;
	uint ntxts = 0;
	for (const material& m : materials) {
		if (m.txt_id != 0) {
			++ntxts;
		}
	}
	cout << "    # Textures: " << ntxts << endl;

	cout << "    Materials: " << endl;
	for (size_t i = 0; i < materials.size(); ++i) {
		cout << "        newmtl " << materials[i].id << endl;
		cout << "        Ns " << materials[i].Ns;
		cout << endl;
		cout << "        Ka: " << materials[i].Ka[0];
		for (size_t j = 1; j < 4; ++j) {
			cout << "," << materials[i].Ka[j];
		}
		cout << endl;
		cout << "        Kd: " << materials[i].Kd[0];
		for (size_t j = 1; j < 4; ++j) {
			cout << "," << materials[i].Kd[j];
		}
		cout << endl;
		cout << "        Ks: " << materials[i].Ks[0];
		for (size_t j = 1; j < 4; ++j) {
			cout << "," << materials[i].Ks[j];
		}
		cout << "        Ni " << materials[i].Ni << endl;
		cout << "        illum " << materials[i].illum << endl;
		cout << "        map_Kd " << materials[i].txt_id << endl;
		cout << endl;
	}
}

void rendered_triangle_mesh::slow_render() const {
	for (size_t t = 0; t < triangles.size(); t += 3) {
		// set the material of the face
		bool textenable = false;
		// material of the face
		int M = mat_idxs[t/3];
		// set the material so that the lighting works
		if (M != __NULL_MATERIAL_INDEX) {
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  &(materials[M].Kd[0]));
			glMaterialfv(GL_FRONT,GL_AMBIENT,  &(materials[M].Ka[0]));
			glMaterialfv(GL_FRONT,GL_SPECULAR, &(materials[M].Ks[0]));
			glMaterialf(GL_FRONT,GL_SHININESS, materials[M].Ns);

			if (materials[M].txt_id == __NULL_TEXTURE_INDEX) {
				glDisable(GL_TEXTURE_2D);
			}
			else {
				textenable = true;
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, materials[M].txt_id);
			}
		}

		glBegin(GL_TRIANGLES);
		for (size_t i = t; i < t + 3; ++i) {
			if (textenable) {
				const glm::vec2& uv = texture_coords[ texture_coord_idxs[i] ];
				glTexCoord2f(uv.x, 1.0 - uv.y);
			}

			int vrtx_idx = triangles[i];

			if (normals.size() > 0) {
				const glm::vec3& n = normals[ normal_idxs[i] ];
				glNormal3f(n.x, n.y, n.z);
			}

			const glm::vec3& v = vertices[vrtx_idx];
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
}

uint rendered_triangle_mesh::compile() {
	if (list_index == 0) {
		list_index = glGenLists(1);
		glNewList(list_index, GL_COMPILE);
		slow_render();
		glEndList();
	}

	#if defined (DEBUG)
	cout << line << " rendered_mesh::compile " << line << ":" << endl;
	cout << line << "     Object compiled into list with index: " << list_index << endl;
	#endif
	return list_index;
}

void rendered_triangle_mesh::make_buffers() {

	vector<float> data(2*3*triangles.size());
	vector<uint> indices(triangles.size());

	for (size_t t = 0; t < triangles.size(); ++t) {
		data[6*t    ] = vertices[ triangles[t] ].x;
		data[6*t + 1] = vertices[ triangles[t] ].y;
		data[6*t + 2] = vertices[ triangles[t] ].z;

		glm::vec3 norm = glm::normalize(normals[ normal_idxs[t] ]);
		data[6*t + 3] = norm.x;
		data[6*t + 4] = norm.y;
		data[6*t + 5] = norm.z;

		indices[t] = t;
	}

	glGenVertexArrays(1, &VAO);
	assert(glGetError() == GL_NO_ERROR);

	uint buffs[2];
	glGenBuffers(2, buffs);
	assert(glGetError() == GL_NO_ERROR);

	VBO = buffs[0];
	EBO = buffs[1];

	fill_buffers(data, indices);

	#if defined (DEBUG)
	cout << line << " rendered_mesh::make_buffers() " << line << " - buffers made" << endl;
	cout << line << "     VAO: " << VAO << endl;
	cout << line << "     VBO: " << VBO << endl;
	cout << line << "     EBO: " << EBO << endl;
	#endif
}
void rendered_triangle_mesh::remake_buffers() {

	vector<float> data(2*3*triangles.size());
	vector<uint> indices(triangles.size());

	for (size_t t = 0; t < triangles.size(); ++t) {
		data[6*t    ] = vertices[ triangles[t] ].x;
		data[6*t + 1] = vertices[ triangles[t] ].y;
		data[6*t + 2] = vertices[ triangles[t] ].z;

		glm::vec3 norm = glm::normalize(normals[ normal_idxs[t] ]);
		data[6*t + 3] = norm.x;
		data[6*t + 4] = norm.y;
		data[6*t + 5] = norm.z;

		indices[t] = t;
	}

	fill_buffers(data, indices);
}

void rendered_triangle_mesh::make_buffers_materials() {

	vector<float> data((3 + 3)*triangles.size());
	vector<int> flat_idxs(triangles.size());
	vector<uint> indices(triangles.size());

	for (size_t t = 0; t < triangles.size(); ++t) {
		data[6*t    ] = vertices[ triangles[t] ].x;
		data[6*t + 1] = vertices[ triangles[t] ].y;
		data[6*t + 2] = vertices[ triangles[t] ].z;

		glm::vec3 norm = glm::normalize(normals[ normal_idxs[t] ]);
		data[6*t + 3] = norm.x;
		data[6*t + 4] = norm.y;
		data[6*t + 5] = norm.z;

		flat_idxs[t] = mat_idxs[t/3];

		indices[t] = t;
	}

	glGenVertexArrays(1, &VAO);
	assert(glGetError() == GL_NO_ERROR);

	uint buffs[3];
	glGenBuffers(3, buffs);
	assert(glGetError() == GL_NO_ERROR);

	VBO = buffs[0];
	IBO = buffs[1];
	EBO = buffs[2];

	fill_buffers_materials(data, flat_idxs, indices);

	#if defined (DEBUG)
	cout << line << " rendered_mesh::make_buffers_materials() " << line << " - buffers made" << endl;
	cout << line << "     VAO: " << VAO << endl;
	cout << line << "     VBO: " << VBO << endl;
	cout << line << "     IBO: " << IBO << endl;
	cout << line << "     EBO: " << EBO << endl;
	#endif
}
void rendered_triangle_mesh::remake_buffers_materials() {

	vector<float> data((3 + 3)*triangles.size());
	vector<int> flat_idxs(triangles.size());
	vector<uint> indices(triangles.size());

	for (size_t t = 0; t < triangles.size(); ++t) {
		data[6*t    ] = vertices[ triangles[t] ].x;
		data[6*t + 1] = vertices[ triangles[t] ].y;
		data[6*t + 2] = vertices[ triangles[t] ].z;

		glm::vec3 norm = glm::normalize(normals[ normal_idxs[t] ]);
		data[6*t + 3] = norm.x;
		data[6*t + 4] = norm.y;
		data[6*t + 5] = norm.z;

		flat_idxs[t] = mat_idxs[t/3];

		indices[t] = t;
	}

	fill_buffers_materials(data, flat_idxs, indices);
}

void rendered_triangle_mesh::make_buffers_materials_textures() {

	vector<float> data((3 + 3 + 2)*triangles.size());
	vector<int> flat_idxs((1 + 1)*triangles.size());
	vector<uint> indices(triangles.size());

	for (size_t t = 0; t < triangles.size(); ++t) {
		const glm::vec3& vert = vertices[ triangles[t] ];
		data[8*t    ] = vert.x;
		data[8*t + 1] = vert.y;
		data[8*t + 2] = vert.z;

		glm::vec3 norm = glm::normalize(normals[ normal_idxs[t] ]);
		data[8*t + 3] = norm.x;
		data[8*t + 4] = norm.y;
		data[8*t + 5] = norm.z;

		if (texture_coord_idxs[t] > 0) {
			const glm::vec2& tex = texture_coords[ texture_coord_idxs[t] ];
			data[8*t + 6] = tex.x;
			data[8*t + 7] = 1.0f - tex.y;
		}

		int M = mat_idxs[t/3];
		flat_idxs[2*t    ] = M;
		if (M != __NULL_MATERIAL_INDEX) {
			flat_idxs[2*t + 1] = materials[M].txt_id;
		}

		indices[t] = t;
	}

	glGenVertexArrays(1, &VAO);
	assert(glGetError() == GL_NO_ERROR);

	GLuint buffs[3];
	glGenBuffers(3, buffs);
	assert(glGetError() == GL_NO_ERROR);

	VBO = buffs[0];
	IBO = buffs[1];
	EBO = buffs[2];

	fill_buffers_materials_textures(data, flat_idxs, indices);

	#if defined (DEBUG)
	cout << line << " rendered_mesh::make_buffers_materials_textures() " << line << " - buffers made" << endl;
	cout << line << "     VAO: " << VAO << endl;
	cout << line << "     VBO: " << VBO << endl;
	cout << line << "     IBO: " << IBO << endl;
	cout << line << "     EBO: " << EBO << endl;
	#endif
}
void rendered_triangle_mesh::remake_buffers_materials_textures() {

	vector<float> data((3 + 3 + 2)*triangles.size());
	vector<int> flat_idxs((1 + 1)*triangles.size());
	vector<uint> indices(triangles.size());

	for (size_t t = 0; t < triangles.size(); ++t) {
		const glm::vec3& vert = vertices[ triangles[t] ];
		data[8*t    ] = vert.x;
		data[8*t + 1] = vert.y;
		data[8*t + 2] = vert.z;

		glm::vec3 norm = glm::normalize(normals[ normal_idxs[t] ]);
		data[8*t + 3] = norm.x;
		data[8*t + 4] = norm.y;
		data[8*t + 5] = norm.z;

		if (texture_coord_idxs[t] > 0) {
			const glm::vec2& tex = texture_coords[ texture_coord_idxs[t] ];
			data[8*t + 6] = tex.x;
			data[8*t + 7] = 1.0f - tex.y;
		}

		int M = mat_idxs[t/3];
		flat_idxs[2*t    ] = M;
		if (M != __NULL_MATERIAL_INDEX) {
			flat_idxs[2*t + 1] = materials[M].txt_id;
		}

		indices[t] = t;
	}

	fill_buffers_materials_textures(data, flat_idxs, indices);
}

void rendered_triangle_mesh::render() const {
	if (uses_buffers()) {
		glBindVertexArray(VAO);
		assert(glGetError() == GL_NO_ERROR);

		glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
		assert(glGetError() == GL_NO_ERROR);

		glBindVertexArray(0);
		assert(glGetError() == GL_NO_ERROR);
	}
	else if (uses_lists()) {
		glCallList(list_index);
	}
	else {
		slow_render();
	}
}
