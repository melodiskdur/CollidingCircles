#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>

class TimeFlow
{
public:
    TimeFlow() { /* ... */ };
    TimeFlow(const TimeFlow& tf) { /* ... */ };
    ~TimeFlow() { /* ... */ };

    constexpr void setDeltaTime(const GLfloat& dt) { m_deltaTime =  std::max(0.0f, dt); };
    constexpr void resetTime() { m_time = 0.0f; };
    constexpr void updateTime() { m_time += m_deltaTime; };
    constexpr GLfloat currentTime() const { return m_time; };
    constexpr GLfloat deltaTime() const { return m_deltaTime; };

    GLfloat* timeReference() { return &m_time; };
    GLfloat* deltaTimeReference() { return &m_deltaTime; };

private:
    GLfloat                 m_time{};
    GLfloat                 m_deltaTime{};
};