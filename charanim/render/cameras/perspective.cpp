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

#include <render/cameras/perspective.hpp>

// C++ includes
#include <cmath>

static inline float deg_to_rad(float d) { return d*M_PI/180.0f; }
static inline float rad_to_deg(float r) { return r*180.0f/M_PI; }

perspective::perspective() {
	zN = -1; zF = 1;
}
perspective::perspective(const perspective& p) : observer(p) {
	raw = p.raw;
	d = p.d;
	alpha = p.alpha;
	initial_alpha = p.initial_alpha;
}

perspective::~perspective() {}

void perspective::make_Zs(const box& b) {
	float R = b.get_diag_length()/2.0f;
	d = 2.0f*R;
	zN = d - R;
	zF = d + R;
}

void perspective::init_camera(const box& b) {
	float R = b.get_diag_length()/2.0f;
	make_Zs(b);
	alpha = std::asin(R/d);
	initial_alpha = alpha;
}

void perspective::adapt_camera_to_viewport(int w, int h) {
	raw = ((float)w)/((float)h);
	if (raw < 1) {
		alpha = atan(tan(initial_alpha)/raw);
	}
	else {
		alpha = initial_alpha;
	}
}

void perspective::set_zoom(float a) {alpha = deg_to_rad(a/2);}
void perspective::zoom(float inc) {
	float beta = rad_to_deg(alpha);
	beta += inc;
	if (beta <= 0) {
		beta = 0.1;
	}
	if (beta >= 90) {
		beta = 89.9;
	}
	alpha = deg_to_rad(beta);
}

void perspective::reset_zoom() {alpha = initial_alpha;}
float perspective::get_zoom() const {return get_FOV();}

float perspective::getD() const {return d;}
float perspective::getRAw() const {return raw;}
float perspective::get_FOV() const {return rad_to_deg(2.0f*alpha);}
float perspective::get_window_height() const {return 2.0f*zN*std::tan(alpha);}
float perspective::get_window_width() const {return raw*get_window_height();}




