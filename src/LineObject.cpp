#include "LineObject.h"

#include <cmath>
#include <iostream>

LineObject::LineObject(glm::vec2&& origin, glm::vec2&& end, glm::vec3&& color)
    : m_origin(std::move(origin)), m_end(std::move(end)), m_color(std::move(color))
{ /* ... */ }

LineObject::~LineObject()
{

}


glm::mat4 LineObject::modelMatrix() const
{
    // Translate.
    glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(0.5f * (m_origin + m_end), 0.0f)) };
    // Rotate.
    glm::vec2 lineDir{ glm::normalize(m_end - m_origin) };
    modelMatrix = glm::rotate(modelMatrix, std::atan2(lineDir.y, lineDir.x), glm::vec3(0.0f, 0.0f, 1.0f));
    // Scale.
    modelMatrix = glm::scale(modelMatrix, glm::vec3(glm::vec2(glm::length(m_end - m_origin)), 1.0f));
    return modelMatrix;
}