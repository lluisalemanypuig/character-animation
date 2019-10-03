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
#include <utility>

// physim includes
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>

namespace charanim {

	typedef physim::math::vec2 vec2;
	typedef physim::math::vec3 vec3;
	typedef std::pair<vec2,vec2> segment;

	class latticePoint : std::pair<int,int> {
		public:
			latticePoint() : std::pair<int,int>() {}
			latticePoint(int x, int y) : std::pair<int,int>(x,y) {}
			~latticePoint() {}
			inline int x() const { return first; }
			inline int& x() { return first; }
			inline int y() const { return second; }
			inline int& y() { return second; }
			inline bool operator< (const latticePoint& lp) const {
				if (x() == lp.x()) {
					return y() < lp.y();
				}
				return x() < lp.x();
			}
			inline bool operator<= (const latticePoint& lp) const {
				if (x() == lp.x()) {
					return y() <= lp.y();
				}
				return x() <= lp.x();
			}
			inline bool operator== (const latticePoint& lp) const {
				return (x() == lp.x()) and (y() == lp.y());
			}
			inline bool operator!= (const latticePoint& lp) const {
				return (x() != lp.x()) or (y() != lp.y());
			}
	};


} // -- namespace charanim
