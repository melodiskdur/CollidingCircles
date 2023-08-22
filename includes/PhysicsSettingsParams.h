#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

struct PhysicsSettingsParams
{
    PhysicsSettingsParams(GLfloat* gravityConstantRef, bool* colDetRef) : m_gravityConstantRef(gravityConstantRef), m_collisionDetectionEnabledRef(colDetRef)
    { /* ... */ };

    // Parameters.
    GLfloat*                        m_gravityConstantRef{};
    bool*                           m_collisionDetectionEnabledRef{ nullptr };

    // Limits & constants.
    static constexpr GLfloat        GRAVITY_CONSTANT_MIN{ 6.674e-11f };
    static constexpr GLfloat        GRAVITY_CONSTANT_MAX{ 6.674e0f };
};