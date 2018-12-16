#include <anim/charanim.hpp>

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
using namespace physim::particles;

// charanim includes
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>
#include <render/shader/shader_helper.hpp>
#include <anim/vec_helper.hpp>

namespace charanim {

	bool load_shaders() {
		bool r;

		r = flat_shader.init("../../charanim/shaders", "flat.vert", "flat.frag");
		if (not r) { return false; }

		r = material_shader.init("../../charanim/shaders", "materials.vert", "materials.frag");
		if (not r) { return false; }

		r = texture_shader.init("../../charanim/shaders", "textures.vert", "textures.frag");
		if (not r) { return false; }

		flat_shader.bind();
		flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));
		flat_shader.release();

		return true;
	}

	bool load_sphere() {
		sphere = new rendered_triangle_mesh();

		OBJ_reader obj;
		obj.load_object("../../models", "sphere.obj", *sphere);
		sphere->make_buffers();

		return true;
	}

} // -- namespace charanim
