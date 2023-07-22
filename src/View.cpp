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

glm::mat4 View::viewProjectionMatrix()
{
    return glm::ortho(m_position.x,
                    m_position.x + m_dimensions.x / m_zoom,
                    m_position.y,
                    m_position.y + m_dimensions.y / m_zoom,
                    -1.0f,
                    1.0f);
}

void View::setZoom(const GLfloat& zoom)
{
    glm::vec2 dimBeforeZoom{ m_dimensions / m_zoom };
    glm::vec2 center{ m_position + 0.5f * dimBeforeZoom };
    m_zoom = std::min(50.0f, std::max(0.1f, m_zoom * zoom));
    glm::vec2 dimAfterZoom{ m_dimensions / m_zoom };
    glm::vec2 deltaDim{ dimAfterZoom - dimBeforeZoom };
    m_position = center - 0.5f * dimAfterZoom;
}

void View::setAspectRatio(const GLfloat& aspectRatio)
{

}

void View::setPosition(const glm::vec2& position)
{
    m_position = position;
}

void View::setDimensions(const glm::vec2& dimensions)
{
    m_dimensions = dimensions;
}

void View::move(const glm::vec2& dpos)
{
    m_position += dpos / m_zoom;
}

void View::zoom(const GLfloat& zoom)
{
    setZoom(zoom);
}

