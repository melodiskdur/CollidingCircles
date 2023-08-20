#pragma once

#include <string>
#include <memory>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "IPatternGenerator.h"
#include "SpiralPatternParams.h"

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