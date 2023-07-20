#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class View
{
public:
    View(const glm::vec2& dimensions);
    View(const View& view);
    ~View();

    constexpr const GLfloat zoomVal() const { return m_zoom; };
    constexpr const GLfloat aspectRatio() const { return m_aspectRatio; };
    constexpr const glm::vec2 position() const { return m_position; };
    constexpr const glm::vec2 dimensions() const { return m_dimensions; };
    constexpr const glm::vec4 viewRect() { return glm::vec4(m_position, m_dimensions); };
    glm::mat4 viewMatrix();
    glm::mat4 projectionMatrix(const glm::vec2& dimensions);

    void setZoom(const GLfloat& zoom);
    void setAspectRatio(const GLfloat& aspectRatio);
    void setPosition(const glm::vec2& position);
    void move(const glm::vec2& dpos);
    void zoom(const GLfloat& zoom);

private:
    glm::vec2                   m_position{};
    glm::vec2                   m_dimensions{};
    GLfloat                     m_zoom{};
    GLfloat                     m_aspectRatio{};
};