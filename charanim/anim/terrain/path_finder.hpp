#pragma once

// C++ includes
#include <cstdint>
#include <vector>

// charanim includes
#include <anim/definitions.hpp>

namespace charanim {

/// The different types of path finders
enum class path_finder_type : int8_t {
	/// Null type
	none = 0,
	/// Regular grid, see @ref regular_grid.
	regular_grid,
	/// Visibility graph, see @ref regular_grid.
	visibility_graph
};

/**
 * @brief Abstract class to implement path finding algorithms
 * and terrain representation data structures.
 */
class path_finder {
	public:
		path_finder();
		virtual ~path_finder();

		// MODIFIERS

		/// Initialises the path finder structure.
		virtual void init(const std::vector<segment>& segs) = 0;

		/// Clears path finding structure.
		virtual void clear() = 0;

		// GETTERS

		/// Initialises the path finder structure.
		virtual void find_path(
			const point2D& source, const point2D& sink,
			std::vector<point2D>& segs
		) = 0;

		/// Returns the type of the path finding representation.
		virtual path_finder_type get_type() const = 0;
};

} // -- namespace charanim
