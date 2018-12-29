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

void rendered_character::set_cal_info(
	shared_ptr<CalCoreModel> cm, shared_ptr<CalModel> m
)
{
	core_model = cm;
	model = m;

	// store materials

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
}

bool rendered_character::fill_buffers() {
	// only God knows how big this will be...
	vector<float> all_data;
	vector<int> all_indices;

	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << ERR << endl;
		cerr << "    Could not begin render" << endl;
		return false;
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

			// retrieve vertices
			float verts[30000][3];
			int n_verts = cal_renderer->getVertices(&verts[0][0]);

			// retrieve normals
			float normals[30000][3];
			int n_normals = cal_renderer->getNormals(&normals[0][0]);

			// retrieve texture coordinates
			float tex_coords[30000][2];
			int n_tex_coords = cal_renderer->getTextureCoordinates(0, &tex_coords[0][0]);

			// store retrieved info
			vector<float> submesh_data(3*n_verts + 3*n_normals + 2*n_tex_coords);
			size_t v_it, n_it, t_it;
			v_it = n_it = t_it = 0;
			for (size_t it = 0; it < submesh_data.size(); it += 8) {
				if (v_it < n_verts) {
					submesh_data[it + 0] = verts[v_it][0];
					submesh_data[it + 1] = verts[v_it][1];
					submesh_data[it + 2] = verts[v_it][2];
				}
				if (n_it < n_normals) {
					submesh_data[it + 3] = normals[n_it][0];
					submesh_data[it + 4] = normals[n_it][1];
					submesh_data[it + 5] = normals[n_it][2];
				}
				if (t_it < n_tex_coords) {
					submesh_data[it + 6] = tex_coords[t_it][0];
					submesh_data[it + 7] = tex_coords[t_it][1];
				}

				++v_it;
				++n_it;
				++t_it;
			}
			all_data.insert(all_data.end(), submesh_data.begin(), submesh_data.end());

			// retrieve face indices
			int n_faces = cal_renderer->getFaceCount();
			vector<int> faces(3*n_faces);
			int r_faces = cal_renderer->getFaces(&faces[0]);
			assert(r_faces == n_faces);

			int N = all_indices.size();
			transform(
				faces.begin(), faces.end(),
				back_inserter(all_indices),
				[N](int k) -> int { return k + N; }
			);
		}
	}

	cal_renderer->endRendering();
	return true;
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
