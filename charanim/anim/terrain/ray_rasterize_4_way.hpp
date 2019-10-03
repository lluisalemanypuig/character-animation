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

#pragma once

// C++ includes
#include <iostream>
#include <cmath>
using namespace std;

// Custom includes
#include <anim/terrain/ray_rasterize.hpp>
#include <anim/definitions.hpp>

namespace charanim {

/// Class to compute the rasterization of a ray from a starting to an ending
/// point using the method 4-way step with midpoint.
class ray_rasterize_4_way : public ray_rasterize {
	private:
		latticePoint ini, fin;
		int ix, iy;  // step advance
		
		bool first, last; // reached last cell
		double x, y; // current cell
		
		// slope of the line between ini and fin
		double slope, inv_slope;
	
	private:
		void advance_pos_slope();
		void advance_neg_slope();
		
		void retreat_pos_slope();
		void retreat_neg_slope();
		
	public:
		
		// The documentation for every function can be found in
		// the header file of the class ray_rasterize (ray_rasterize.hpp)
		
		ray_rasterize_4_way();
		ray_rasterize_4_way(const ray_rasterize_4_way& r);
		~ray_rasterize_4_way();
		
		ray_rasterize_4_way& operator= (const ray_rasterize_4_way& r);
		
		void init(const latticePoint& ini, const latticePoint& fin);

		ray_rasterize_4_way *clone() const;
		
		void current_cell(latticePoint& current) const;
		
		void advance();
		void retreat();
		
		void place_at(const latticePoint& p);
		
		bool is_first() const;
		bool is_last() const;
		
		bool on_ray(const latticePoint& c) const;
};

} // -- namespace charanim
