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

#include <render/geometry/rgeometry.hpp>

// render includes
#include <render/include_gl.hpp>

rgeom::rgeom() {
	render = true;
	r = 1.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;

	t = rendered_geometry_type::none;
	_model = nullptr;
}

rgeom::~rgeom() {
	clear();
}

void rgeom::clear() {
	if (_model != nullptr) {
		_model = nullptr;
	}
}

// SETTERS

void rgeom::set_render(bool r) { render = r; }
void rgeom::set_color(float _r, float _g, float _b, float _a) {
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}
void rgeom::set_model(const std::shared_ptr<rendered_triangle_mesh>& _m) {
	_model = _m;
}

// GETTERS

bool rgeom::should_render() const { return render; }

rendered_geometry_type rgeom::get_type() const {
	return t;
}

std::shared_ptr<rendered_triangle_mesh> rgeom::get_model() {
	return _model;
}

const std::shared_ptr<rendered_triangle_mesh> rgeom::get_model() const {
	return _model;
}

float rgeom::red() const { return r; }
float rgeom::green() const { return g; }
float rgeom::blue() const { return b; }
float rgeom::alpha() const { return a; }

// OTHERS

void rgeom::translate_object() const {
	// method left blank since this is abstract geometry
}

void rgeom::draw() const {
	if (_model != nullptr) {
		if (_model->uses_buffers()) {
			_model->render();
		}
		else {
			glPushMatrix();
				translate_object();
				_model->render();
			glPopMatrix();
		}
	}
	else {
		draw_geometry();
	}
}
