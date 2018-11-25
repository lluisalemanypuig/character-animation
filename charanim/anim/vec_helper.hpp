#pragma once

#define out_vec2(v) (v).x << "," << (v).y
#define out_vec3(v) out_vec2(v) << "," << (v).z

glm::vec3 to_gvec3(const physim::math::vec3& v) {
	return glm::vec3(v.x, v.y, v.z);
}
