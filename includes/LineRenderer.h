#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "LineObject.h"
#include "ShaderProgram.h"

class LineRenderer
{
public:
    LineRenderer();
    ~LineRenderer();

    void init();
    void render(const std::vector<LineObject>& lines, const glm::mat4& viewProjectionMatrix);
    std::shared_ptr<ShaderProgram> shaderProgram() const { return m_shaderProgram; };

public:
    static const GLsizei POINTS_SIZE{ 4 };

private:
    GLuint                                  m_vao{ 0 };
    GLuint                                  m_vbo{ 0 };
    GLuint                                  m_ebo{ 0 };
    std::shared_ptr<ShaderProgram>          m_shaderProgram{ };

    std::array<GLfloat, POINTS_SIZE>    m_points
    {
        -0.5f, 0.0f,
         0.5f, 0.0f
    };
};