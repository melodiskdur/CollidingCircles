#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct CircleRenderData
{
    glm::vec2   m_pos{};
    glm::vec3   m_color{};
    GLfloat     m_radius{};
};