#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

enum class SIM_STATE { RUN, STOP, STEP };

struct FlowControlParams
{
    FlowControlParams(GLfloat* timeRef, GLfloat* timeStepRef) : m_timeRef(timeRef), m_timeStepRef(timeStepRef)
    { /* ... */ };

    GLfloat*                    m_timeRef{ nullptr };
    GLfloat*                    m_timeStepRef{ nullptr };
    SIM_STATE                   m_simState{ SIM_STATE::STOP };

    static constexpr GLfloat    TIMESTEP_MIN{ 0.05f };
    static constexpr GLfloat    TIMESTEP_MAX{ 5.f };
};