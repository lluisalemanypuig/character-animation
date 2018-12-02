#include <render/textures/texture_loader.hpp>

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// render includes
#include <render/textures/std_image.hpp>
#include <render/err_war_utils.hpp>
#include <render/include_gl.hpp>

// PRIVATE

texture_loader::texture_loader() {
	tex_loaded = 0;
}

// PUBLIC

texture_loader::~texture_loader() {
	clear_all();
}

// MODIFIERS

void texture_loader::load_textures(vector<material>& mats, vector<unsigned int>& idxs)
{
	set<unsigned int> unique_texs;
	for (size_t i = 0; i < mats.size(); ++i) {
		material& m = mats[i];

		if (m.txt_name == __NULL_TEXTURE_NAME) {
			m.txt_id = __NULL_TEXTURE_INDEX;
			continue;
		}

		// check if it has been already loaded
		auto it = textures.find(m.txt_name);
		unsigned int id = 0;
		if (it != textures.end()) {
			id = it->second;
		}

		// already loaded or not previously freed
		if (id != 0) {
			unique_texs.insert(id);
			m.txt_id = id;
			continue;
		}

		// load texture and assign index to material

		#if defined (DEBUG)
		cout << line << " texture_loader::load_textures:" << endl;
		cout << line << "     About to load image '" << m.txt_name << "'" << endl;
		cout << line << "     of " << i << "-th material" << endl;
		#endif

		int width, height, n_channels;
		unsigned char *data =
			stbi_load(m.txt_name.c_str(), &width, &height, &n_channels, 0);
		if (data == nullptr) {
			cerr << "texture_loader::load_textures - " << ERR << ":" << endl;
			cerr << "    Could not load texture '" << m.txt_name << "'" << endl;
			cerr << "    in material '" << m.id << "'" << endl;
		}

		#if defined (DEBUG)
		cout << line << "     image loaded!" << endl;
		cout << line << "         dimensions: " << width << "x" << height << endl;
		cout << line << "         # channels: " << n_channels << endl;
		cout << line << "     Generating texture... ";
		#endif

		glGenTextures(1, &id);
		assert(glGetError() == GL_NO_ERROR);

		#if defined (DEBUG)
		cout << "id: " << id << endl;
		cout << line << "     glBindTexture...      ";
		#endif

		glBindTexture(GL_TEXTURE_2D, id);
		assert(glGetError() == GL_NO_ERROR);

		#if defined (DEBUG)
		cout << "done" << endl;
		cout << line << "     glTexImage2D...       ";
		#endif

		if (n_channels == 3) {
			glTexImage2D
			(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			assert(glGetError() == GL_NO_ERROR);
		}
		else if (n_channels == 4) {
			glTexImage2D
			(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			assert(glGetError() == GL_NO_ERROR);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		assert(glGetError() == GL_NO_ERROR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		assert(glGetError() == GL_NO_ERROR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		assert(glGetError() == GL_NO_ERROR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		assert(glGetError() == GL_NO_ERROR);

		#if defined (DEBUG)
		cout << "done" << endl;
		#endif

		glGenerateMipmap(GL_TEXTURE_2D);
		assert(glGetError() == GL_NO_ERROR);

		stbi_image_free(data);

		textures[m.txt_name] = id;
		m.txt_id = id;
		unique_texs.insert(id);
		++tex_loaded;
	}

	idxs.clear();
	idxs = vector<unsigned int>( unique_texs.begin(), unique_texs.end() );

	if (tex_loaded > __MAX_TEXTURES_LOADED) {
		cerr << "texture_loader::load_textures - " << ERR << endl;
		cerr << "    Maximum amount of loaded textures reached" << endl;
	}
}

/**
 * @brief Clears the textures of a collection of materials.
 *
 * Given a collection of materials, either withor without textures,
 * clear all their textures. If a texture was cleared previouly,
 * then it is NOT cleared twice.
 * @param[in] mats Collection of materials.
 */
void texture_loader::clear_textures(const vector<material>& mats) {
	for (const material& m : mats) {
		if (m.txt_name == __NULL_TEXTURE_NAME) {
			continue;
		}

		// check if it has been already freed
		auto it = textures.find(m.txt_name);
		unsigned int id = 0;
		if (it != textures.end()) {
			id = it->second;
		}
		// not loaded or previously freed
		if (id == 0) {
			continue;
		}

		glDeleteTextures(1, &id);
		assert(glGetError() == GL_NO_ERROR);
		it->second = 0;
		--tex_loaded;
	}
}

void texture_loader::clear_all() {
	if (tex_loaded > 0) {
		#if defined(DEBUG)
		cout << line << " texture_loader::clear_all() - Delete all texutres" << endl;
		#endif
		for (auto& txt_id : textures) {
			if (txt_id.second == 0) {
				continue;
			}
			#if defined(DEBUG)
			cout << line << "     deleting " << txt_id.second << endl;
			#endif
			glDeleteTextures(1, &txt_id.second);
			assert(glGetError() == GL_NO_ERROR);
			txt_id.second = 0;
		}
		tex_loaded = 0;
	}
}

// GETTERS

bool texture_loader::texture_index(const string& s, unsigned int& idx) const {
	auto it = textures.find(s);
	if (it == textures.end()) {
		return false;
	}
	idx = it->second;
	return true;
}
