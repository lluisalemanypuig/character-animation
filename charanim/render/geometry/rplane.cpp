#include <render/geometry/rplane.hpp>

// render includes
#include <render/include_gl.hpp>

rplane::rplane() : rgeom() {
	t = rendered_geometry_type::plane;
}
rplane::~rplane() {}

// SETTERS

void rplane::set_points
(const glm::vec3& p1, const glm::vec3& p2,
 const glm::vec3& p3, const glm::vec3& p4)
{
	_p1 = p1; _p2 = p2; _p3 = p3; _p4 = p4;
}

// GETTERS

const glm::vec3& rplane::p1() const { return _p1; }
const glm::vec3& rplane::p2() const { return _p2; }
const glm::vec3& rplane::p3() const { return _p3; }
const glm::vec3& rplane::p4() const { return _p4; }

// OTHERS

void rplane::draw_geometry() const {
	glDisable(GL_LIGHTING);
	glColor4f(r,g,b,a);
	glBegin(GL_TRIANGLES);
		glVertex3f(_p1.x, _p1.y, _p1.z);
		glVertex3f(_p2.x, _p2.y, _p2.z);
		glVertex3f(_p3.x, _p3.y, _p3.z);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f(_p1.x, _p1.y, _p1.z);
		glVertex3f(_p3.x, _p3.y, _p3.z);
		glVertex3f(_p4.x, _p4.y, _p4.z);
	glEnd();
	glEnable(GL_LIGHTING);
}


void rplane::make_model_matrix(glm::mat4& ) const {}
void rplane::make_box(box& b) const {
	b.set_min_max(_p1, _p1);
	b.enlarge_box(_p2);
	b.enlarge_box(_p3);
	b.enlarge_box(_p4);
}
