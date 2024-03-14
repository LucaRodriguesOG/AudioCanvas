#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "GlobalConstants.h"

class Shader {
public:
	Shader();
	Shader(std::vector<std::string>);
	~Shader();

	void AddShaders(std::vector<std::string>);
	void Compile();

	GLuint GetProgramID();
private:
	void LoadShader(std::string);
	GLenum GrabShaderExtension(std::string);
	void CheckCompilerErrors(GLuint, GLenum);

	GLuint mProgramID;
	std::vector<GLuint> mShaderIDs;

	std::vector<std::string> shaderFiles;
};