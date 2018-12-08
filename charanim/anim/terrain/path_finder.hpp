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
		/// Default constructor.
		path_finder();
		/// Destructor.
		virtual ~path_finder();

		// MODIFIERS

		/// Initialises the path finder structure.
		virtual void init(const std::vector<segment>& segs) = 0;

		/// Clears path finding structure.
		virtual void clear() = 0;

		// GETTERS

		/**
		 * @brief Finds a path between points 'source' and 'sink'.
		 * @param source Start of the path.
		 * @param sink Goal of the parth.
		 * @param R The minimum distance the path has to be from an obstacle.
		 * @param[out] segs The path from @e source to @e sink.
		 */
		virtual void find_path(
			const vec2& source, const vec2& sink,
			float R, std::vector<vec2>& segs
		) = 0;

		/// Returns the type of the path finding representation.
		virtual path_finder_type get_type() const = 0;
};

} // -- namespace charanim
