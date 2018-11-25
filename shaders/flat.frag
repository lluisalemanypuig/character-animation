#version 330 core

uniform bool wireframe;
uniform vec4 colour;

in vec3 frag_normal;

out vec4 frag_color;

void main() {
	if (wireframe) {
		frag_color = colour;
	}
	else {
		frag_color = colour*normalize(frag_normal).z;
	}
}
