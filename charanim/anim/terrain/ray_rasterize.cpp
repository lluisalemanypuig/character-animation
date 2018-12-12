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
