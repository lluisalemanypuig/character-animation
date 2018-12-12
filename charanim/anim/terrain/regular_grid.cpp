#include <anim/terrain/regular_grid.hpp>

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
#include <anim/terrain/ray_rasterize_4_way.hpp>

namespace charanim {

static inline
float dist_point_to_rect(const vec2& p1, const vec2& p2, const vec2& pm) {
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;
	float xm = pm.x;
	float ym = pm.y;

	float num = std::abs( (y2 - y1)*xm - (x2 - x1)*ym + x2*y1 - y2*x1 );
	float den = std::sqrt( (y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1) );
	return num/den;
}

#define global(x,y) static_cast<size_t>(y)*resX + static_cast<size_t>(x)
#define  local(g, x,y) x = g%resX; y = g/resX

#define in_box(s,t, p)													\
	(((std::min(s.x, t.x) <= p.x) and (p.x <= std::max(s.x,t.x))) and	\
	 ((std::min(s.y, t.y) <= p.y) and (p.y <= std::max(s.y,t.y))))

#define charanim_l1(x1,y1, x2,y2) std::abs(x2 - x1) + std::abs(y2 - y1)
#define l1(c1, c2) charanim_l1(c1.x(),c1.y(), c2.x(),c2.y())
#define charanim_l2(x1,y1, x2,y2) std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
#define l2(c1, c2) charanim_l2(c1.x(),c1.y(), c2.x(),c2.y())

#define make_neighbour(i, cx, cy)				\
	if (grid_cells[global(cx,cy)] >= R) {	\
		ns[i].x() = cx; ns[i].y() = cy; ++i;	\
	}

// PRIVATE

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

regular_grid::regular_grid() {
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
		g_idx = global(grid_cell.x(), grid_cell.y());
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

			grid_cells[global(cx,cy)] =
				std::min(grid_cells[global(cx,cy)], D);
		}
	}
}

void regular_grid::make_final_state() {
	max_dist = 0.0f;
	for (size_t cy = 0; cy < resY; ++cy) {
		for (size_t cx = 0; cx < resX; ++cx) {
			max_dist = std::max(max_dist, grid_cells[global(cx,cy)]);
		}
	}
}

// GETTERS

void regular_grid::find_path(
	const vec2& source, const vec2& sink,
	float R,
	vector<vec2>& path,
	vector<vec2>& smoothed_path
)
{
	/* This algorithm can be optimised a lot but I'm
	 * running out of time. For example, the priority queue
	 * can be optimised to have a single instance of each node
	 * (identified by lattice point). To do this we would have
	 * to change a node's priority and remake the heap, instead
	 * of adding it again with a different priority (as it is
	 * done now).
	 */

	typedef double pct;
	typedef pair<pct, latticePoint> node;
	const latticePoint start = from_vec2_to_latPoint(source);
	const latticePoint goal = from_vec2_to_latPoint(sink);

	latticePoint ns[8];

	// came_from[i] = (cx,cy) <-> cell i is reached via cell (cx,cy).
	vector<latticePoint> came_from(resX*resY, latticePoint(0,0));
	// cost_so_far[i] = d <-> cell i is reached with cost d.
	vector<pct> cost_so_far(resX*resY, 0.0);
	// cost_is_valid[i] = d <-> cost to reach cell i in 'cost_so_far'
	//						is valid
	vector<bool> valid_cost(resX*resY, false);

	// function to estimate the cost of going from a
	// cell 'C' to another cell 'G'
	auto heuristic =
	[&](const latticePoint& cell) {
		return l2(cell, goal)*0.5;
	};

	priority_queue<node> Q;
	Q.push(node(-0.0, start));
	valid_cost[global(start.x(), start.y())] = true;
	cost_so_far[global(start.x(), start.y())] = 0.0;
	bool reached_goal = false;

	while (not Q.empty() and not reached_goal) {
		node top = Q.top();
		Q.pop();

		// if we have reached the goal then
		// go to the end of the while loop
		if (top.second == goal) {
			reached_goal = true;
		}

		const latticePoint& cur_cell = top.second;
		size_t cur_idx = global(cur_cell.x(), cur_cell.y());
		pct cur_cost = cost_so_far[cur_idx];

		// obtain the valid neighbours around the current cell
		size_t n = make_neighbours(cur_cell, R, ns);

		// iterate through the neighbours and add them
		// to the priority queue if needed
		for (size_t i = 0; i < n; ++i) {
			const latticePoint& neigh = ns[i];
			size_t neigh_idx = global(neigh.x(), neigh.y());

			pct new_cost =
				cur_cost +			// cost of going from start to current cell
				l2(cur_cell, neigh);// cost of going from current cell to neighbour

			if (
				(not valid_cost[neigh_idx]) or
				(valid_cost[neigh_idx] and new_cost < cost_so_far[neigh_idx])
			)
			{
				valid_cost[neigh_idx] = true;
				cost_so_far[neigh_idx] = new_cost;
				came_from[neigh_idx] = cur_cell;

				// approximate cost of going from the
				// neighbour to the goal.
				pct H = heuristic(neigh);
				// actual priority of neighbour
				pct priority = new_cost + H;
				// add to queue
				Q.push(node(-priority, neigh));
			}
		}
	}

	// make path from goal to start and reverse
	latticePoint lp = goal;
	while (lp != start) {
		path.push_back(from_latPoint_to_vec2(lp));
		lp = came_from[global(lp.x(), lp.y())];
	}
	std::reverse(path.begin(), path.end());

	// refine path using polylines
	if (path.size() <= 2) {
		smoothed_path = path;
		return;
	}

	smoothed_path.push_back(path[0]);
	const float tol = 0.5f;

	unsigned int i0 = 0;
	unsigned int i1 = 1;
	while (i1 + 1 < path.size()) {
		float d = dist_point_to_rect(smoothed_path[i0], path[i1 + 1], path[i1]);
		cout << "d= " << d << endl;
		if (d > tol) {
			smoothed_path.push_back(path[i1]);
			++i0;
		}

		++i1;
	}
	smoothed_path.push_back(path[i1]);
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
