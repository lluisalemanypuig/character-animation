#pragma once

#include <physim/math/vec3.hpp>
#include <glm/vec3.hpp>

typedef physim::math::vec3 pm_vec3;
typedef glm::vec3 glm_vec3;

#define out_vec2(v) (v).x << "," << (v).y
#define out_vec3(v) out_vec2(v) << "," << (v).z

static inline
glm::vec3 to_gvec3(const physim::math::vec3& v) {
	return glm::vec3(v.x, v.y, v.z);
}
