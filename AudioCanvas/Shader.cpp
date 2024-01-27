#include "Shader.h"

Shader::Shader() {
	mShaderID = -1;
	mShaderType = GL_ZERO;
}

Shader::Shader(std::string filename) {
	LoadFromFile(filename);
	Compile();
}

void Shader::LoadFromFile(std::string filename) {
	std::ifstream shaderFile;

	shaderFile.open(filename);

	std::stringstream shaderStream;

	shaderStream << shaderFile.rdbuf();

	shaderFile.close();

	mShaderCode = shaderStream.str();

	mShaderType = GrabShaderExtension(filename);
}

void Shader::Compile() {
	mShaderID = glCreateShader(mShaderType);

	const char* tempCode = mShaderCode.c_str();

	glShaderSource(mShaderID, 1, &tempCode, nullptr);
	
	glCompileShader(mShaderID);

	CheckCompileErrors();
}

GLenum Shader::GrabShaderExtension(std::string filename) {
	std::string extension = filename.substr(filename.rfind('.') + 1);

	if (extension == "comp")
		return GL_COMPUTE_SHADER;
	else if (extension == "vert")
		return GL_VERTEX_SHADER;
	else if (extension == "tesc")
		return GL_TESS_CONTROL_SHADER;
	else if (extension == "tese")
		return GL_TESS_EVALUATION_SHADER;
	else if (extension == "geom")
		return GL_GEOMETRY_SHADER;
	else if (extension == "frag")
		return GL_FRAGMENT_SHADER;
	else
		throw std::runtime_error("Failed to find shader type.");

	return GL_ZERO;
}

void Shader::CheckCompileErrors() {
	int success;
	char infoLog[1024];
	
	glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(mShaderID, 1024, nullptr, infoLog);
		throw std::runtime_error("Failed to compile shader: " + std::string(infoLog));
	}
}