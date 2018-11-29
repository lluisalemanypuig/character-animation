#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 view_pos;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform Light light;
uniform Material material[4];

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 tex_coord;
flat in int mat_id;
flat in int tex_id;

out vec4 frag_color;

void main() {
	vec4 col = vec4(0,0,0,0);

	if (mat_id >= 0) {
		// ambient
		vec3 ambient = light.ambient * material[mat_id].ambient;

		// diffuse
		vec3 norm = normalize(frag_normal);
		vec3 light_dir = normalize(light.position - frag_pos);
		float diff = max(dot(norm, light_dir), 0.0);
		vec3 diffuse = light.diffuse*(diff*material[mat_id].diffuse);

		// specular
		vec3 view_dir = normalize(view_pos - frag_pos);
		vec3 refl_dir = reflect(-light_dir, norm);
		float spec = pow(max(dot(view_dir, refl_dir), 0.0), material[mat_id].shininess);
		vec3 specular = light.specular*(spec*material[mat_id].specular);

		col = vec4(ambient + diffuse + specular, 1.0);
	}
	if (tex_id == 1) {
		col = texture2D(tex1, tex_coord)*col;
	}
	else if (tex_id == 2) {
		col = texture2D(tex2, tex_coord)*col;
	}
	else if (tex_id == 3) {
		col = texture2D(tex3, tex_coord)*col;
	}
	else if (tex_id == 4) {
		col = texture2D(tex4, tex_coord)*col;
	}

	frag_color = col;
}

