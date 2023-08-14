#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class CircleCreatorSettings
{
public:
    CircleCreatorSettings() { /* ... */ };
    ~CircleCreatorSettings() { /*...*/ };

    constexpr glm::vec3 circleColor() const { return m_circleColor; };
    constexpr GLfloat circleRadius() const { return m_circleRadius; };
    constexpr GLfloat circleMass() const { return m_circleMass; };
    constexpr GLsizei numCircles() const { return m_numCircles; };
    constexpr GLfloat stationaryChecked() const { return m_stationaryChecked; };

    glm::vec3* circleColorRef() { return &m_circleColor; };
    GLfloat* circleRadiusRef() { return &m_circleRadius; };
    GLfloat* circleMassRef() { return &m_circleMass; };
    GLsizei* numCirclesRef() { return &m_numCircles; };
    bool* stationaryCheckedRef() { return &m_stationaryChecked; };

    // Limits.
    static constexpr GLfloat CIRCLE_RADIUS_MIN{ 0.5f };
    static constexpr GLfloat CIRCLE_RADIUS_MAX{ 100.f };

    static constexpr GLfloat CIRCLE_MASS_MIN{ 1.f };
    static constexpr GLfloat CIRCLE_MASS_MAX{ 200.f };

private:
    glm::vec3           m_circleColor{ 0.99f, 0.2f, 0.0f };
    GLfloat             m_circleRadius{ 2.0f };
    GLfloat             m_circleMass{ 2.0f };
    GLsizei             m_numCircles{ 0 };
    bool                m_stationaryChecked{ false };
};