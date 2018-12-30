#version 330 core

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
struct Light {
	vec3 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

uniform vec3 view_pos;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform Light light;
uniform Material material[32];

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
		vec4 ambient = light.ambient * material[mat_id].ambient;

		// diffuse
		vec3 norm = normalize(frag_normal);
		vec3 light_dir = normalize(light.position - frag_pos);
		float diff = max(dot(norm, light_dir), 0.0);
		vec4 diffuse = light.diffuse*(diff*material[mat_id].diffuse);

		// specular
		vec3 view_dir = normalize(view_pos - frag_pos);
		vec3 refl_dir = reflect(-light_dir, norm);
		float spec = pow(max(dot(view_dir, refl_dir), 0.0), material[mat_id].shininess);
		vec4 specular = light.specular*(spec*material[mat_id].specular);

		col = ambient + diffuse + specular;
	}
	switch (tex_id) {
	case  1: col = texture2D( tex1, tex_coord)*col; break;
	case  2: col = texture2D( tex2, tex_coord)*col; break;
	case  3: col = texture2D( tex3, tex_coord)*col; break;
	case  4: col = texture2D( tex4, tex_coord)*col; break;
	default:
		// do nothing
		col = col;
	}

	frag_color = col;
}

