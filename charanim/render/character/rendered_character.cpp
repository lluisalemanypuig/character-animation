#include <render/character/rendered_character.hpp>

// C++ includes
#include <algorithm>
using namespace std;

// render includes
#include <render/include_gl.hpp>
#include <render/err_war_utils.hpp>

rendered_character::rendered_character() {
	core_model = nullptr;

	VAO = VBO = IBO = EBO = 0;
}

rendered_character::~rendered_character() {
	core_model = nullptr;

	clear_buffers();
}

void rendered_character::clear_buffers() {
	if (VAO > 0) {
		#if defined(DEBUG)
		cout << line
			 << " rendered_character::clear_buffers() - delete VAO " << VAO
			 << endl;
		#endif
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO > 0) {
		#if defined(DEBUG)
		cout << line
			 << " rendered_character::clear_buffers() - delete VBO " << VBO
			 << endl;
		#endif
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (IBO > 0) {
		#if defined(DEBUG)
		cout << line
			 << " rendered_character::clear_buffers() - delete IBO " << IBO
			 << endl;
		#endif
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (EBO > 0) {
		#if defined(DEBUG)
		cout << line
			 << " rendered_character::clear_buffers() - delete EBO " << EBO
			 << endl;
		#endif
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
}

void rendered_character::set_cal_info(
	shared_ptr<CalCoreModel> cm, shared_ptr<CalModel> m
)
{
	core_model = cm;
	model = m;
}

void rendered_character::initialise_buffers() {
	glGenVertexArrays(1, &VAO);
	assert(glGetError() == GL_NO_ERROR);

	GLuint buffs[3];
	glGenBuffers(3, buffs);
	assert(glGetError() == GL_NO_ERROR);

	VBO = buffs[0];
	IBO = buffs[1];
	EBO = buffs[2];

	#if defined (DEBUG)
	cout << line << " rendered_character::initialise_buffers " << line
		 << " - buffers initialised" << endl;
	cout << line << "     VAO: " << VAO << endl;
	cout << line << "     VBO: " << VBO << endl;
	cout << line << "     IBO: " << IBO << endl;
	cout << line << "     EBO: " << EBO << endl;
	#endif

	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << ERR << endl;
		cerr << "    Could not begin render" << endl;
		return;
	}

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

			/* retrieve material */
			unsigned char colC[4];
			material M;

			// retrieve ambient color
			cal_renderer->getAmbientColor(&colC[0]);
			M.Ka = glm::vec4(colC[0]/255.0f, colC[1]/255.0f,
							 colC[2]/255.0f, 1.0f - colC[3]/255.0f);
			// retrieve diffuse color
			cal_renderer->getDiffuseColor(&colC[0]);
			M.Kd = glm::vec4(colC[0]/255.0f, colC[1]/255.0f,
							 colC[2]/255.0f, 1.0f - colC[3]/255.0f);
			// retrieve specular color
			cal_renderer->getSpecularColor(&colC[0]);
			M.Ks = glm::vec4(colC[0]/255.0f, colC[1]/255.0f,
							 colC[2]/255.0f, 1.0f - colC[3]/255.0f);

			// shininess
			M.Ns = cal_renderer->getShininess();
			// OpenGL index of the texture
			M.txt_id =
			reinterpret_cast<uintptr_t>(cal_renderer->getMapUserData(0));

			all_mats.push_back(M);
		}
	}

	cal_renderer->endRendering();

	#if defined(DEBUG)
	cout << line << " materials successfully made" << endl;
	#endif
}

