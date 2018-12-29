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

uniform Light light;
uniform vec3 view_pos;
uniform Material material[4];

in vec3 frag_pos;
in vec3 frag_normal;
flat in int mat_id;

out vec4 frag_color;

void main() {
	vec4 result = vec4(0,0,0,0);
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

		result = ambient + diffuse + specular;
	}
	frag_color = result;
}

