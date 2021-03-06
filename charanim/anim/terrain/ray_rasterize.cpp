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

#include "ray_rasterize.hpp"

namespace charanim {

// Public

ray_rasterize::ray_rasterize() { }
ray_rasterize::~ray_rasterize() { }

void ray_rasterize::get_advance(latticePoint& next) {
	current_cell(next);
	advance();
}

void ray_rasterize::get_retreat(latticePoint& next) {
	current_cell(next);
	retreat();
}

} // -- namespace charanim
