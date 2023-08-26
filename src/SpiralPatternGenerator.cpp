#include "SpiralPatternGenerator.h"

#include <time.h>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <iostream>

SpiralPatternGenerator::SpiralPatternGenerator() :
    m_params(std::make_shared<SpiralPatternParams>()), m_rgen(std::chrono::system_clock::now().time_since_epoch().count())
{ /* ... */ }

SpiralPatternGenerator::SpiralPatternGenerator(const SpiralPatternGenerator& spg)
{
    m_params = spg.m_params;
}

SpiralPatternGenerator::~SpiralPatternGenerator()
{ /* ... */ }

void SpiralPatternGenerator::generate()
{
    GLfloat deltaDegRot{ 360.f / static_cast<GLfloat>(m_params->m_numMainArms) };
    for (GLsizei i{ 0 }; i < m_params->m_numMainArms; i++)
    {
        GLfloat degStep{ m_params->m_armMaxDeg / static_cast<GLfloat>(m_params->m_numCirclesPerMainArm) };
        generateMainArm(m_params->m_initialRotationDeg + i * deltaDegRot, degStep);
    }

    GLfloat deltaDegRot2{ 360.f / static_cast<GLfloat>(m_params->m_numSubArms) };
    for (GLsizei i{ 0 }; i < m_params->m_numSubArms; i++)
    {
        GLfloat degStep{ m_params->m_armMaxDeg / static_cast<GLfloat>(m_params->m_numCirclesPerSubArm) };
        generateSubArm(m_params->m_initialRotationDeg * 0.5f + i * deltaDegRot2, degStep);
    }
}

void SpiralPatternGenerator::generateCenter()
{

}

void SpiralPatternGenerator::generateMainArm(const GLfloat& initialRotationDeg, const GLfloat& degStep)
{
    GLfloat rotation{ m_params->m_clockwiseRotation ? -1.0f : 1.0f };
    for (GLsizei i{ 0 }; i < m_params->m_numCirclesPerMainArm; i++)
    {
        GLfloat theta{ glm::radians(i * degStep) };
        GLfloat expFactor{ m_params->m_discRadius * glm::exp(m_params->m_logConstant * theta) };
        std::uniform_real_distribution<float> coordDisplacement(-m_params->m_displacementFactor,
                                                                m_params->m_displacementFactor);
        glm::vec2 position{ m_center.x + expFactor * glm::cos(glm::radians(initialRotationDeg) + rotation * theta) + coordDisplacement(m_rgen),
                            m_center.y + expFactor * glm::sin(glm::radians(initialRotationDeg) + rotation * theta) + coordDisplacement(m_rgen) };
        glm::vec2 normalVector{ (position - m_center).y, -(position - m_center).x };
        GLfloat distance{ glm::length(position - m_center) };
        glm::vec2 velocity{ -rotation * normalVector / distance };
        m_generatorCallback(glm::vec2(position), glm::vec2(velocity));
    }
}

void SpiralPatternGenerator::generateSubArm(const GLfloat& initialRotationDeg, const GLfloat& degStep)
{
    GLfloat rotation{ m_params->m_clockwiseRotation ? -1.0f : 1.0f };
    for (GLsizei i{ 0 }; i < m_params->m_numCirclesPerSubArm; i++)
    {
        GLfloat theta{ glm::radians(i * degStep) };
        GLfloat expFactor{ m_params->m_discRadius * glm::exp(m_params->m_logConstant * theta) };
        std::uniform_real_distribution<float> coordDisplacement(-m_params->m_displacementFactor,
                                                                m_params->m_displacementFactor);
        glm::vec2 position{ m_center.x + expFactor * glm::cos(glm::radians(initialRotationDeg) + rotation * theta) + m_params->m_displacementFactor * coordDisplacement(m_rgen),
                            m_center.y + expFactor * glm::sin(glm::radians(initialRotationDeg) + rotation * theta) + m_params->m_displacementFactor * coordDisplacement(m_rgen) };
        glm::vec2 normalVector{ (position - m_center).y, -(position - m_center).x };
        GLfloat distance{ glm::length(position - m_center) };
        glm::vec2 velocity{ -rotation * normalVector / distance };
        m_generatorCallback(glm::vec2(position), glm::vec2(velocity));
    }
}