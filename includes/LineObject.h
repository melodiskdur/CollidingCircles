#pragma once

#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class LineObject
{
public:
    LineObject(glm::vec2&& origin, glm::vec2&& end, glm::vec3&& color);
    ~LineObject();

    inline glm::vec3 color() const { return m_color; };
    glm::mat4 modelMatrix() const;

    constexpr void setLine(glm::vec2&& origin, glm::vec2&& end) { m_origin = origin; m_end = end; };

private:
    glm::vec2                           m_origin{ };
    glm::vec2                           m_end{ };
    glm::vec3                           m_color{ };

};