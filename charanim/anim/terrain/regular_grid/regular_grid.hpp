#pragma once

// C++ includes
#include <cstddef>
#include <vector>

// charanim includes
#include <anim/definitions.hpp>
#include <anim/terrain/path_finder.hpp>

namespace charanim {

class regular_grid : public path_finder {
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

		/// Maximum value in the cells.
		float max_dist;

	public:
		regular_grid();
		~regular_grid();

		// MODIFIERS

		/// Initialises the grid with cellsx x cellsy cells.
		void init(size_t cellsx, size_t cellsy, float dimX, float dimY);
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

		void find_path(
			const vec2& source, const vec2& sink,
			std::vector<vec2>& segs
		);

		path_finder_type get_type() const;

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

		/// Prints to standard output the contents of the grid.
		void inspect();
};

} // -- namespace charanim
