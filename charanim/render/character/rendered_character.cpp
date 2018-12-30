#include <render/character/rendered_character.hpp>

// C++ includes
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

// glm includes
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
using namespace glm;

// render includes
#include <render/include_gl.hpp>
#include <render/err_war_utils.hpp>

// PRIVATE

void rendered_character::clear_buffer(uint& buf_id) {
	if (buf_id > 0) {
		#if defined(DEBUG)
		cout << line
			 << " rendered_character::clear_buffers() - delete buffer " << buf_id
			 << endl;
		#endif
		glDeleteBuffers(1, &buf_id);
		buf_id = 0;
	}
}

// PUBLIC

rendered_character::rendered_character() {
	core_model = nullptr;

	VAO = VBO_verts = VBO_normals = VBO_tex_coords = EBO = 0;
}

rendered_character::~rendered_character() {
	core_model = nullptr;

	clear_buffers();
}

void rendered_character::clear_buffers() {
	clear_buffer(VAO);
	clear_buffer(VBO_verts);
	clear_buffer(VBO_normals);
	clear_buffer(VBO_tex_coords);
	clear_buffer(IBO_mats);
	clear_buffer(IBO_texs);
	clear_buffer(EBO);
}

void rendered_character::set_cal_info(
	shared_ptr<CalCoreModel> cm, shared_ptr<CalModel> m
)
{
	core_model = cm;
	model = m;

	model->update(0.0001f);

	vmin = glm::vec3( numeric_limits<float>::max());
	vmax = glm::vec3(-numeric_limits<float>::max());

	// auxiliar
	float verts[30000][3], __tex_coords[30000][2];
	int faces[30000][3];

	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << ERR << endl;
		cerr << "    Could not begin render" << endl;
		return;
	}

	int total_vertices = 0;
	int total_faces = 0;

	// one pass to
	// 1. allocate memory for vertices, normals and tex_coords
	// 2. store material and texture indices
	// 3. store corner indices

	int n_meshes = cal_renderer->getMeshCount();
	for (int mesh_id = 0; mesh_id < n_meshes; ++mesh_id) {
		int n_submeshes = cal_renderer->getSubmeshCount(mesh_id);
		for (int submesh_id = 0; submesh_id < n_submeshes; ++submesh_id) {

			bool s = cal_renderer->selectMeshSubmesh(mesh_id, submesh_id);
			if (not s) {
				cerr << "Error (" << __LINE__ << "):" << endl;
				cerr << "    when selecting submesh " << submesh_id
					 << " from mesh " << mesh_id << endl;
				continue;
			}

			// retrieve vertices
			int n_verts = cal_renderer->getVertices(&verts[0][0]);
			// retrieve corners
			int n_faces = cal_renderer->getFaces(&faces[0][0]);
			// retrieve texture coordinates
			int n_texes = cal_renderer->getTextureCoordinates(0, &__tex_coords[0][0]);

			// bounding box
			for (int i = 0; i < n_verts; ++i) {
				vmin = glm::min(vmin, vec3(verts[i][0], verts[i][1], verts[i][2]));
				vmax = glm::max(vmax, vec3(verts[i][0], verts[i][1], verts[i][2]));
			}

			/* retrieve material */
			unsigned char colC[4];
			material M;

#define to_vec4(C) glm::vec4(C[0]/255.0f,C[1]/255.0f,C[2]/255.0f,C[3]/255.0f)

			// retrieve ambient color
			cal_renderer->getAmbientColor(&colC[0]);
			M.Ka = to_vec4(colC);
			// retrieve diffuse color
			cal_renderer->getDiffuseColor(&colC[0]);
			M.Kd = to_vec4(colC);
			// retrieve specular color
			cal_renderer->getSpecularColor(&colC[0]);
			M.Ks = to_vec4(colC);

			// shininess
			M.Ns = cal_renderer->getShininess();
			// OpenGL index of the texture
			M.txt_id =
			static_cast<uint>(
			reinterpret_cast<uintptr_t>(cal_renderer->getMapUserData(0))
			);

			all_mats.push_back(M);

			// increment counters
			total_vertices += n_verts;
			total_faces += n_faces;
			faces_per_submesh.push_back(n_faces);

			if (n_texes > 0) {
				tex_coords.insert
				(tex_coords.end(),
					&__tex_coords[0][0],
					&__tex_coords[0][0] + 2*n_texes
				);
			}
			else {
				tex_coords.insert(tex_coords.end(), 2*n_verts, 0.0f);
			}

			// for every vertex in this submesh, add
			// a material index and a texture index			
			mat_idxs.insert(mat_idxs.end(), n_verts, all_mats.size() - 1);
			tex_idxs.insert(tex_idxs.end(), n_verts, M.txt_id);

			// store corner indices
			int size_pre = total_vertices - n_verts;
			transform(
				&faces[0][0], &faces[0][0] + 3*n_faces, back_inserter(corners),
				[size_pre](int f) -> int { return f + size_pre; }
			);
		}
	}

	vertices.resize(3*total_vertices);
	normals.resize(3*total_vertices);
	tex_coords.resize(2*total_vertices);

	cal_renderer->endRendering();
}

