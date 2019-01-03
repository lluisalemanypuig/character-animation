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
#include <anim/utils/indexed_minheap.hpp>

namespace charanim {

#define MAX_FINF numeric_limits<float>::max()
#define MAX_DINF numeric_limits<double>::max()
#define NO_LIST		0x00
#define OPEN_LIST	0x01
#define CLOSED_LIST 0x02
#define global_xy(x,y) static_cast<size_t>(y)*resX + static_cast<size_t>(x)
#define global_latpoint(C) static_cast<size_t>(C.y())*resX + static_cast<size_t>(C.x())
#define vec2_out(c) "(" << c.x << "," << c.y << ")"
#define latpoint_out(c) "(" << c.x() << "," << c.y() << ")"

#define in_box(s,t, p)													\
	(((std::min(s.x, t.x) <= p.x) and (p.x <= std::max(s.x,t.x))) and	\
	 ((std::min(s.y, t.y) <= p.y) and (p.y <= std::max(s.y,t.y))))

#define charanim_l1(x1,y1, x2,y2) (std::abs(x2 - x1) + std::abs(y2 - y1))
#define l1(c1, c2) charanim_l1(c1.x(),c1.y(), c2.x(),c2.y())
#define charanim_l2(x1,y1, x2,y2) (std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)))
#define l2(c1, c2) charanim_l2(c1.x(),c1.y(), c2.x(),c2.y())

#define make_neighbour(i, cx, cy)				\
	if (grid_cells[global_xy(cx,cy)] >= R) {	\
		ns[i].x() = cx; ns[i].y() = cy; ++i;	\
	}

struct node {
	latticePoint latpoint;
	double priority;
	char LIST;

	node() {
		LIST = NO_LIST;
	}
	node(char l, double p, const latticePoint& lp) {
		LIST = l;
		priority = p;
		latpoint = lp;
	}

	inline bool in_none() const {
		return LIST == NO_LIST;
	}
	inline bool in_closed() const {
		return (LIST & CLOSED_LIST) != 0;
	}
	inline bool in_open() const {
		return (LIST & OPEN_LIST) != 0;
	}

	inline bool operator< (const node& n) const {
		// if this node is not the queue it should have lowest priority
		if (not in_open()) {
			return false;
		}
		// if parameter node is not the queue it should have lowest priority
		if (not n.in_open()) {
			return true;
		}

		// if both nodes are in the queue then compare priorities
		return priority < n.priority;
	}
};

static inline
float dist_point_to_rect(const vec2& p1, const vec2& p2, const vec2& pm) {
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;
	float x0 = pm.x;
	float y0 = pm.y;

	float num = std::abs( (y2 - y1)*x0 - (x2 - x1)*y0 + x2*y1 - y2*x1 );
	float den = std::sqrt( (y2 - y1)*(y2 - y1) + (x2 - x1)*(x2 - x1) );
	return num/den;
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
		if (p.x() > 0) {
			make_neighbour(it, p.x() - 1, p.y() - 1)
		}
		make_neighbour(it, p.x(), p.y() - 1)
		if (p.x() < _resX) {
			make_neighbour(it, p.x() + 1, p.y() - 1)
		}
	}

	if (p.x() > 0) {
		make_neighbour(it, p.x() - 1, p.y())
	}
	if (p.x() < _resX) {
		make_neighbour(it, p.x() + 1, p.y())
	}

	if (p.y() < _resY) {
		if (p.x() > 0) {
			make_neighbour(it, p.x() - 1, p.y() + 1)
		}
		make_neighbour(it, p.x(), p.y() + 1)
		if (p.x() < _resX) {
			make_neighbour(it, p.x() + 1, p.y() + 1)
		}
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
		g_idx = global_xy(grid_cell.x(), grid_cell.y());
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

			grid_cells[global_xy(cx,cy)] =
				std::min(grid_cells[global_xy(cx,cy)], D);
		}
	}
}

