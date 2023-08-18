#include "CircleCreator.h"

#include <iostream>
#include <functional>

CircleCreator::CircleCreator()
{ /* ... */ }

CircleCreator::~CircleCreator()
{ /* ... */ }

void CircleCreator::init()
{
    m_spiralPatternGen = std::make_shared<SpiralPatternGenerator>();
    m_spiralPatternGen->setGeneratorCallback([&](glm::vec2&& pos, glm::vec2&& vel)
    {
        generateSingleCircle(std::move(pos), std::move(vel));
    });
}

void CircleCreator::generate(const glm::vec2& worldPosition, const glm::vec2& initialVelocity)
{
    if (!m_circleCreatorSettings)
    {
        std::cerr << "CircleCreator::generate(): CircleCreatorSettings hasn't been set (see CircleCreator::setCircleCreatorSettings())\n";
        return;
    }

    switch (m_circleCreatorSettings->generatorType())
    {
        case 0:     generateSingleCircle(glm::vec2(worldPosition), glm::vec2(initialVelocity));
                    break;
        case 1:     m_spiralPatternGen->setCenter(worldPosition);
                    m_spiralPatternGen->generate();
                    break;
    };
}

void CircleCreator::generateSingleCircle(glm::vec2&& worldPosition, glm::vec2&& initialVelocity)
{
    CircleObject newCircle(std::move(worldPosition), m_circleCreatorSettings->circleColor(), std::move(initialVelocity));
    newCircle.setRadius(m_circleCreatorSettings->circleRadius());
    newCircle.setMass(m_circleCreatorSettings->circleMass());
    newCircle.setStationary(m_circleCreatorSettings->stationaryChecked());
    m_newCircles.push_back(std::move(newCircle));
}