void rendered_character::initialise_buffers() {
	glGenVertexArrays(1, &VAO);
	assert(glGetError() == GL_NO_ERROR);

	GLuint buffs[6];
	glGenBuffers(6, buffs);
	assert(glGetError() == GL_NO_ERROR);

	VBO_verts = buffs[0];
	VBO_normals = buffs[1];
	VBO_tex_coords = buffs[2];
	IBO_mats = buffs[3];
	IBO_texs = buffs[4];
	EBO = buffs[5];

	#if defined (DEBUG)
	cout << line << " rendered_character::initialise_buffers " << line
		 << " - buffers initialised" << endl;
	cout << line << "                VAO: " << VAO << endl;
	cout << line << "          VBO_verts: " << VBO_verts << endl;
	cout << line << "        VBO_normals: " << VBO_normals << endl;
	cout << line << "     VBO_tex_coords: " << VBO_tex_coords << endl;
	cout << line << "           IBO_mats: " << IBO_mats << endl;
	cout << line << "           IBO_texs: " << IBO_texs << endl;
	cout << line << "                EBO: " << EBO << endl;
	#endif

	// store some data that will never change:
	// everything except vertices and normals

	// bind VAO
	glBindVertexArray(VAO);

	// ---------------------
	// VBO_tex_coords: texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tex_coords);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData
	(GL_ARRAY_BUFFER, tex_coords.size()*sizeof(float), &tex_coords[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// IBO_mats fill
	glBindBuffer(GL_ARRAY_BUFFER, IBO_mats);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, mat_idxs.size()*sizeof(int), &mat_idxs[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, 0, (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(3);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// IBO_texs fill
	glBindBuffer(GL_ARRAY_BUFFER, IBO_texs);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, tex_idxs.size()*sizeof(int), &tex_idxs[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, 0, (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(4);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// EBO fill
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData
	(GL_ELEMENT_ARRAY_BUFFER, corners.size()*sizeof(uint), &corners[0], GL_STATIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
	// ---------------------

	// VAO release
	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);
}

bool rendered_character::flatten_data() {

	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << ERR << endl;
		cerr << "    Could not begin render" << endl;
		return false;
	}

	size_t vertex_counter = 0;
	size_t normal_counter = 0;

	int n_meshes = cal_renderer->getMeshCount();
	for (int mesh_id = 0; mesh_id < n_meshes; ++mesh_id) {

		int n_submeshes = cal_renderer->getSubmeshCount(mesh_id);
		for (int submesh_id = 0; submesh_id < n_submeshes; ++submesh_id) {

			bool s = cal_renderer->selectMeshSubmesh(mesh_id, submesh_id);
			if (not s) {
				cerr << "Error (" << __LINE__ << "):" << endl;
				cerr << "    when selecting submesh " << submesh_id
					 << " from mesh " << mesh_id << endl;
				continue;
			}

			// retrieve vertices
			int n_verts = cal_renderer->getVertices(&vertices[vertex_counter]);

			// retrieve normals
			cal_renderer->getNormals(&normals[normal_counter]);

			vertex_counter += 3*static_cast<size_t>(n_verts);
			normal_counter += 3*static_cast<size_t>(n_verts);
		}
	}

	cal_renderer->endRendering();
	return true;
}

void rendered_character::fill_buffers() {
	// send to memory only that data that changes continuously:
	// vertices and normals

	// bind VAO
	glBindVertexArray(VAO);

	// ---------------------
	// VBO_vertices: vertex coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO_verts);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData
	(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// VBO_normals: normal coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData
	(GL_ARRAY_BUFFER, normals.size()*sizeof(float), &normals[0], GL_DYNAMIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);

	// VAO release
	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);
}

void rendered_character::render() const {
	glBindVertexArray(VAO);
	assert(glGetError() == GL_NO_ERROR);

	glDrawElements(GL_TRIANGLES, corners.size(), GL_UNSIGNED_INT, 0);
	assert(glGetError() == GL_NO_ERROR);

	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);
}

void rendered_character::draw() const {
	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << ERR << endl;
		cerr << "    Could not begin render" << endl;
		return;
	}

	float verts[30000][3];
	float normals[30000][3];
	float tex_coords[30000][2];
	int faces[30000][3];

	size_t mat_idx = 0;

	int n_meshes = cal_renderer->getMeshCount();
	for (int mesh_id = 0; mesh_id < n_meshes; ++mesh_id) {

		int n_submeshes = cal_renderer->getSubmeshCount(mesh_id);
		for (int submesh_id = 0; submesh_id < n_submeshes; ++submesh_id, ++mat_idx) {

			bool s = cal_renderer->selectMeshSubmesh(mesh_id, submesh_id);
			if (not s) {
				cerr << "Error (" << __LINE__ << "):" << endl;
				cerr << "    when selecting submesh " << submesh_id
					 << " from mesh " << mesh_id << endl;
				continue;
			}

			// retrieve vertices
			cal_renderer->getVertices(&verts[0][0]);
			// retrieve normals
			cal_renderer->getNormals(&normals[0][0]);
			// retrieve texture coordinates
			int n_tex_coords =
				cal_renderer->getTextureCoordinates(0, &tex_coords[0][0]);

			// retrieve face indices
			int n_faces = cal_renderer->getFaces(&faces[0][0]);

			glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(all_mats[mat_idx].Kd));
			glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(all_mats[mat_idx].Ka));
			glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(all_mats[mat_idx].Ks));
			glMaterialf(GL_FRONT, GL_SHININESS, all_mats[mat_idx].Ns);

			bool textenable = false;
			// set the texture coordinate buffer and state if necessary
			if ((cal_renderer->getMapCount() > 0) and (n_tex_coords > 0)) {
				textenable = true;
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,
					static_cast<uint>(
					reinterpret_cast<uintptr_t>(cal_renderer->getMapUserData(0))
					)
				);
			}
			else {
				glDisable(GL_TEXTURE_2D);
			}

			for (int f = 0; f < n_faces; ++f) {
				glBegin(GL_TRIANGLES);
				for (int k = 0; k < 3; ++k) {
					int v = faces[f][k];
					if (textenable) {
						glTexCoord2f(tex_coords[v][0], tex_coords[v][1]);
					}
					glNormal3f(normals[v][0], normals[v][1], normals[v][2]);
					glVertex3f(verts[v][0], verts[v][1], verts[v][2]);
				}
				glEnd();
			}
		}
	}
	cal_renderer->endRendering();

	/*
	// for every submesh
	size_t corner_it = 0;
	for (size_t fps_it = 0; fps_it < faces_per_submesh.size(); ++fps_it) {

		// Faces of current submesh. This submesh has only
		// one material.
		int faces_submesh = faces_per_submesh[fps_it];

		// corner_it points to the first vertex of the
		// current submesh
		uint v_idx = corners[corner_it];
		uint mat_idx = mat_idxs[v_idx];
		uint tex_idx = tex_idxs[v_idx];

		if (tex_idx > 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex_idx);
		}
		else {
			glDisable(GL_TEXTURE_2D);
		}

		glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(all_mats[mat_idx].Ka));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(all_mats[mat_idx].Kd));
		glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(all_mats[mat_idx].Ks));
		glMaterialf(GL_FRONT, GL_SHININESS, all_mats[mat_idx].Ns);

		// for each face of submesh
		for (size_t f = corner_it; f < corner_it + 3*faces_submesh; f += 3) {
			glBegin(GL_TRIANGLES);

			for (size_t k = 0; k < 3; ++k) {
				uint vk = corners[f + k];
				if (tex_idx > 0) {
					glTexCoord2f(tex_coords[2*vk + 0], tex_coords[2*vk + 1]);
				}
				glNormal3f(normals[3*vk + 0], normals[3*vk + 1], normals[3*vk + 2]);
				glVertex3f(vertices[3*vk + 0], vertices[3*vk + 1], vertices[3*vk + 2]);
			}

			glEnd();
		}

		corner_it += 3*static_cast<size_t>(faces_submesh);
	}
	*/
}

// GETTERS

void rendered_character::get_bounding_box(vec3& _vmin, vec3& _vmax) const {
	_vmin = vmin;
	_vmax = vmax;
}

glm::vec3 rendered_character::get_center() const {
	return (vmin + vmax)/2.0f;
}

const std::vector<material>& rendered_character::get_materials() const {
	return all_mats;
}

shared_ptr<CalCoreModel> rendered_character::get_core_model() {
	return core_model;
}
const shared_ptr<CalCoreModel> rendered_character::get_core_model() const {
	return core_model;
}

shared_ptr<CalModel> rendered_character::get_model() {
	return model;
}
const shared_ptr<CalModel> rendered_character::get_model() const {
	return model;
}

