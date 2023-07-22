#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"

class GridRenderer
{
public:
    GridRenderer();
    ~GridRenderer();

    void init(glm::vec2&& gridDimensions);
    void initEBO();
    std::shared_ptr<ShaderProgram> shaderProgram() const { return m_shaderProgram; };
    void render();
    constexpr void toggleGrid(const bool& state) { m_gridEnabled = state; };
    constexpr bool isGridEnabled() { return m_gridEnabled; };

private:
    void generateGridVertices();

private:
    std::vector<GLfloat>                m_gridVertices{};
    std::vector<GLshort>                m_indices{};
    glm::vec2                           m_gridDimensions{};
    std::shared_ptr<ShaderProgram>      m_shaderProgram{};
    GLuint                              m_vao{};
    GLuint                              m_vbo{};
    GLuint                              m_ebo{};
    bool                                m_gridEnabled{true};
};