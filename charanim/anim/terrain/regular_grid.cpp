#include <anim/terrain/regular_grid.hpp>

// C++ includes
using namespace std;

namespace charanim {

// PUBLIC

regular_grid::regular_grid() : path_finder() {
	resX = resY = 0;
}

regular_grid::~regular_grid() {

}

// MODIFIERS

void regular_grid::init(size_t cellsx, size_t cellsy) {
	resX = cellsx;
	resY = cellsy;
	occupied_cells = vector<bool>(resX*resY, false);
}

void regular_grid::init(const std::vector<segment>& segs) {
	// rasterise each segment into the grid
}

void regular_grid::clear() {
	resX = resY = 0;
	occupied_cells.clear();
}

// GETTERS

void regular_grid::find_path(
	const point2D& source, const point2D& sink,
	std::vector<point2D>& segs
)
{

}

path_finder_type regular_grid::get_type() const {
	return path_finder_type::regular_grid;
}

} // -- namespace charanim
