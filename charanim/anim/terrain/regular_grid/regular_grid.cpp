#include <anim/terrain/regular_grid/regular_grid.hpp>

// C includes
#include <stdlib.h>

// C++ includes
#include <algorithm>
#include <utility>
#include <queue>
using namespace std;

// physim includes
#include <physim/math/vec2.hpp>

// charanim includes
#include <anim/terrain/regular_grid/ray_rasterize_4_way.hpp>

namespace charanim {

#define global(x,y, g) g = y*resX + x
#define get_global(x,y) static_cast<size_t>(y)*resX + static_cast<size_t>(x)
#define  local(g, x,y) x = g%resX; y = g/resX

#define in_box(s,t, p)													\
	(((std::min(s.x, t.x) <= p.x) and (p.x <= std::max(s.x,t.x))) and	\
	 ((std::min(s.y, t.y) <= p.y) and (p.y <= std::max(s.y,t.y))))

#define charanim_l1(x1,y1, x2,y2) std::abs(x2 - x1) + std::abs(y2 - y1)
#define l1(c1, c2) charanim_l1(c1.x(), c1.y(), c2.x(), c2.y())
#define charanim_l2(x1,y1, x2,y2) std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
#define l2(c1, c2) charanim_l2(c1.x(), c1.y(), c2.x(), c2.y())

// PRIVATE

#define make_neighbour(i, cx, cy)				\
	if (grid_cells[get_global(cx,cy)] >= R) {	\
		ns[i].x() = cx; ns[i].y() = cy; ++i;	\
	}

latticePoint regular_grid::from_vec2_to_latPoint(const vec2& v) const {
	int px = static_cast<int>(v.x/lenX);
	int py = static_cast<int>(v.y/lenY);
	return latticePoint(px,py);
}

vec2 regular_grid::from_latPoint_to_vec2(const latticePoint& v) const {
	return vec2(lenX*v.x() + lenX/2.0f, lenY*v.y() + lenY/2.0f);
}

vec2 regular_grid::from_latPoint_to_vec2(size_t x, size_t y) const {
	return vec2(lenX*x + lenX/2.0f, lenY*y + lenY/2.0f);
}

size_t regular_grid::make_neighbours
(const latticePoint& p, float R, latticePoint ns[8]) const
{
	size_t it = 0;
	int _resX = static_cast<int>(resX);
	int _resY = static_cast<int>(resY);

	if (p.y() > 0) {
		if (p.x() > 0) { make_neighbour(it, p.x() - 1, p.y() - 1) }
		make_neighbour(it, p.x(), p.y() - 1)
		if (p.x() < _resX) { make_neighbour(it, p.x() + 1, p.y() - 1) }
	}

	if (p.x() > 0) { make_neighbour(it, p.x() - 1, p.y()) }
	if (p.x() < _resX) { make_neighbour(it, p.x() + 1, p.y()) }

	if (p.y() < _resY) {
		if (p.x() > 0) { make_neighbour(it, p.x() - 1, p.y() + 1) }
		make_neighbour(it, p.x(), p.y() + 1)
		if (p.x() < _resX) { make_neighbour(it, p.x() + 1, p.y() + 1) }
	}

	return it;
}

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
	lenX = dimX/resX;
	lenY = dimY/resY;
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
	/* ------------------------------- */
	/* rasterise segment into the grid */
	ray_rasterize_4_way ray;

	const vec2& s = seg.first;
	const vec2& t = seg.second;

	latticePoint lP = from_vec2_to_latPoint(s);
	latticePoint lQ = from_vec2_to_latPoint(t);

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
	const vec2& s = seg.first;
	const vec2& t = seg.second;

	/* compute distance of every other cell to the segment */
	vec2 u = t - s;
	float uu = physim::math::dot(u,u);

	for (size_t cy = 0; cy < resY; ++cy) {
		for (size_t cx = 0; cx < resX; ++cx) {

			// projection of point (cx, cy) onto line
			// through the segment
			vec2 p = from_latPoint_to_vec2(cx,cy);
			vec2 sp = p - s;
			float l0 = ((physim::math::dot(sp,u))/uu);
			vec2 proj = s + u*l0;

			// distance from point (cx,cy) to the segment
			float D;

			if (in_box(s,t, proj)) {
				D = physim::math::dist(p, proj);
			}
			else {
				D = std::min(physim::math::dist(p, s),
							 physim::math::dist(p, t));
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
	float R, std::vector<vec2>& segs
)
{
	typedef double pct;
	typedef pair<pct, latticePoint> node;
	const latticePoint start = from_vec2_to_latPoint(source);
	const latticePoint goal = from_vec2_to_latPoint(sink);

	latticePoint ns[8];

	// came_from[i] = (cx,cy) <-> cell i is reached via cell (cx,cy).
	vector<latticePoint> came_from(resX*resY, latticePoint(0,0));
	// cost_so_far[i] = d <-> cell i is reached with cost d.
	vector<pct> cost_so_far(resX*resY, numeric_limits<pct>::max());
	// seen[i] = true <-> cell i has been visited at some point.
	vector<bool> seen(resX*resY, false);

	// function to estimate the cost of going from a
	// cell 'C' to another cell 'G'
	auto heuristic =
	[](const latticePoint& cell, const latticePoint& G) {
		return l2(cell, G);
	};

	priority_queue<node> Q;
	Q.push(node(-0.0f, start));

	bool reached_goal = false;
	while (not Q.empty() and not reached_goal) {
		node top = Q.top();
		Q.pop();

		cout << "Current cell: "
			 << top.second.x() << "," << top.second.y()
			 << endl;

		// if we have reached the goal then
		// go to the end of the while loop
		if (top.second == goal) {
			cout << "    reached goal" << endl;
			reached_goal = true;
			continue;
		}

		pct cur_cost = -top.first;
		assert(cur_cost >= static_cast<pct>(0.0));
		const latticePoint& cur_cell = top.second;

		// obtain the valid neighbours around the current cell
		size_t n = make_neighbours(cur_cell, R, ns);

		// iterate through the neighbours and add them
		// to the priority queue if needed
		for (size_t i = 0; i < n; ++i) {
			const latticePoint& neigh = ns[i];
			size_t neigh_idx = get_global(neigh.x(), neigh.y());

			pct new_cost =
				cur_cost +				// cost of going from start to current cell
				l1(cur_cell, ns[i]);	// cost of going from current cell to neighbour

			if (not seen[neigh_idx] or new_cost < cost_so_far[neigh_idx]) {

				seen[neigh_idx] = true;
				cost_so_far[neigh_idx] = new_cost;
				came_from[neigh_idx] = cur_cell;

				// approximate cost of going from the
				// neighbour to the goal.
				pct H = heuristic(neigh, goal);
				// actual priority of neighbour
				pct priority = new_cost + H;
				// add to queue
				Q.push(node(-priority, neigh));
			}
		}
	}

	cout << "Making path..." << endl;

	latticePoint lp = goal;
	while (seen[get_global(lp.x(), lp.y())] and lp != start) {
		cout << lp.x() << "," << lp.y() << endl;
		segs.push_back(from_latPoint_to_vec2(lp));
		lp = came_from[get_global(lp.x(), lp.y())];
	}

	cout << "reversing..." << endl;

	std::reverse(segs.begin(), segs.end());
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

} // -- namespace charanim
