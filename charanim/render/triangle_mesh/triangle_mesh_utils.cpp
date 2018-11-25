#include <render/triangle_mesh/triangle_mesh_utils.hpp>

material::material
(
	const std::string& _id,
	const std::string& _txt_name,
	uint _txt_id,
	const glm::vec3& amb,
	const glm::vec3& dif,
	const glm::vec3& spec,
	float ns, float ni,float D,
	int ill
)
{
	id = _id;
	txt_name = _txt_name;
	txt_id = _txt_id;

	Ns = ns;
	Ni = ni;
	d =  D;
	illum = ill;
	Ka = glm::vec4(amb.x, amb.y, amb.z, D);
	Kd = glm::vec4(dif.x, dif.y, dif.z, D);
	Ks = glm::vec4(spec.x, spec.y, spec.z, D);
}

material::material(const material& m) {
	id = m.id;
	txt_name = m.txt_name;
	txt_id = m.txt_id;

	Ns = m.Ns;
	Ni = m.Ni;
	d =  m.d;
	illum = m.illum;
	Ka = m.Ka;
	Kd = m.Kd;
	Ks = m.Ks;
}
