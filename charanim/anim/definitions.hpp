#pragma once

// C++ includes
#include <utility>

// physim includes
#include <physim/math/vec2.hpp>

namespace charanim {

	typedef physim::math::vec2 vec2;
	typedef std::pair<vec2,vec2> segment;

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
