// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
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

// custom includes
#include "character_reader.hpp"

static CalCoreModel *core_model;
static CalModel *model;

int main() {
	core_model = nullptr;
	model = nullptr;

	bool res = character_reader::load_core_model(
		"../../characters", "paladin.cfg", "dummy",
		&core_model, &model
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

	size_t total_materials = 0;

	cout << "Information about the cal_core_model:" << endl;
	cout << "    It has: " << core_model->getCoreMeshCount() << " meshes" << endl;
	for (int i = 0; i < core_model->getCoreMeshCount(); ++i) {
		CalCoreMesh *mesh = core_model->getCoreMesh(i);
		cout << "    mesh " << i << " has " << mesh->getCoreSubmeshCount() << " submeshes" << endl;
		for (int j = 0; j < mesh->getCoreSubmeshCount(); ++j) {
			CalCoreSubmesh *submesh = mesh->getCoreSubmesh(j);
			cout << "        submesh " << j << " has:" << endl;
			cout << "            " << submesh->getVertexCount() << " vertices:" << endl;

			/*cout << "            With coordinates: ";
			const vector<CalCoreSubmesh::Vertex>& verts = submesh->getVectorVertex();
			for (const CalCoreSubmesh::Vertex& v : verts) {
				const CalVector& pos = v.position;
				cout << "(" << pos[0] << "," << pos[1] << "," << pos[2] << "),";
			}
			cout << endl;*/

			cout << "            " << submesh->getVectorFace().size() << " triangles:" << endl;
			/*cout << "            With vertex indices: ";
			const vector<CalCoreSubmesh::Face>& faces = submesh->getVectorFace();
			for (const CalCoreSubmesh::Face& f : faces) {
				cout << "(" << f.vertexId[0] << "," << f.vertexId[1] << "," << f.vertexId[2] << "),";
			}
			cout << endl;*/
		}
	}
}
