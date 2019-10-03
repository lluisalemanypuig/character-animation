/*********************************************************************
 * charanim - Character Animation Project
 * Copyright (C) 2018 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
