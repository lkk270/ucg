#pragma once

////////////////////////////////////////////////////////////////////////////////
#include <glad/glad.h>
#include <Eigen/Core>
#include <vector>
#include <string>
////////////////////////////////////////////////////////////////////////////////

class VertexArrayObject { //Object stores conenctions between program and data on gpu
public:
	unsigned int id;

	VertexArrayObject() : id(0) { }

	// Create a new VAO
	void init();

	// Select this VAO for subsequent draw calls
	void bind();

	// Release the id
	void free();
};

// -----------------------------------------------------------------------------

class VertexBufferObject { //creates buffer on gpu
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	GLuint id; 
	GLuint rows;
	GLuint cols;

	VertexBufferObject() : id(0), rows(0), cols(0) { } //consturctor initializes everything to 0

	// Create a new empty VBO
	void init(); //calls GLuint bufer

	// Updates the VBO with a matrix M //converts eigen matrix into a video
	void update(const Eigen::MatrixXf& M);

	// Select this VBO for subsequent draw calls
	void bind(); //binds - uses the buffer

	// Release the id
	void free();
};

// -----------------------------------------------------------------------------

// This class wraps an OpenGL program composed of two shaders
class Program {
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	//3 gpu objects we need
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program_shader;

	Program() : vertex_shader(0), fragment_shader(0), program_shader(0) { } 

	// Create a new shader from the specified source strings
	bool init(const std::string &vertex_shader_string,
		const std::string &fragment_shader_string,
		const std::string &fragment_data_name);

	// Select this shader for subsequent draw calls
	void bind(); 

	// Release all OpenGL objects
	void free(); //dealocates stuff on gpu

	// Return the OpenGL handle of a named shader attribute (-1 if it does not exist)
	GLint attrib(const std::string &name) const;

	// Return the OpenGL handle of a uniform attribute (-1 if it does not exist)
	GLint uniform(const std::string &name) const;

	// Bind a per-vertex array attribute
	GLint bindVertexAttribArray(const std::string &name, VertexBufferObject& VBO) const;

	GLuint create_shader_helper(GLint type, const std::string &shader_string);
};

////////////////////////////////////////////////////////////////////////////////

// From: https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__) //injects name of file and current line