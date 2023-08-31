
#include <fstream>
#include <iostream>

#include <glad/glad.h>

#include "core/shader.h"


core::Shader::Shader()
	:id{},mAttributeCache{},mUniformCache{}
{

}

core::Shader::~Shader()
{
	glDeleteProgram(id);
}

bool core::Shader::init(const char* shaderDirectory,const char* vertexShaderFile, const char* fragmentShaderFile)
{

    std::pair<bool,GLuint> pair = compileShader(shaderDirectory,vertexShaderFile, GL_VERTEX_SHADER);

    if (!pair.first) return false;
    GLuint vertexShaderId = pair.second;
    pair = compileShader(shaderDirectory,fragmentShaderFile, GL_FRAGMENT_SHADER);
    if (!pair.first) return false;
    GLuint fragShaderId = pair.second;



    id = glCreateProgram();
    glAttachShader(id, vertexShaderId);
    glAttachShader(id, fragShaderId);

    //Optionally specify which framebuffer color output goes to
    //Do before linking
    glBindFragDataLocation(id, 0, "outColor");

    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char buffer[512];
        glGetProgramInfoLog(id, 512, NULL, buffer);
        std::cerr << "Error: Couldn't link shaders files (" << vertexShaderFile << "," << fragmentShaderFile << ")" << '\n' << buffer << '\n';
        return false;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragShaderId);

    return true;
}

void core::Shader::use()
{
    glUseProgram(id);
}

GLuint core::Shader::getId() const
{
    return id;
}

GLint core::Shader::getAttributeLocation(const char* str)
{
    auto it = mAttributeCache.find(str);
    if (it != mAttributeCache.end()) {
        return it->second;
    }

    GLint loc = glGetAttribLocation(id, str);
    mAttributeCache.insert(std::make_pair(str, loc));
    return loc;
}

GLint core::Shader::getUniformLocation(const char* str)
{
    auto it = mUniformCache.find(str);
    if (it != mUniformCache.end()) {
        return it->second;
    }

    GLint loc = glGetUniformLocation(id, str);
    mUniformCache.insert(std::make_pair(str, loc));
    return loc;
}

void core::Shader::enableVertexAttributeArray(const char* str)
{
    glEnableVertexAttribArray(getAttributeLocation(str));
}

void core::Shader::setUniformBool(const char* str, bool value)
{
    glUniform1i(getUniformLocation(str), value);
}

void core::Shader::setUniformInt(const char* str, int value)
{
    glUniform1i(getUniformLocation(str), value);
}

void core::Shader::setUniformFloat(const char* str,float value)
{
    glUniform1f(getUniformLocation(str), value);
}

void core::Shader::setUniformFloat2(const char* str, float valueX, float valueY){

    glUniform2f(getUniformLocation(str), valueX,valueY);
}

void core::Shader::setUniformFloat3(const char* str, float valueX, float valueY,float valueZ)
{
    glUniform3f(getUniformLocation(str), valueX, valueY, valueZ);
}

void core::Shader::setUniformFloat3(const char* str, const math::Vec3& v)
{
    glUniform3f(getUniformLocation(str), v[0], v[1], v[2]);
}

void core::Shader::setUniformFloat4(const char* str, float valueX, float valueY, float valueZ, float valueW)
{
    glUniform4f(getUniformLocation(str), valueX, valueY, valueZ,valueW);
}

void core::Shader::setUniformFloat4(const char* str, const math::Vec4& v)
{
    glUniform4f(getUniformLocation(str), v[0], v[1], v[2], v[3]);
}

void core::Shader::setUniformMat4(const char* str, const float* value)
{
    glUniformMatrix4fv(getUniformLocation(str), 1, GL_FALSE, value);
}


std::pair<bool,GLuint> core::Shader::compileShader(const char * shaderDirectory,const char* shaderFile, GLuint shaderType)
{
    if ((shaderType != GL_VERTEX_SHADER) && (shaderType != GL_FRAGMENT_SHADER)) return std::make_pair(false, 0);

    std::string fullFilePath(shaderDirectory);
    fullFilePath += shaderFile;

    std::ifstream file(fullFilePath);
    std::string shaderStr{""};

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            shaderStr += (line + '\n');
        }
    }
    else {
        std::cerr << "Error: couldn't open shader file: " << shaderFile << '\n';
        return std::make_pair(false,0);
    }

    const GLchar* shaderData = (const GLchar* )shaderStr.c_str();

    GLint status;
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1,&shaderData, nullptr);

    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glad_glGetShaderInfoLog(shaderId, 512, nullptr, buffer);
        std::cerr << "Error: Couldn't compile shader " << shaderFile << '\n' << buffer << '\n';
        return std::make_pair(false, 0);
    }

    return std::make_pair(true,shaderId);
}
