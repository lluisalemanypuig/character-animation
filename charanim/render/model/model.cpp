#include <render/model/model.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/glm.hpp>

// PRIVATE

// PROTECTED

glm::vec3 model::triangle_normal(int t) const {
	assert(t != -1);

	int T = 3*t;
	const glm::vec3& v1 = vertices[triangles[T    ]];
	const glm::vec3& v2 = vertices[triangles[T + 1]];
	const glm::vec3& v3 = vertices[triangles[T + 2]];

	glm::vec3 u = v2 - v1;
	glm::vec3 v = v3 - v1;
	glm::vec3 w = glm::normalize(glm::cross(u,v));

	return w;
}

// PUBLIC

model::model() {
	mesh_name = "anonymous";
}

model::model(const model& m) {
	mesh_name = m.mesh_name;
	vertices = m.vertices;
	normals = m.normals;
	triangles = m.triangles;
	normal_idxs = m.normal_idxs;
}

model::~model() {
	clear();
}

// SETTERS

void model::set_name(const string& name) {
	mesh_name = name;
}

void model::set_vertices(const vector<glm::vec3>& verts) {
	vertices = verts;
}

void model::set_normals(const vector<glm::vec3>& nrmls) {
	normals = nrmls;
}

void model::set_triangles(const vector<int>& tris) {
	triangles = tris;
}

void model::set_normal_idxs(const vector<int>& nrmls_idxs) {
	normal_idxs = nrmls_idxs;
}

// MODIFIERS

mesh_state model::state(const mesh_state& ignore) const {
	if (((ignore & mesh_state::no_vertices) == 0) and vertices.size() == 0) {
		cerr << "mesh::is_valid: Error" << endl;
		cerr << "    Vertices not found in mesh '" << mesh_name << "'" << endl;
		return mesh_state::no_vertices;
	}
	if (((ignore & mesh_state::no_triangles) == 0) and triangles.size() == 0) {
		cerr << "mesh::is_valid: Error" << endl;
		cerr << "    No triangles found in mesh '" << mesh_name << "'" << endl;
		return mesh_state::no_triangles;
	}
	if (((ignore & mesh_state::no_normals) == 0) and normals.size() == 0) {
		cerr << "mesh::is_valid: Error" << endl;
		cerr << "    No normal vectors found in mesh '" << mesh_name << "'" << endl;
		return mesh_state::no_normals;
	}

	if ((ignore & mesh_state::vertex_idx_ob) == 0 and (ignore & mesh_state::normal_idx_ob) != 0) {
		return mesh_state::correct;
	}

	// check that indexes are correct.
	for (size_t t = 0; t < triangles.size(); ++t) {
		if ((ignore & mesh_state::vertex_idx_ob) == 0) {
			if (triangles[t] != -1 and triangles[t] > vertices.size()) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Face " << t/3 << " has " << t%3 << "-th "
					 << "vertex index (" << triangles[t]
					 << ") out of bounds." << endl;
				return mesh_state::vertex_idx_ob;
			}
		}

		if ((ignore & mesh_state::normal_idx_ob) == 0) {
			if (normal_idxs[t] != -1 and normal_idxs[t] > normals.size()) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Triangle " << t/3 << " has " << t%3 << "-th "
					 << "normal index (" << normal_idxs[t]
					 << ") out of bounds." << endl;
				return mesh_state::normal_idx_ob;
			}
		}
	}

	return mesh_state::correct;
}

const std::vector<glm::vec3>& model::get_vertices() const {
	return vertices;
}

void model::make_box(box& b) const {
	glm::vec3 min = vertices[0];
	glm::vec3 max = vertices[0];
	for (const glm::vec3& v : vertices) {
		min.x = std::min(min.x, v.x);
		min.y = std::min(min.y, v.y);
		min.z = std::min(min.z, v.z);

		max.x = std::max(max.x, v.x);
		max.y = std::max(max.y, v.y);
		max.z = std::max(max.z, v.z);
	}

	b.set_min_max(min, max);
}

void model::make_normals_flat() {
	normals.clear();
	normal_idxs.clear();

	for (size_t t = 0; t < triangles.size(); t += 3) {
		glm::vec3 n = triangle_normal(t/3);

		int idx = normals.size();
		normals.push_back(n);
		normals.push_back(n);
		normals.push_back(n);
		normal_idxs[t    ] = idx;
		normal_idxs[t + 1] = idx + 1;
		normal_idxs[t + 2] = idx + 2;
	}
}

void model::make_normals_smooth() {
	// compute to what faces each
	// vertex is adjacent to
	vector<vector<int> > tris_per_vertex(vertices.size());
	for (size_t t = 0; t < triangles.size(); t += 3) {
		tris_per_vertex[ triangles[t    ] ].push_back(t/3);
		tris_per_vertex[ triangles[t + 1] ].push_back(t/3);
		tris_per_vertex[ triangles[t + 2] ].push_back(t/3);
	}

	// temporarily use the normals vector as a
	// 'triangle normals' vector
	normals.resize(triangles.size()/3);
	// normal_computed[t] = true <-> normal for
	// triangle t was computed
	vector<bool> normal_computed(triangles.size()/3, false);

	// Firstly, compute the smoothed normals for each vertex,
	// and store them in a separate vector.
	vector<glm::vec3> smoothed_normals(vertices.size());

	// compute normals for the vertices that make
	// up the faces marked with 'smooth = true'
	for (size_t v = 0; v < vertices.size(); ++v) {
		smoothed_normals[v] = glm::vec3(0.0f,0.0f,0.0f);
		glm::vec3& normal = smoothed_normals[v];

		// add to 'normal' the normal of those triangles
		// that share vertex V.
		for (int t : tris_per_vertex[v]) {
			// if the normal for triangle 't' was not
			// computed, do it now and store it.
			if (not normal_computed[t]) {
				normals[t] = triangle_normal(t);
				normal_computed[t] = true;
			}
			normal += normals[t];
		}

		// average the normal
		normal /= tris_per_vertex[v].size();
		// normalise the normal
		normal = glm::normalize(normal);

		// since 'normal' is a reference to a value of
		// 'smoothed_normals' we do not need to
		// assign this normal to the vector
	}

	// Secondly, replace normal indices
	for (size_t t = 0; t < triangles.size(); ++t) {
		normal_idxs[t] = triangles[t];
	}

	// Finally, store the smoothed normals
	normals = smoothed_normals;
}

void model::scale_to_unit() {
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 m(1e10, 1e10, 1e10);
	glm::vec3 M(-1e10, -1e10, -1e10);

	for (size_t i = 0; i < vertices.size(); ++i) {
		center += vertices[i];
		m = glm::min(m, vertices[i]);
		M = glm::max(M, vertices[i]);
	}
	center /= vertices.size();

	float largestSize = std::max(M.x - m.x, std::max(M.y - m.y, M.z - m.z));

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i] = (vertices[i] - center)/largestSize;
	}
}

void model::clear() {
	vertices.clear();
	triangles.clear();
	normals.clear();
	normal_idxs.clear();
}

// OTHERS

void model::display_mesh_info() const {
	cout << "Mesh '" << mesh_name << "' information: " << endl;
	cout << "    # Vertices= " << vertices.size() << endl;
	cout << "    # Triangles= " << triangles.size() << endl;
	cout << "    # Normals= " << normals.size() << endl;
	cout << "    # Normal indices= " << normal_idxs.size() << endl;
}

