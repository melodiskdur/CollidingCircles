#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "CircleObject.h"

class World
{
public:
    World();
    World(const World& w);
    ~World();

    void setWorldDimensions(const glm::vec2& dimensions);

    std::shared_ptr<std::vector<CircleObject>> circles() { return m_circles; }
    constexpr glm::vec2 worldDimensions() const { return m_worldDimensions; };
    constexpr glm::vec2 worldCenter() const { return 0.5f * m_worldDimensions; };

    void generateCircles(const std::size_t& n);

private:
    constexpr bool isWithinBoundaries(const glm::vec2& pos) const
    {
        return (pos.x > 0.0f && pos.x < m_worldDimensions.x) &&
               (pos.y > 0.0f && pos.y < m_worldDimensions.y);
    };

public:
    glm::vec2                       DIMENSIONS_MIN{ 50.f };

private:
    glm::vec2                                       m_worldDimensions{};
    std::shared_ptr<std::vector<CircleObject>>      m_circles{};
};