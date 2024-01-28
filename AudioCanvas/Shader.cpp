#include "Shader.h"

Shader::Shader() {
    mProgramID = 0;
}

Shader::Shader(std::vector<std::string> shaders) {
    for (auto& filename : shaders) {
        shaderFiles.push_back(filename);
    }
    Compile();
}

Shader::~Shader() {
    glDeleteProgram(mProgramID);
}

void Shader::AddShaders(std::vector<std::string> shaders) {
    for (auto& filename : shaders) {
        shaderFiles.push_back(filename);
    }
}

void Shader::Compile() {
    for (auto& filename : shaderFiles) {
        LoadShader(filename);
    }

    mProgramID = glCreateProgram();

    for (auto shader : mShaderIDs) {
        glAttachShader(mProgramID, shader);
    }

    glLinkProgram(mProgramID);

    CheckCompilerErrors(mProgramID, GL_PROGRAM);

    for (auto shader : mShaderIDs) {
        glDeleteShader(shader);
    }
}

GLuint Shader::GetProgramID() {
    return mProgramID;
}

void Shader::LoadShader(std::string filename) {
    std::ifstream shaderFile;

    shaderFile.open(filename, std::ios::in);

    std::stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    std::string shaderCode = shaderStream.str();

    const char* shaderCodeC = shaderCode.c_str();

    GLenum mShaderType = GrabShaderExtension(filename);

    GLuint tempShaderID = glCreateShader(mShaderType);

    glShaderSource(tempShaderID, 1, &shaderCodeC, nullptr);

    glCompileShader(tempShaderID);

    CheckCompilerErrors(tempShaderID, GL_SHADER);

    mShaderIDs.push_back(tempShaderID);
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

void Shader::CheckCompilerErrors(GLuint id, GLenum type) {
    int success;
    char infoLog[1024];

    if (type == GL_PROGRAM) {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            throw std::runtime_error("Failed to link program - " + std::string(infoLog));
        }
    } else if (type == GL_SHADER) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            throw std::runtime_error("Failed to compile shader - " + std::string(infoLog));
        }
    }
}