bool rendered_character::fill_buffers() {
	data.clear();
	flat_idxs.clear();
	indices.clear();

	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << ERR << endl;
		cerr << "    Could not begin render" << endl;
		return false;
	}

	float verts[30000][3];
	float normals[30000][3];
	float tex_coords[30000][2];
	int faces[30000][3];

	int material_index;
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

			// retrieve normals
			int n_normals = cal_renderer->getNormals(&normals[0][0]);

			// retrieve texture coordinates
			int n_tex_coords =
				cal_renderer->getTextureCoordinates(0, &tex_coords[0][0]);

			// store retrieved info
			vector<float> submesh_data(3*n_verts + 3*n_normals + 2*n_tex_coords);
			size_t v_it, n_it, t_it;
			v_it = n_it = t_it = 0;
			for (size_t it = 0; it < submesh_data.size(); it += 8) {
				if (n_verts > 0 and v_it < n_verts) {
					submesh_data[it + 0] = verts[v_it][0];
					submesh_data[it + 1] = verts[v_it][1];
					submesh_data[it + 2] = verts[v_it][2];
				}
				if (n_normals > 0 and n_it < n_normals) {
					submesh_data[it + 3] = normals[n_it][0];
					submesh_data[it + 4] = normals[n_it][1];
					submesh_data[it + 5] = normals[n_it][2];
				}
				if (n_tex_coords > 0 and t_it < n_tex_coords) {
					submesh_data[it + 6] = tex_coords[t_it][0];
					submesh_data[it + 7] = tex_coords[t_it][1];
				}

				++v_it;
				++n_it;
				++t_it;
			}
			data.insert(data.end(), submesh_data.begin(), submesh_data.end());

			// retrieve face indices
			int n_faces = cal_renderer->getFaces(&faces[0][0]);
			// store indices
			int N = indices.size();
			transform(
				&faces[0][0], &faces[0][0] + 3*n_faces,
				back_inserter(indices),
				[N](int k) -> int { return k + N; }
			);

			// for each vertex, add the material index and
			// the texture id
			for (int i = 0; i < 3*n_faces; ++i) {
				flat_idxs.push_back(material_index);
				flat_idxs.push_back(all_mats[material_index].txt_id);
			}

			++material_index;
		}
	}

	cal_renderer->endRendering();

	/*
	data = vector<float>({0, 0, 0, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
						  1, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
						  1, 0, 1, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f,
						  0, 0, 1, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
						  0, 1, 0, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
						  1, 1, 0, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f,
						  1, 1, 1, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
						  0, 1, 1, 1.0f, 1.0f, 1.0f, 0.2f, 0.8f
						 });

	flat_idxs = vector<int>({1, 1,
							 1, 1,
							 1, 1,
							 1, 1,
							 1, 1,
							 1, 1,
							 1, 1,
							 1, 1
							});

	indices = vector<uint>({0,1,2, 0,2,3, 0,1,5, 0,5,4,
							0,3,7, 0,7,4, 1,2,6, 1,6,5,
							2,3,7, 2,7,6, 4,5,6, 4,6,7
						   });
	*/

	// bind VAO
	glBindVertexArray(VAO);

	// ---------------------
	// VBO fill
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float),
				 &data[0], GL_DYNAMIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	// vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
						  (void *)0);
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(0);
	assert(glGetError() == GL_NO_ERROR);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
						  (void *)(3*sizeof(float)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(1);
	assert(glGetError() == GL_NO_ERROR);

	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
						  (void *)(6*sizeof(float)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(2);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// IBO fill
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ARRAY_BUFFER, flat_idxs.size()*sizeof(int),
				 &flat_idxs[0], GL_DYNAMIC_DRAW);
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
	glVertexAttribIPointer(4, 1, GL_INT, 2*sizeof(int),
						   (void *)(1*sizeof(int)));
	assert(glGetError() == GL_NO_ERROR);

	glEnableVertexAttribArray(4);
	assert(glGetError() == GL_NO_ERROR);

	// ---------------------
	// EBO fill
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	assert(glGetError() == GL_NO_ERROR);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint),
				 &indices[0], GL_DYNAMIC_DRAW);
	assert(glGetError() == GL_NO_ERROR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assert(glGetError() == GL_NO_ERROR);
	// ---------------------

	// VAO release
	glBindVertexArray(0);
	assert(glGetError() == GL_NO_ERROR);

	return true;
}

void rendered_character::render() const {
	if (VBO > 0 and EBO > 0) {
		glBindVertexArray(VAO);
		assert(glGetError() == GL_NO_ERROR);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		assert(glGetError() == GL_NO_ERROR);

		glBindVertexArray(0);
		assert(glGetError() == GL_NO_ERROR);
	}
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
