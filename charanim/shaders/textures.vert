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

#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 _tex_coord;
layout (location = 3) in int _mat_id;
layout (location = 4) in int _tex_id;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat3 normal_matrix;

out vec3 frag_normal;
out vec3 frag_pos;
out vec2 tex_coord;
flat out int mat_id;
flat out int tex_id;

void main() {
	gl_Position = projection*modelview*vec4(position, 1.0);
	frag_normal = normal_matrix*normal;
	frag_pos = vec3(modelview*vec4(position, 1.0));
	tex_coord = _tex_coord;
	mat_id = _mat_id;
	tex_id = _tex_id;
}
