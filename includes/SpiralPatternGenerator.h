#pragma once

#include <string>
#include <memory>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "IPatternGenerator.h"

struct SpiralPatternParams
{
    GLsizei                 m_numCirclesPerMainArm{ 40 };
    GLsizei                 m_numCirclesPerSubArm{ 20 };
    GLfloat                 m_discRadius{ 200.f };
    GLsizei                 m_numMainArms{ 4 };
    GLsizei                 m_numSubArms{ 2 };
    GLfloat                 m_logConstant{ -0.35f };
    GLfloat                 m_fuzzFactor{ 1.4 };
    GLfloat                 m_initialRotationDeg{ 45.0f };
    GLfloat                 m_armMaxDeg{ 360.f };
};

class SpiralPatternGenerator : public IPatternGenerator
{
public:
    SpiralPatternGenerator();
    SpiralPatternGenerator(const SpiralPatternGenerator& spg);
    ~SpiralPatternGenerator();

    const char* name() const override { return m_name.c_str(); };
    void generate() override;

    constexpr void setCenter(const glm::vec2& center) { m_center = center; };
    void setGeneratorCallback(std::function<void(glm::vec2&& wp, glm::vec2&& vel)> callback) { m_generatorCallback = callback; };
    std::shared_ptr<SpiralPatternParams> spiralPatternParams() { return m_params; };

private:
    void generateCenter();
    void generateMainArm(const GLfloat& initialRotationDeg, const GLfloat& degStep);
    void generateSubArm(const GLfloat& initialRotationDeg, const GLfloat& degStep);

private:
    std::string                             m_name{ "Spiral Pattern Generator" };
    std::shared_ptr<SpiralPatternParams>    m_params{ };
    glm::vec2                               m_center{ };
    std::mt19937                            m_rgen{ };
};