void regular_grid::make_final_state() {
	max_dist = 0.0f;
	for (size_t cy = 0; cy < resY; ++cy) {
		for (size_t cx = 0; cx < resX; ++cx) {
			max_dist = std::max(max_dist, grid_cells[global_xy(cx,cy)]);
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
	// make sure that the particle can start at
	// 'source' and finish at 'end'
	const latticePoint start = from_vec2_to_latPoint(source);
	const latticePoint goal = from_vec2_to_latPoint(sink);

	if (grid_cells[global_latpoint(start)] <= R) {
		cerr << "Error: a particle of radius " << R
			 << " can't start at " << latpoint_out(start) << endl;
		cerr << "    This position is at a distance from a static obstacle"
			 << " of: " << grid_cells[global_latpoint(start)] << endl;
		return;
	}
	if (grid_cells[global_latpoint(goal)] <= R) {
		cerr << "Error: a particle of radius " << R
			 << " can't finish at " << latpoint_out(goal) << endl;
		cerr << "    This position is at a distance from a static obstacle"
			 << " of: " << grid_cells[global_latpoint(goal)] << endl;
		return;
	}

	// function to estimate the cost of going from a
	// cell 'C' to another cell 'G'
	auto heuristic =
	[&](const latticePoint& cell) {
		// distance to closest obstacle
		double dist_closest = double(grid_cells[global_xy(cell.x(),cell.y())]);
		return 0.5*l2(cell, goal) - 2.0*dist_closest;
	};

	// array of neighbours of a lattice point
	latticePoint ns[8];

	vector<node> all_nodes(resX*resY);
	for (size_t x = 0; x < resX; ++x) {
		for (size_t y = 0; y < resY; ++y) {
			all_nodes[ global_xy(x,y) ].priority = MAX_DINF;
			all_nodes[ global_xy(x,y) ].LIST = NO_LIST;
			all_nodes[ global_xy(x,y) ].latpoint.x() = static_cast<int>(x);
			all_nodes[ global_xy(x,y) ].latpoint.y() = static_cast<int>(y);
		}
	}

	// priority queue for the lattice points
	indexed_minheap<node> OPEN;
	OPEN.make_heap(all_nodes);

	// What is the previous cell in the path to cell i?
	// parent[i] = (x,y) <-> the shortest path goes
	// first through (x,y) and then through i.
	// This is used later to reconstruct the path.
	vector<latticePoint> parent(resX*resY);

	// Actual path cost of going from start to cell i:
	// cost_so_far[i] = X <-> the cost of going from start
	// to cell i is equal to X.
	vector<double> cost_so_far(resX*resY, MAX_DINF);

	// Although we make the heap with all points of
	// the lattice, none of them is considered to
	// actually be in the OPEN list
	vector<char> which_list(resX*resY, NO_LIST);

	// initialise
	cost_so_far[ global_latpoint(start) ] = 0.0;
	OPEN.modify_th( global_latpoint(start), node(OPEN_LIST, 0.0, start) );
	which_list[ global_latpoint(start) ] = OPEN_LIST;

	bool reached_goal = false;

	while (not reached_goal) {
		const node& top = OPEN.top();

		// if we have reached the goal then
		// go to the end of the while loop
		if (top.latpoint == goal) {
			reached_goal = true;

			break;
		}

		assert(top.LIST == OPEN_LIST);

		latticePoint cur_cell = top.latpoint;
		size_t cur_idx = global_latpoint(cur_cell);
		double cur_cost = cost_so_far[cur_idx];

		// remove current from OPEN
		which_list[cur_idx] = CLOSED_LIST;
		OPEN.modify_th(cur_idx, node(CLOSED_LIST, top.priority, cur_cell));

		// obtain the valid neighbours around the current cell
		size_t n = make_neighbours(cur_cell, R, ns);

		// iterate through the neighbours and add them
		// to the priority queue if needed
		for (size_t i = 0; i < n; ++i) {
			const latticePoint& neigh = ns[i];
			size_t neigh_idx = global_xy(neigh.x(), neigh.y());

			// cost of reaching neighbour
			// =
			// cost of going from start to current cell
			// +
			// cost of going from current cell to neighbour
			double neigh_cost = cur_cost + l2(cur_cell, neigh);

			// approximate cost of going from the neighbour to the goal
			double h = heuristic(neigh);
			// actual priority of neighbour
			double f = neigh_cost + h;

			if (which_list[neigh_idx] == OPEN_LIST) {
				// lattice point was added at some point
				// to the priority queue
				if (neigh_cost < cost_so_far[neigh_idx]) {
					// the cost of reaching neighbour via another
					// path is less than the current cost
					OPEN.modify_th(neigh_idx, node(NO_LIST, f, neigh));
					which_list[neigh_idx] = NO_LIST;
				}
			}

			if (which_list[neigh_idx] == CLOSED_LIST) {
				// lattice point is considered candidate for path
				if (neigh_cost < cost_so_far[neigh_idx]) {
					// but if the cost of reaching neighbour via
					// another path is less than the current cost
					// do not do it
					OPEN.modify_th(neigh_idx, node(NO_LIST, f, neigh));
					which_list[neigh_idx] = NO_LIST;
				}
			}

			if (which_list[neigh_idx] == NO_LIST) {
				// Lattice point was never added to the heap,
				// or was added and then removed in one of the
				// two previous steps

				OPEN.modify_th(neigh_idx, node(OPEN_LIST, f, neigh));
				cost_so_far[neigh_idx] = neigh_cost;
				parent[neigh_idx] = cur_cell;
				which_list[neigh_idx] = OPEN_LIST;
			}
		}
	}

	// make path from goal to start and reverse
	latticePoint lp = goal;
	while (lp != start) {
		path.push_back(from_latPoint_to_vec2(lp));
		lp = parent[global_latpoint(lp)];
	}
	std::reverse(path.begin(), path.end());

	// refine path using polylines
	if (path.size() <= 2) {
		smoothed_path = path;
		return;
	}

	smoothed_path.push_back(path[0]);
	const float max_dist_allowed = 0.5f*std::sqrt(lenX*lenX + lenY*lenY)/2.0f;

	size_t smooth_it = 0;
	size_t path_it = 1;
	size_t watch_it = path.size() + 1;

	while (path_it + 1 < path.size()) {
		if (watch_it > path.size()) {
			float d = dist_point_to_rect
			(smoothed_path[smooth_it], path[path_it + 1], path[path_it]);

			if (d > 0.0f) {
				// watch this cell closely to avoid
				// underfitting smooth paths
				watch_it = path_it;
			}
		}
		else {
			float wd = dist_point_to_rect
			(smoothed_path[smooth_it], path[path_it], path[watch_it]);
			if (wd > max_dist_allowed) {
				// smooth path must have this node.
				// procedure must then start here
				smoothed_path.push_back(path[watch_it]);
				++smooth_it;
				watch_it = path.size() + 1;
			}
		}
		++path_it;
	}
	smoothed_path.push_back(path[path_it]);
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
