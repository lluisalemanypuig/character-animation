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
