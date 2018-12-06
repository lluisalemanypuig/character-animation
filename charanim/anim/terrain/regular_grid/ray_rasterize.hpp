#pragma once

// glm includes
#include <glm/vec2.hpp>

// custom includes
#include <anim/definitions.hpp>

namespace charanim {

/// Abstract class used to make it easier to implement different types
/// of ray rasterizers and to use them in the visibility analyzers.
/// Be mindful that the x value of a cell represents the column of a matix,
/// and the y value represents the row of a matrix.
class ray_rasterize {
	private:
	public:
		ray_rasterize();
		virtual ~ray_rasterize();
		
		// Initializes this class using the initial point (ini) and the
		// last point (fin). Ray from ini to fin.
		virtual void init(const latticePoint& ini, const latticePoint& fin) = 0;
		
		// Clones this object.
		virtual ray_rasterize *clone() const = 0;
		
		// Returns the current cell as a 2-tuple in the ray from ini to fin.
		// If the functions get_next_cell, get_previous_cell, advance_cell
		// nor retreat_cell have been called, the first cell to be returned
		// is ini.
		virtual void current_cell(latticePoint& current) const = 0;
		
		// Advances the current cell one step in the rasterized ray.
		virtual void advance() = 0;
		
		// Retreats the current cell one step in the rasterized ray.
		virtual void retreat() = 0;
		
		// Returns the current cell as a 2-tuple in the ray from ini to fin
		// and advances one step in the ray.
		// If the functions advance_cell, retreat_cell nor get_previous_cell
		// have been called, the first cell to be returned is ini.
		void get_advance(latticePoint& next);
		
		// Returns the current cell as a 2-tuple in the ray from ini to fin
		// and advances one step in the ray.
		// If the functions advance_cell, retreat_cell nor get_next_cell
		// have been called, the first cell to be returned is ini.
		void get_retreat(latticePoint& next);
		
		// Advances/Retreats until the cell p. The first cell returned
		// after calling this method is the cell p.
		// pre: the cell must be on the ray
		virtual void place_at(const latticePoint& p) = 0;
		
		// Returns true iff ini has been reached from any cell on the ray.
		virtual bool is_first() const = 0;
		
		// Returns true iff fin has been reached from any cell on the ray.
		virtual bool is_last() const = 0;
		
		// Returns true iff c is on the *ray* going through ini and fin.
		// (ray, ¡not segment!)
		virtual bool on_ray(const latticePoint& c) const = 0;
		
};

} // -- namespace charanim
