#pragma once

// C++ includes
#include <utility>
#include <string>
#include <vector>

// anim includes
#include <anim/definitions.hpp>
#include <anim/terrain/path_finder.hpp>

namespace charanim {

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
		path_finder *pf;

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
		const path_finder *get_path_finder() const;

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
