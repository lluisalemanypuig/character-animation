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

#include <anim/terrain/ray_rasterize_4_way.hpp>

// Non-class private

const static double _r_90 = M_PI/2.0;
const static double _r_180 = M_PI;
const static double _r_270 = 3.0*M_PI/2.0;
const static double _r_360 = 2.0*M_PI;

namespace charanim {

// alpha: rad
void locate_point(const latticePoint& ini, size_t d, double phi, latticePoint& fin) {
	double alpha;
	
	if (0 <= phi and phi < _r_90) alpha = phi;
	else if (_r_90 <= phi and phi < _r_180) alpha = _r_180 - phi;
	else if (_r_180 <= phi and phi < _r_270) alpha = phi - _r_180;
	else alpha = _r_360 - phi;
	
	double dx = d*cos(alpha);
	double dy = d*sin(alpha);
	
	if (0 <= phi and phi <= _r_90) { fin.x() = ini.x() + dx; fin.y() = ini.y() + dy; }
	else if (_r_90 < phi and phi <= _r_180) { fin.x() = ini.x() - dx; fin.y() = ini.y() + dy; }
	else if (_r_180 < phi and phi <= _r_270) { fin.x() = ini.x() - dx; fin.y() = ini.y() - dy; }
	else { fin.x() = ini.x() + dx; fin.y() = ini.y() - dy; }
}

/// Private

void ray_rasterize_4_way::advance_pos_slope() {
	// slope > 0
	if (slope < 1.0) {
		if (int(x) == fin.x()) last = true;
		x += ix;
		y += iy*slope;
	}
	else {
		if (int(y) == fin.y()) last = true;
		x += ix*inv_slope;
		y += iy;
	}
}

void ray_rasterize_4_way::advance_neg_slope() {
	// slope < 0
	if (abs(slope) < 1.0) {
		if (int(x) == fin.x()) last = true;
		x += ix;
		y -= iy*slope;
	}
	else {
		if (int(y) == fin.y()) last = true;
		x -= ix*inv_slope;
		y += iy;
	}
}

void ray_rasterize_4_way::retreat_pos_slope() {
	// slope > 0
	if (slope < 1.0) {
		x -= ix;
		y -= iy*slope;
		if (int(x) == ini.x()) first = true;
	}
	else {
		x -= ix*inv_slope;
		y -= iy;
		if (int(y) == ini.y()) first = true;
	}
}

void ray_rasterize_4_way::retreat_neg_slope() {
	// slope < 0
	if (abs(slope) < 1.0) {
		x -= ix;
		y += iy*slope;
		if (int(x) == ini.x()) first = true;
	}
	else {
		x += ix*inv_slope;
		y -= iy;
		if (int(y) == ini.y()) first = true;
	}
}

/// Public

ray_rasterize_4_way::ray_rasterize_4_way() : ray_rasterize() {
	ini.x() = ini.y() = 0;
	fin.x() = fin.y() = 0;
	ix = iy = 0;
	x = y = 0.0;
	slope = inv_slope = 0.0;
	first = true;
	last = false;
}

ray_rasterize_4_way::ray_rasterize_4_way(const ray_rasterize_4_way& r) : ray_rasterize() {
	*this = r;
}

ray_rasterize_4_way::~ray_rasterize_4_way() { }

void ray_rasterize_4_way::init(const latticePoint& _ini, const latticePoint& _fin) {
	ini.x() = _ini.x(); fin.x() = _fin.x();
	ini.y() = _ini.y(); fin.y() = _fin.y();
	
	int dx = _fin.x() - _ini.x();
	int dy = _fin.y() - _ini.y();
	
	if (dx != 0) {
		slope = (1.0*dy)/dx;
		if (slope != 0.0) inv_slope = 1.0/slope;
	}
	else {
		slope = 99999.0; // infinite (in this case, this is not used)
		inv_slope = 0.0;
	}
	
	if (dx > 0) ix = 1;
	else ix = -1;
	
	if (dy > 0) iy = 1;
	else iy = -1;
	
	x = ini.x();
	y = ini.y();
	
	first = true;
	last = false;
}

ray_rasterize_4_way& ray_rasterize_4_way::operator= (const ray_rasterize_4_way& r) {
	ini = r.ini;
	fin = r.fin;
	ix = r.ix;
	iy = r.iy;
	
	x = r.x;
	y = r.y;
	
	slope = r.slope;
	inv_slope = r.inv_slope;
	
	first = r.first;
	last = r.last;
	return *this;
}

ray_rasterize_4_way *ray_rasterize_4_way::clone() const {
	return new ray_rasterize_4_way(*this);
}

void ray_rasterize_4_way::current_cell(latticePoint& current) const {
	if (ini.x() == fin.x() or ini.y() == fin.y()) {
		current.x() = x;
		current.y() = y;
	}
	else if (abs(slope) < 1.0) {
		current.x() = x;
		
		current.y() = y;
		if (y >= 0) current.y() += 0.5;
		else current.y() -= 0.5;
	}
	else {
		current.x() = x;
		if (x >= 0) current.x() += 0.5;
		else current.x() -= 0.5;
		
		current.y() = y;
	}
}

void ray_rasterize_4_way::advance() {
	if (first) first = false;
	
	if (ini.x() == fin.x()) {
		// vertical ray (m = infty)
		if (uint(y) == fin.y()) last = true;
		y += iy;
	}
	else if (slope >= 0.0) advance_pos_slope();
	else advance_neg_slope();
}

void ray_rasterize_4_way::retreat() {
	if (last) last = false;
	
	if (ini.x() == fin.x()) {
		// vertical ray (m = infty)
		if (uint(y) == ini.y()) first = true;
		y -= iy;
	}
	else if (slope >= 0.0) retreat_pos_slope();
	else retreat_neg_slope();
}

void ray_rasterize_4_way::place_at(const latticePoint& p) {
	// Cell on a vertical or horizontal ray
	if (ini.x() == fin.x() or ini.y() == fin.y()) { x = p.x(); y = p.y(); return; }
	
	int k;
	
	if (slope >= 0.0) {
		if (slope < 1.0) {
			k = (p.x() - ini.x())/ix;
			x = ini.x() + k*ix;
			y = ini.y() + k*iy*slope;
		}
		else {
			k = (p.y() - ini.y())/iy;
			x = ini.x() + k*ix*inv_slope;
			y = ini.y() + k*iy;
		}
	}
	else {
		if (abs(slope) < 1.0) {
			k = (p.x() - ini.x())/ix;
			x = ini.x() + k*ix;
			y = ini.y() - k*iy*slope;
		}
		else {
			k = (p.y() - ini.y())/iy;
			x = ini.x() - k*ix*inv_slope;
			y = ini.y() + k*iy;
		}
	}
	
	if (x == ini.x() and y == ini.y()) { first = true; last = false; }
	else if (x == fin.x() and y == fin.y()) { first = false; last = true; }
	else { first = last = false; }
}

bool ray_rasterize_4_way::is_first() const { return first; }
bool ray_rasterize_4_way::is_last() const { return last; }

#define is_on_ray ((expr_x == c.x()) and (expr_y == c.y()))
bool ray_rasterize_4_way::on_ray(const latticePoint& c) const {
	// Base cases
	
	// Cell on a vertical ray
	if (ini.x() == fin.x()) {
		if (c.x() == ini.x()) return true;
		return false;
	}
	
	// Cell on a horizontal ray
	if (ini.y() == fin.y()) {
		if (c.y() == ini.y()) return true;
		return false;
	}
	
	int k, expr_x, expr_y;
	
	if (slope >= 0.0) {
		if (slope < 1.0) {
			k = (c.x() - ini.x())/ix;
			
			expr_x = ini.x() + k*ix;
			expr_y = ini.y() + k*iy*slope;
			
			if (expr_y >= 0) expr_y += 0.5;
			else expr_y -= 0.5;
			
			return is_on_ray;
		}
		
		k = (c.y() - ini.y())/iy;
		
		expr_x = ini.x() + k*ix*inv_slope;
		expr_y = ini.y() + k*iy;
		
		if (expr_x >= 0) expr_x += 0.5;
		else expr_x -= 0.5;
		
		return is_on_ray;
	}
	
	if (abs(slope) < 1.0) {
		k = (c.x() - ini.x())/ix;
		
		expr_x = ini.x() + k*ix;
		expr_y = ini.y() - k*iy*slope;
		
		if (expr_y >= 0) expr_y += 0.5;
		else expr_y -= 0.5;
		
		return is_on_ray;
	}
	k = (c.y() - ini.y())/iy;
	
	expr_x = ini.x() - k*ix*inv_slope;
	expr_y = ini.y() + k*iy;
	
	if (expr_x >= 0) expr_x += 0.5;
	else expr_x -= 0.5;
	
	return is_on_ray;
}

} // -- namespace charanim
