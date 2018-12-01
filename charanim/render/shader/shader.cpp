#include <render/shader/shader.hpp>

// C++ includes
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

// glm includes
#include <glm/gtc/type_ptr.hpp>

// render includes
#include <render/err_war_utils.hpp>

shader::shader() {
	ID = 0;
}

shader::~shader() {
	clear();
}

// MODIFIERS

bool shader::init(const string& dir, const string& vertex_name, const string& fragment_name) {
	string vertex_file = dir + "/" + vertex_name;
	string fragment_file = dir + "/" + fragment_name;

	#if defined (DEBUG)
	cout << line << " shader::init - Load shader programs:" << endl;
	cout << line << "     vertex:   " << vertex_file << endl;
	cout << line << "     fragment: " << fragment_file << endl;
	#endif

	// 1. retrieve the vertex/fragment source code from filePath
	string vertex_code, fragment_code;
	ifstream vertex_shader_file, fragment_shader_file;

	// ensure ifstream objects can throw
	// exceptions when reading
	vertex_shader_file.exceptions(ifstream::badbit);
	fragment_shader_file.exceptions(ifstream::badbit);

	int success = 1;

	try {
		// open files
		vertex_shader_file.open(vertex_file);
		if (not vertex_shader_file.is_open()) {
			throw new ifstream::failure("Could not open vertex shader file.");
		}

		fragment_shader_file.open(fragment_file);
		if (not fragment_shader_file.is_open()) {
			throw new ifstream::failure("Could not open fragment shader file.");
		}

		#if defined (DEBUG)
		cout << line << "     Reading buffers..." << endl;
		#endif

		// read file's buffer contents into streams
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		#if defined (DEBUG)
		cout << line << "         buffers read" << endl;
		#endif

		// close file handlers
		vertex_shader_file.close();
		fragment_shader_file.close();

		// convert stream into string
		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();

		#if defined (DEBUG)
		cout << line << "         contents of files stored" << endl;
		#endif
	}
	catch (ifstream::failure e) {
		cerr << "shader::init - " << ERR << endl;
		cerr << "    While reading shader files." << endl;
		cerr << "    Exception: " << string(e.what()) << endl;
		success = 0;
	}

	if (success == 0) {
		cerr << line << "     Initialisation of shader program aborted." << endl;
		return false;
	}

	const char *vertex_shader_code = vertex_code.c_str();
	const char *fragment_shader_code = fragment_code.c_str();

	#if defined (DEBUG)
	cout << line << "     Compile shaders..." << endl;
	#endif

	// 2. compile shaders
	char info_buf[512];

	#if defined (DEBUG)
	cout << line << "     * vertex shader:" << endl;
	cout << line << "         - create shader ";
	#endif

	// -------------
	// vertex shader
	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	assert(glGetError() == GL_NO_ERROR);

	#if defined (DEBUG)
	cout << vertex << endl;
	cout << line << "         - set shader source" << endl;
	#endif

	glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
	assert(glGetError() == GL_NO_ERROR);

	#if defined (DEBUG)
	cout << line << "         - compile source" << endl;
	#endif

	glCompileShader(vertex);
	assert(glGetError() == GL_NO_ERROR);

	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	assert(glGetError() == GL_NO_ERROR);

	if (success == 0) {
		glGetShaderInfoLog(vertex, 512, nullptr, info_buf);
		cerr << "-------------------------------------" << endl;
		cerr << "shader::init - " << ERR << endl;
		cerr << "    Vertex shader compilation failed." << endl;
		cerr << "    Log: " << endl;
		cerr << info_buf << endl;
		cerr << "-------------------------------------" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << line << "     * fragment shader..." << endl;
	cout << line << "         - create shader ";
	#endif

	// ---------------
	// fragment shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	assert(glGetError() == GL_NO_ERROR);

	#if defined (DEBUG)
	cout << fragment << endl;
	cout << line << "         - set shader source" << endl;
	#endif

	glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
	assert(glGetError() == GL_NO_ERROR);

	#if defined (DEBUG)
	cout << line << "         - compile source" << endl;
	#endif

	glCompileShader(fragment);
	assert(glGetError() == GL_NO_ERROR);

	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	assert(glGetError() == GL_NO_ERROR);

	if (success == 0) {
		glGetShaderInfoLog(fragment, 512, nullptr, info_buf);
		cerr << "-------------------------------------" << endl;
		cerr << "shader::init - " << ERR << endl;
		cerr << "    Fragment shader compilation failed." << endl;
		cerr << "    Log: " << endl;
		cerr << info_buf << endl;
		cerr << "-------------------------------------" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << line << "     Creating program..." << endl;
	#endif

	// create shader program
	ID = glCreateProgram();
	assert(glGetError() == GL_NO_ERROR);

	glAttachShader(ID, vertex);
	assert(glGetError() == GL_NO_ERROR);

	glAttachShader(ID, fragment);
	assert(glGetError() == GL_NO_ERROR);

	glLinkProgram(ID);
	assert(glGetError() == GL_NO_ERROR);

	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	assert(glGetError() == GL_NO_ERROR);

	if (success == 0) {
		glGetProgramInfoLog(ID, 512, nullptr, info_buf);
		cerr << "-------------------------------------" << endl;
		cerr << "shader::init - " << ERR << endl;
		cerr << "    Shader program linkage failed." << endl;
		cerr << "    Log: " << endl;
		cerr << info_buf << endl;
		cerr << "-------------------------------------" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << line << "         program created: " << ID << endl;
	#endif

	// delete shaders as they're linked into
	// the program now and no longer necessery
	glDeleteShader(vertex);
	assert(glGetError() == GL_NO_ERROR);

	glDeleteShader(fragment);
	assert(glGetError() == GL_NO_ERROR);

	return true;
}

void shader::clear() {
	if (ID != 0) {
		#if defined(DEBUG)
		cout << line << " shader::clear() - deleting shader program " << ID << endl;
		#endif

		glDeleteProgram(ID);
		assert(glGetError() == GL_NO_ERROR);

		ID = 0;
	}
}

// SETTERS

void shader::set_bool(const string& name, bool value) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform1i(loc, value);
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_int(const string& name, int value) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform1i(loc, value);
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_float(const string& name, float value) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform1f(loc, value);
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_vec2(const string& name, const glm::vec2& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform2f(loc, v.x, v.y);
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_vec3(const string& name, const glm::vec3& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(loc, v.x, v.y, v.z);
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_vec4(const string& name, const glm::vec4& v) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform4fv(loc, 1, glm::value_ptr(v));
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_mat3(const string& name, const glm::mat3& m, bool transpose) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix3fv(loc, 1, transpose, glm::value_ptr(m));
	assert(glGetError() == GL_NO_ERROR);
}

void shader::set_mat4(const string& name, const glm::mat4& m, bool transpose) const {
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(m));
	assert(glGetError() == GL_NO_ERROR);
}

// GETTERS

GLuint shader::get_id() const {
	return ID;
}

// OTHERS

void shader::bind() const {
	glUseProgram(ID);
	assert(glGetError() == GL_NO_ERROR);
}

void shader::release() const {
	glUseProgram(0);
	assert(glGetError() == GL_NO_ERROR);
}
