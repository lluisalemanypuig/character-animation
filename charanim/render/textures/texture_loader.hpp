#pragma once

// C++ includes
#include <vector>
#include <string>
#include <map>

// render includes
#include <render/model/model_utils.hpp>

/**
 * @brief Single class to manage textures.
 *
 * Used to avoid allocating the same texture
 * into memory more than once, free the memory
 * occupied by the same texture more than once,
 * ...
 */
class texture_loader {
	private:

		/// The textures loaded into memory
		std::map<std::string, unsigned int> textures;
		/// Amount of textures loaded.
		uint tex_loaded;

		/// Constructor
		texture_loader();

	public:
		/**
		 * @brief Destructor
		 *
		 * Calls @ref clear_all method
		 */
		~texture_loader();

		// MODIFIERS

		/**
		 * @brief Loads the textures of a collection of materials.
		 *
		 * Given a collection of materials, either withor without textures,
		 * load all of them into memory. If a texture was loaded previously
		 * then it is NOT loaded again.
		 *
		 * The indexes generated for each texture are stored in @e info.
		 * @param[out] mats Collection of materials. The textures indexes
		 * are is overriden for those materials with textures.
		 * @param[out] idxs At the end will conatin the indexes of the
		 * textures genereated.
		 */
		void load_textures(std::vector<material>& mats,
							 std::vector<unsigned int>& idxs);

		/**
		 * @brief Clears the textures of a collection of materials.
		 *
		 * Given a collection of materials, either withor without textures,
		 * clear all their textures. If a texture was cleared previouly,
		 * then it is NOT cleared twice.
		 * @param[in] mats Collection of materials.
		 */
		void clear_textures(const std::vector<material>& mats);

		/**
		 * @brief Clears all textures loaded so far.
		 */
		void clear_all();

		// GETTERS

		/**
		 * @brief Returns the index of the texture.
		 * @param s Texture to look for.
		 * @param idx (OpenGL) index of the texture.
		 * @return Returns true if the texture was found. In this case
		 * the value in idx is valid. Returns false if the texture was
		 * not found.
		 */
		bool texture_index(const std::string& s, unsigned int& idx) const;

		// OTHERS

		/// Return the texture loader
		static texture_loader& get_loader() {
			static texture_loader L;
			return L;
		}

		/// Deleted copy-constructor.
		texture_loader (const texture_loader& L) = delete;
		/// Deleted assignation operator.
		void operator= (const texture_loader& L) = delete;

};
