#include "CircleObject.h"
#include <iostream>

CircleObject::CircleObject() { /* ... */ }


CircleObject::CircleObject(glm::vec2&& pos) : m_pos(pos), m_prevPos(pos)
{ /* ... */}


CircleObject::CircleObject(glm::vec2&& pos, glm::vec3&& color) : m_pos(pos), m_prevPos(pos), m_color(color)
{ /* ... */ }

CircleObject::CircleObject(glm::vec2&& pos, glm::vec3&& color, glm::vec2&& initialVelocity)
    : m_pos(pos), m_color(color), m_velocityVec(initialVelocity), m_prevPos(pos - initialVelocity)
{ /* ... */ }

CircleObject::~CircleObject() { /* ... */}

void CircleObject::mergeInto(CircleObject& collided)
{
    m_radius += 0.5f * collided.m_radius;
    if (!m_stationary)
        m_velocityVec += collided.m_mass * collided.m_velocityVec / m_mass;
    m_mass += collided.m_mass;
    m_color += 0.05f * glm::normalize(collided.m_color);
    collided.setToDestroy();
}