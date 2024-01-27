#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
	Shader();
	Shader(std::string);

	void LoadFromFile(std::string);
	void Compile();
private:
	GLenum GrabShaderExtension(std::string);
	void CheckCompileErrors();

	GLuint mShaderID;
	GLenum mShaderType;
	std::string mShaderCode;
};