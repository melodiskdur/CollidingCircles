#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

// Helper class to represent a single shader.
struct Shader
{
    Shader(const std::string& name, std::string&& source, const GLuint& id, const GLenum& type)
        : m_name(name), m_source(source), m_shaderid(id), m_type(type)
    { /* ... */}
    std::string             m_name{};
    std::string             m_source{};
    GLuint                  m_shaderid{};
    GLenum                  m_type{};
};

// For readability.
using ShaderPtr = std::shared_ptr<Shader>;

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    constexpr GLuint id() const { return m_programid; }
    void addShader(const std::string& name, const std::string& filePath, const GLenum& type);
    void combineShadersIntoPair(const std::string& vtx_name, const std::string& frag_name);
    void setCurrentShaderPair(const std::string& vtx_name, const std::string& frag_name);
    void linkAndUse();

    void updateUniform1i(const std::string& uniformName, const GLint& uniformData) const;
    void updateUniform1f(const std::string& uniformName, const GLfloat& uniformData) const;
    void updateUniform1fv(const std::string& uniformName, const std::vector<GLfloat>& uniformData) const;
    void updateUniform2fv(const std::string& uniformName, const std::vector<glm::vec2>& uniformData) const;
    void updateUniform3f(const std::string& uniformName, const glm::vec3& uniformData) const;
    void updateUniform3fv(const std::string& uniformName, const std::vector<glm::vec3>& uniformData) const;
    void updateUniformMatrix4fv(const std::string& uniformName, const glm::mat4& mat4) const;

private:
    std::string parse(const std::string& path);
    void checkCompileError(const std::string& name, const GLuint& id);
    std::vector<ShaderPtr>::iterator findShader(const std::string& name);

private:
    GLuint                                       m_programid{};
    std::vector<ShaderPtr>                       m_shaders{};
    std::pair<ShaderPtr, ShaderPtr>              m_currentShaderPair{};
    std::vector<std::pair<ShaderPtr, ShaderPtr>> m_shaderPairs{};
};