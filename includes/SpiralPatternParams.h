#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct SpiralPatternParams
{
    // Params.
    GLsizei                 m_numCirclesPerMainArm{ 40 };
    GLsizei                 m_numCirclesPerSubArm{ 20 };
    GLfloat                 m_discRadius{ 200.f };
    GLsizei                 m_numMainArms{ 4 };
    GLsizei                 m_numSubArms{ 2 };
    GLfloat                 m_logConstant{ -0.35f };
    GLfloat                 m_fuzzFactor{ 1.4 };
    GLfloat                 m_initialRotationDeg{ 45.0f };
    GLfloat                 m_armMaxDeg{ 360.f };

    // Limits.
    static constexpr GLsizei      MAINARM_NUMCIRCLES_MIN{ 10 };
    static constexpr GLsizei      MAINARM_NUMCIRCLES_MAX{ 200 };
    static constexpr GLsizei      SUBARM_NUMCIRCLES_MIN{ 5 };
    static constexpr GLsizei      SUBARM_NUMCIRCLES_MAX{ 100 };
    static constexpr GLfloat      DISCRADIUS_MIN{ 50.0f };
    static constexpr GLfloat      DISCRADIUS_MAX{ 1000.0f };
    static constexpr GLsizei      MAINARMS_NUM_MAX{ 8 };
    static constexpr GLsizei      SUBARMS_NUM_MAX{ 8 };
    static constexpr GLfloat      LOG_CONSTANT_MIN{ -1.0f };
    static constexpr GLfloat      LOG_CONSTANT_MAX{ -0.05f };
    static constexpr GLfloat      FUZZ_FACTOR_MIN{ 0.4f };
    static constexpr GLfloat      FUZZ_FACOTR_MAX{ 2.0f };
    static constexpr GLfloat      PATTERN_ROTATION_MAX{ 180.f };
    static constexpr GLfloat      SPIRAL_DEG_MIN{ 180.f };
    static constexpr GLfloat      SPIRAL_DEG_MAX{ 720.f };
};