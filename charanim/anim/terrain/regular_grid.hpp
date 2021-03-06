/*********************************************************************
 * charanim - Character Animation Project
 * Copyright (C) 2018 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// C++ includes
#include <cstddef>
#include <vector>

// charanim includes
#include <anim/definitions.hpp>

namespace charanim {

class regular_grid {
	private:
		/**
		 * @brief Cells of the grid.
		 *
		 * Each cell contains the distance function to the closest segment in
		 * the grid. The segments are rasterised into cells with value 0. The
		 * other cells have
		 *
		 * \f$min\{d_1, d_2, \cdots, d_n\}\f$
		 *
		 * where \f$d_i\f$ is the distance between the cell and the @e i-th
		 * segment.
		 */
		float *grid_cells;

		/// Number of cells in the x-axis.
		size_t resX;
		/// Number of cells in the y-axis.
		size_t resY;

		/// Continuous dimension in the x-axis
		float dimX;
		/// Continuous dimension in the y-axis
		float dimY;

		/// Length of every cell in the x-axis.
		float lenX;
		/// Length of every cell in the y-axis.
		float lenY;

		/// Maximum value in the cells.
		float max_dist;

	private:

		/// Convert a vec2 to a lattice point.
		latticePoint from_vec2_to_latPoint(const vec2& v) const;
		/// Convert a lattice point to a vec2.
		vec2 from_latPoint_to_vec2(const latticePoint& v) const;
		/// Convert a lattice point to a vec2.
		vec2 from_latPoint_to_vec2(size_t x, size_t y) const;

		/**
		 * @brief Finds the (at most) 8 neighbours of the grid cell p.
		 * @param[in] p A valid grid cell.
		 * @param[in] R The minimum distance between a neighbour and the
		 * closest obstacle.
		 * @param[out] ns Valid neighbouring grid cells.
		 * @return Returns the amount of valid neighbours in @e ns.
		 */
		size_t make_neighbours
		(const latticePoint& p, float R, latticePoint ns[8]) const;

	public:
		/// Default constructor.
		regular_grid();
		/// Destructor.
		~regular_grid();

		// MODIFIERS

		/// Initialises the grid with cellsx x cellsy cells.
		void init(size_t cellsx, size_t cellsy, float dimX, float dimY);
		/// Raterises the segments in @e segs and updates the distance
		/// function in the grid's cells.
		void init(const std::vector<segment>& segs);

		/// Clears the memory occupied by this grid.
		void clear();

		/**
		 * @brief Rasterises the segment into the grid.
		 *
		 * Each cell of the rasterised segment is assigned value 0.
		 */
		void rasterise_segment(const segment& s);

		/**
		 * @brief Computes the distance function of every cell.
		 *
		 * With respect to a *rasterised* segment @e s.
		 */
		void expand_function_distance(const segment& s);

		/**
		 * @brief Computes necessary internal data.
		 *
		 * Computes the maximum value of the values in the cells.
		 */
		void make_final_state();

		// GETTERS

		/**
		 * @brief Fins a path between two points.
		 * @param[in] source Starting point.
		 * @param[in] sink Goal point.
		 * @param[in] R Minimum distance between the path and fixed obstacles.
		 * @param[out] path Non-refined path.
		 * @param[out] smooth_path Refined path.
		 */
		void find_path(
			const vec2& source, const vec2& sink,
			float R,
			std::vector<vec2>& path,
			std::vector<vec2>& smoothed_path
		);

		/// Returns the cells of the grid.
		const float *get_grid() const;

		/// Returns the number of cells in the x-axis.
		size_t get_resX() const;
		/// Returns the number of cells in the y-axis.
		size_t get_resY() const;

		/// Returns the continuous dimension in the x-axis
		float get_dimX() const;
		/// Returns the continuous dimension in the y-axis
		float get_dimY() const;

		/// Returns the maximum value in the cells.
		float get_max_dist() const;

		// OTHERS
};

} // -- namespace charanim
