#include "View.h"
#include <iostream>

View::View(const glm::vec2& dimensions)
    : m_position(0.0f, 0.0f), m_dimensions(dimensions), m_aspectRatio(dimensions.x / dimensions.y), m_zoom(1.0f)
{ /* ... */ }

View::View(const View& view)
{
    *this = view;
}

View::~View()
{ /* ... */}

glm::mat4 View::viewMatrix()
{
    glm::mat4 viewMat{ glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f)) };
    viewMat = glm::scale(viewMat, glm::vec3(m_zoom, m_zoom, 1.0f));
    return viewMat;
}

glm::mat4 View::projectionMatrix(const glm::vec2& dimensions)
{
    return glm::ortho(m_position.x - dimensions.x,
                    m_position.x + dimensions.x,
                    m_position.y - dimensions.y,
                    m_position.y + dimensions.y,
                    -1.0f,
                    1.0f);
}

void View::setZoom(const GLfloat& zoom)
{
    m_zoom = std::min(50.0f, std::max(0.1f, zoom));
}

void View::setAspectRatio(const GLfloat& aspectRatio)
{

}

void View::setPosition(const glm::vec2& position)
{
    m_position = position;
}

void View::move(const glm::vec2& dpos)
{
    m_position += dpos;
}

void View::zoom(const GLfloat& zoom)
{
    setZoom(m_zoom + 0.05f * zoom);
}