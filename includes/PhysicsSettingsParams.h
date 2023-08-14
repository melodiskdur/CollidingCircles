#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

struct PhysicsSettingsParams
{
    PhysicsSettingsParams(GLfloat* gravityConstantRef) : m_gravityConstantRef(gravityConstantRef)
    { /* ... */ };

    // Parameters.
    GLfloat*                        m_gravityConstantRef{};

    // Limits & constants.
    static constexpr GLfloat        GRAVITY_CONSTANT_MIN{ 6.674e-11f };
    static constexpr GLfloat        GRAVITY_CONSTANT_MAX{ 6.674e0f };
};