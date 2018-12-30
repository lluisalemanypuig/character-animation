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
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform sampler2D tex8;
uniform sampler2D tex9;
uniform sampler2D tex10;
uniform sampler2D tex11;
uniform sampler2D tex12;
uniform sampler2D tex13;
uniform sampler2D tex14;
uniform sampler2D tex15;
uniform sampler2D tex16;
uniform sampler2D tex17;
uniform sampler2D tex18;
uniform sampler2D tex19;
uniform sampler2D tex20;
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
	case  5: col = texture2D( tex5, tex_coord)*col; break;
	case  6: col = texture2D( tex6, tex_coord)*col; break;
	case  7: col = texture2D( tex7, tex_coord)*col; break;
	case  8: col = texture2D( tex8, tex_coord)*col; break;
	case  9: col = texture2D( tex9, tex_coord)*col; break;
	case 10: col = texture2D(tex10, tex_coord)*col; break;
	case 11: col = texture2D(tex11, tex_coord)*col; break;
	case 12: col = texture2D(tex12, tex_coord)*col; break;
	case 13: col = texture2D(tex13, tex_coord)*col; break;
	case 14: col = texture2D(tex14, tex_coord)*col; break;
	case 15: col = texture2D(tex15, tex_coord)*col; break;
	case 16: col = texture2D(tex16, tex_coord)*col; break;
	case 17: col = texture2D(tex17, tex_coord)*col; break;
	case 18: col = texture2D(tex18, tex_coord)*col; break;
	case 19: col = texture2D(tex19, tex_coord)*col; break;
	case 20: col = texture2D(tex20, tex_coord)*col; break;
	default:
		// do nothing
		col = col;
	}

	frag_color = col;
}

