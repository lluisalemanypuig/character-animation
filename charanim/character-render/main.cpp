// C includes
#include <string.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <memory>
#include <string>
using namespace std;

// Cal3d includes
#include <cal3d/cal3d.h>
#include <cal3d/coremodel.h>
#include <cal3d/coresubmesh.h>

// render includes
#include <render/include_gl.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/shader/shader.hpp>
#include <render/scene/viewer.hpp>
#include <render/character/character_reader.hpp>

void info(shared_ptr<CalCoreModel> core_model) {
	cout << "Information about the cal_core_model:" << endl;
	cout << "    It has: " << core_model->getCoreMeshCount() << " meshes" << endl;
	for (int i = 0; i < core_model->getCoreMeshCount(); ++i) {
		CalCoreMesh *mesh = core_model->getCoreMesh(i);
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
}

int main() {
	shared_ptr<CalCoreModel> core_model = nullptr;
	shared_ptr<CalModel> model = nullptr;

	bool res = character_reader::load_core_model(
		"../../characters", "paladin.cfg", "dummy",
		core_model, model
	);

	if (not res) {
		cerr << "Error: when loading model 'paladin'" << endl;
		return 1;
	}
	if (core_model == nullptr) {
		cerr << "Error: CalCoreModel not created" << endl;
		return 1;
	}
	if (model == nullptr) {
		cerr << "Error: CalModel not created" << endl;
		return 1;
	}

	//info(core_model);

	size_t total_submeshes = 0;
	for (int i = 0; i < core_model->getCoreMeshCount(); ++i) {
		CalCoreMesh *mesh = core_model->getCoreMesh(i);
		total_submeshes += static_cast<size_t>(mesh->getCoreSubmeshCount());
	}

	model->update(0.01f);

	CalRenderer *cal_renderer = model->getRenderer();
	bool begin_render = cal_renderer->beginRendering();
	if (not begin_render) {
		cerr << "Error (" << __LINE__ << "):" << endl;
		cerr << "    Could not begin render" << endl;
		return 1;
	}

	vector<float> all_data;
	vector<int> all_indices;

	cout << "Building triangle meshes..." << endl;

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
	cout << "All data: " << all_data.size() << endl;
	for (size_t i = 0; i < all_data.size(); i += 8) {
		cout << i << ":" << endl;
		cout << "    vertex: " << all_data[i + 0] << endl;
		cout << "            " << all_data[i + 1] << endl;
		cout << "            " << all_data[i + 2] << endl;
		cout << "   normals: " << all_data[i + 3] << endl;
		cout << "            " << all_data[i + 4] << endl;
		cout << "            " << all_data[i + 5] << endl;
		cout << "tex coords: " << all_data[i + 6] << endl;
		cout << "            " << all_data[i + 7] << endl;
	}
	cout << "All indices: ";
	for (size_t i = 0; i < all_indices.size(); ++i) {
		cout << " " << all_indices[i];
	}
	cout << endl;
}

