#include <anim/terrain/regular_grid/regular_grid.hpp>

// C includes
#include <stdlib.h>

// C++ includes
using namespace std;

// charanim includes
#include <anim/terrain/regular_grid/ray_rasterize_4_way.hpp>

namespace charanim {

#define global(x,y, g) g = y*resX + x
#define  local(g, x,y) x = g%resX; y = g/resX

#define l1(x1,y1, x2,y2) std::abs(x2 - x1) + std::abs(y2 - y1)
#define l2(x1,y1, x2,y2) std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))

// PUBLIC

regular_grid::regular_grid() : path_finder() {
	resX = resY = 0;
	grid_cells = nullptr;
}

regular_grid::~regular_grid() {
	clear();
}

// MODIFIERS

void regular_grid::init(size_t cellsx, size_t cellsy, float dx, float dy) {
	resX = cellsx;
	resY = cellsy;
	dimX = dx;
	dimY = dy;
	grid_cells = static_cast<float *>(malloc(resX*resY*sizeof(float)));
	for (size_t i = 0; i < resX*resY; ++i) {
		grid_cells[i] = numeric_limits<float>::max();
	}
}

void regular_grid::init(const std::vector<segment>& segs) {
	float lenX = dimX/resX;
	float lenY = dimY/resY;

	// rasterise each segment into the grid
	ray_rasterize_4_way ray;
	for (const segment& s : segs) {
		const point2D& p = s.first;
		int px = static_cast<int>(p.x/lenX);
		int py = static_cast<int>(p.y/lenY);
		const point2D& q = s.second;
		int qx = static_cast<int>(q.x/lenX);
		int qy = static_cast<int>(q.y/lenY);

		latticePoint lP(px,py);
		latticePoint lQ(qx,qy);

		// rasterise the line and set '0' to its grid cells
		ray.init(lP, lQ);
		latticePoint grid_cell;
		size_t g_idx;
		while (not ray.is_last()) {
			ray.get_advance(grid_cell);
			global(grid_cell.x(), grid_cell.y(), g_idx);
			grid_cells[g_idx] = 0.0f;
		}
	}

	for (size_t i = 0; i < resY; ++i) {
		for (size_t j = 0; j < resX; ++j) {
			size_t gbl_idx;
			global(j,i, gbl_idx);
			if (grid_cells[gbl_idx] == numeric_limits<float>::max()) {
				cout << "-";
			}
			else {
				cout << "0";
			}
		}
		cout << endl;
	}
}

void regular_grid::clear() {
	resX = resY = 0;
	if (grid_cells != nullptr) {
		free(grid_cells);
		grid_cells = nullptr;
	}
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
