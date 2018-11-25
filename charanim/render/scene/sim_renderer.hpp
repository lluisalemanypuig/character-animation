#pragma once

// C++ includes
#include <vector>

// render includes
#include <render/scene/viewer.hpp>
#include <render/geometry/rgeometry.hpp>

// physim includes
#include <physim/simulator.hpp>

/**
 * @brief Simulation renderer.
 */
class sim_renderer : public viewer {
	private:
		/// Renderer's simulator.
		physim::simulator S;
		/// Geometry in the simulation.
		std::vector<rgeom *> geometry;

	public:
		/// Default constructor.
		sim_renderer();
		/// Destructor.
		~sim_renderer();

		void clear();

		void set_particle_size(float s);
		void set_spring_width(float w);
		void add_geometry(rgeom *r);

		const std::vector<rgeom *>& get_geometry() const;

		/// Returns a reference to the renderer's simulator.
		physim::simulator& get_simulator();

		void apply_time_step();
		void render_geometry() const;
		void render_simulation() const;
};
