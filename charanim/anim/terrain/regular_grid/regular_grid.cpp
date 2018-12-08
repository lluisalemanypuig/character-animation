#include <anim/terrain/regular_grid/regular_grid.hpp>

// C includes
#include <stdlib.h>

// C++ includes
#include <iomanip>
using namespace std;

// physim includes
#include <physim/math/vec2.hpp>

// charanim includes
#include <anim/terrain/regular_grid/ray_rasterize_4_way.hpp>

namespace charanim {

#define global(x,y, g) g = y*resX + x
#define get_global(x,y) y*resX + x
#define  local(g, x,y) x = g%resX; y = g/resX

#define in_box(s,t, p)													\
	(((std::min(s.x, t.x) <= p.x) and (p.x <= std::max(s.x,t.x))) and	\
	 ((std::min(s.y, t.y) <= p.y) and (p.y <= std::max(s.y,t.y))))

#define l2(x1,y1, x2,y2) std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))

// PRIVATE

// PUBLIC

regular_grid::regular_grid() : path_finder() {
	resX = resY = 0;
	dimX = dimY = 0.0f;
	max_dist = 0.0f;
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
	max_dist = 0.0f;
	grid_cells = static_cast<float *>(malloc(resX*resY*sizeof(float)));
	for (size_t i = 0; i < resX*resY; ++i) {
		grid_cells[i] = numeric_limits<float>::max();
	}
}

void regular_grid::init(const std::vector<segment>& segs) {
	for (const segment& s : segs) {
		rasterise_segment(s);
		expand_function_distance(s);
	}
}

void regular_grid::clear() {
	resX = resY = 0;
	dimX = dimY = 0.0f;
	max_dist = 0.0f;
	if (grid_cells != nullptr) {
		free(grid_cells);
		grid_cells = nullptr;
	}
}

void regular_grid::rasterise_segment(const segment& seg) {
	const float lenX = dimX/resX;
	const float lenY = dimY/resY;

	/* ------------------------------- */
	/* rasterise segment into the grid */
	ray_rasterize_4_way ray;

	const vec2& s = seg.first;
	int px = static_cast<int>(s.x/lenX);
	int py = static_cast<int>(s.y/lenY);
	const vec2& t = seg.second;
	int qx = static_cast<int>(t.x/lenX);
	int qy = static_cast<int>(t.y/lenY);

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

void regular_grid::expand_function_distance(const segment& seg) {
	const float lenX = dimX/resX;
	const float lenY = dimY/resY;
	const vec2& s = seg.first;
	const vec2& t = seg.second;

	/* compute distance of every other cell to the segment */
	vec2 u = t - s;
	float uu = physim::math::dot(u,u);

	for (size_t cy = 0; cy < resY; ++cy) {
		for (size_t cx = 0; cx < resX; ++cx) {

			// projection of point (cx, cy) onto line
			// through the segment
			vec2 p(lenX*cx + lenX/2.0f, lenY*cy + lenY/2.0f);
			vec2 sp = p - s;
			float l0 = ((physim::math::dot(sp,u))/uu);
			vec2 proj = s + u*l0;

			// distance from point (cx,cy) to the segment
			float D;

			if (in_box(s,t, proj)) {
				D = physim::math::dist(p, proj);
			}
			else {
				D = std::min(physim::math::dist(p, s), physim::math::dist(p, t));
			}

			grid_cells[get_global(cx,cy)] =
				std::min(grid_cells[get_global(cx,cy)], D);
		}
	}
}

void regular_grid::make_final_state() {
	max_dist = 0.0f;
	for (size_t cy = 0; cy < resY; ++cy) {
		for (size_t cx = 0; cx < resX; ++cx) {
			max_dist = std::max(max_dist, grid_cells[get_global(cx,cy)]);
		}
	}
}

// GETTERS

void regular_grid::find_path(
	const vec2& source, const vec2& sink,
	std::vector<vec2>& segs
)
{

}

path_finder_type regular_grid::get_type() const {
	return path_finder_type::regular_grid;
}

const float *regular_grid::get_grid() const {
	return grid_cells;
}

size_t regular_grid::get_resX() const {
	return resX;
}
size_t regular_grid::get_resY() const {
	return resY;
}

float regular_grid::get_dimX() const {
	return dimX;
}
float regular_grid::get_dimY() const {
	return dimY;
}

float regular_grid::get_max_dist() const {
	return max_dist;
}

// OTHERS

void regular_grid::inspect() {
	cout.setf(ios::fixed);
	cout.precision(2);
	for (size_t i = 0; i < resY; ++i) {
		for (size_t j = 0; j < resX; ++j) {
			cout << setw(5) << grid_cells[get_global(j,i)] << " ";
		}
		cout << endl;
	}
}

} // -- namespace charanim
