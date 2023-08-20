#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "CircleObject.h"
#include "CircleCreatorSettings.h"
#include "SpiralPatternGenerator.h"

class CircleCreator
{
public:
    CircleCreator();
    ~CircleCreator();

    void init();
    void generate(const glm::vec2& worldPosition, const glm::vec2& initialVelocity);
    inline std::vector<CircleObject>& newCircles() { return m_newCircles; };

    void setCircleCreatorSettings(std::shared_ptr<CircleCreatorSettings> ccs) { m_circleCreatorSettings = ccs; };

    std::shared_ptr<SpiralPatternGenerator> spiralPatternGen() { return m_spiralPatternGen; };

private:
    void generateSingleCircle(glm::vec2&& worldPosition, glm::vec2&& initialVelocity);

private:
    std::shared_ptr<CircleCreatorSettings>      m_circleCreatorSettings{ };
    std::vector<CircleObject>                   m_newCircles{ };
    std::shared_ptr<SpiralPatternGenerator>     m_spiralPatternGen{ };
};