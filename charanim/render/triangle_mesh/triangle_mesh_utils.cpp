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

#include <render/triangle_mesh/triangle_mesh_utils.hpp>

#include <iostream>
using namespace std;

material::material() {}

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
