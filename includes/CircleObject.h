#pragma once

#include <algorithm>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "CircleRenderData.h"

class CircleObject
{
public:
    CircleObject();
    CircleObject(glm::vec2&& pos);
    CircleObject(glm::vec2&& pos, glm::vec3&& color);
    CircleObject(glm::vec2&& pos, glm::vec3&& color, glm::vec2&& initialVelocity);
    ~CircleObject();

    void mergeInto(CircleObject& collided);
    constexpr void addForce(glm::vec2&& F) { m_force += F; }

    constexpr void setPos(glm::vec2&& pos)          { m_pos = std::move(pos); }
    constexpr void setPrevPos(glm::vec2&& pos)      { m_prevPos = std::move(pos); }
    constexpr void setRadius(GLfloat&& r)           { m_radius = std::move(r); }
    constexpr void setColor(glm::vec3&& c)          { m_color = std::move(c); }
    constexpr void setMass(GLfloat&& m)             { m_mass = std::move(m); }
    constexpr void setForce(glm::vec2&& F)          { m_force = std::move(F); }
    constexpr void setVelocityVec(glm::vec2&& v)    { m_velocityVec = std::move(v); }
    constexpr void setToDestroy()                   { m_toDestroy = true; }
    constexpr void setStationary(const bool& b)     { m_stationary = b; }

    constexpr glm::vec2 pos() const                 { return m_pos; }
    constexpr glm::vec2 prevPos() const             { return m_prevPos; }
    constexpr GLfloat radius() const                { return m_radius; }
    constexpr glm::vec3 color() const               { return m_color; }
    constexpr glm::vec2 force() const               { return m_force; }
    constexpr GLfloat mass() const                  { return m_mass; }
    constexpr glm::vec2 velocityVec() const         { return m_velocityVec; }
    constexpr bool toBeDestroyed() const            { return m_toDestroy; }
    constexpr bool isStationary() const             { return m_stationary; }

private:
    glm::vec2                           m_pos{ GLfloat(0.0f) };
    glm::vec2                           m_prevPos{ GLfloat(0.0f) };
    glm::vec2                           m_velocityVec{ GLfloat(0.0f) };
    GLfloat                             m_radius{ 1.0f };
    GLfloat                             m_scale{ 1.0f };
    glm::vec3                           m_color{ GLfloat(0.0f) };
    GLfloat                             m_mass{ 1.0f };
    glm::vec2                           m_force{ 0.0f };
    bool                                m_toDestroy{ false };
    bool                                m_stationary{ false };
};