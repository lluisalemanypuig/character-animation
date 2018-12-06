#pragma once

// C++ includes
#include <utility>

// physim includes
#include <physim/math/vec2.hpp>

namespace charanim {

	typedef physim::math::vec2 point2D;
	typedef std::pair<point2D,point2D> segment;

	class latticePoint : std::pair<int,int> {
		public:
			latticePoint() : std::pair<int,int>() {}
			latticePoint(int x, int y) : std::pair<int,int>(x,y) {}
			~latticePoint() {}
			int x() const { return first; }
			int& x() { return first; }
			int y() const { return second; }
			int& y() { return second; }
	};


} // -- namespace charanim
