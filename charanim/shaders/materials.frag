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

uniform Light light;
uniform vec3 view_pos;
uniform Material material[4];

in vec3 frag_pos;
in vec3 frag_normal;
flat in int mat_id;

out vec4 frag_color;

void main() {
	vec3 result = vec3(0,0,0);
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

		result = ambient + diffuse + specular;
	}
	frag_color = vec4(result, 1.0);
}

