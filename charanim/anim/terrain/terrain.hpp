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
#include <utility>
#include <string>
#include <vector>

// anim includes
#include <anim/definitions.hpp>
#include <anim/terrain/regular_grid.hpp>

namespace charanim {

/// The different types of path finders
enum class path_finder_type : int8_t {
	/// Null type
	none = 0,
	/// Regular grid, see @ref regular_grid.
	regular_grid
};

/**
 * @brief Simple class for 2D terrains.
 */
class terrain {
	private:
		/// The segments of the terrain.
		std::vector<segment> sgs;

		/// Dimension in the x-axis.
		float dimX;
		/// Dimension in the y-axis.
		float dimY;

		/// Underlying data structure for path finding.
		regular_grid *rg;

	public:
		/// Default constructor.
		terrain();
		/// Destructor.
		~terrain();

		// MODIFIERS

		/// Clears the underlying structure for path finding.
		void clear();

		// SETTERS

		// GETTERS

		/**
		 * @brief Returns the segments of this terrain.
		 *
		 * Each segment represents a wall or a piece of it.
		 *
		 * Once the map has been read the underlying structure for path finding
		 * is initialised.
		 * @return Returns a constant reference to @ref sgs.
		 */
		const std::vector<segment>& get_segments() const;

		/// Returns the underlying path finder.
		regular_grid *get_regular_grid();
		/// Returns the underlying path finder.
		const regular_grid *get_regular_grid() const;

		// I/O

		/**
		 * @brief Reads map from a file.
		 *
		 * The current representation of this terrain is cleared.
		 * @param filename File describing the map.
		 * @return Returns true on success.
		 */
		bool read_map(const std::string& filename);
};

} // -- namespace charanim
