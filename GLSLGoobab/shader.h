#ifndef SHADER_H
#define SHADER_H



#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

static std::string get_file_contents(const char* filename);

class Shader
{		

	private:
		GLuint ID;

	public:
		
		Shader(const char* vertexFile, const char* fragmentFile);
		Shader() = default;

		GLuint getID() { return this->ID; }
		void Activate();
		void Delete();
};



#endif