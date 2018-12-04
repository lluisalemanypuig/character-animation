#pragma once

// C++ includes
#include <cstddef>
#include <vector>

// charanim includes
#include <anim/terrain/definitions.hpp>
#include <anim/terrain/path_finder.hpp>

namespace charanim {

class regular_grid : public path_finder {
	private:
		/**
		 * @brief Cells of the grid.
		 *
		 * A cell is marked as true if it is occupied.
		 */
		std::vector<bool> occupied_cells;

		/// Number of cells in the x-axis.
		size_t resX;
		/// Number of cells in the y-axis.
		size_t resY;

	public:
		regular_grid();
		~regular_grid();

		// MODIFIERS

		/// Initialises the grid with cellsx x cellsy cells.
		void init(size_t cellsx, size_t cellsy);
		void init(const std::vector<segment>& segs);

		void clear();

		// GETTERS

		void find_path(
			const point2D& source, const point2D& sink,
			std::vector<point2D>& segs
		);

		path_finder_type get_type() const;
};

} // -- namespace charanim
