#pragma once

// C++ includes
#include <memory>
#include <vector>

// Cal3d includes
#include <cal3d/cal3d.h>
#include <cal3d/coremodel.h>
#include <cal3d/coresubmesh.h>

// render includes
#include <render/triangle_mesh/triangle_mesh_utils.hpp>

class rendered_character {
	private:
		std::shared_ptr<CalCoreModel> core_model;
		std::shared_ptr<CalModel> model;

		std::vector<material> all_mats;

		/// Vertex Array Object index
		uint VAO;
		/// Vertex Buffer Object index.
		uint VBO;
		/**
		 * @brief Indices Buffer Object index.
		 *
		 * May contain only material indices or both
		 * material and texture indices.
		 */
		uint IBO;
		/// Element Buffer Object (indices).
		uint EBO;

	public:
		rendered_character();
		~rendered_character();

		void clear_buffers();

		void set_cal_info
		(std::shared_ptr<CalCoreModel> core_model,
		 std::shared_ptr<CalModel> model);

		// needs cal_info
		void initialise_buffers();

		// needs buffers initialised
		bool fill_buffers();


		std::shared_ptr<CalCoreModel> get_core_model();
		const std::shared_ptr<CalCoreModel> get_core_model() const;

		std::shared_ptr<CalModel> get_model();
		const std::shared_ptr<CalModel> get_model() const;
};
