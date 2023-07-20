#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "CircleObject.h"

struct CDGridCell
{
    std::vector<CircleObject*>      m_circles{ };
    glm::vec4                       m_rect{ };
};

class CollisionDetectionGrid
{
public:
    CollisionDetectionGrid();
    ~CollisionDetectionGrid();

    bool init();
    void storeCirclesIntoGridCells(std::shared_ptr<std::vector<CircleObject>>& circles);
    void detectCollisions();
    void resolveCollisions(std::shared_ptr<std::vector<CircleObject>>& circles) const;

    constexpr void setGridDimensions(const glm::u16vec2& dim) { m_gridDimensions = dim; };
    constexpr void setGridSize(const glm::vec2& gridSize) { m_gridSize = gridSize; };

private:
    inline bool circleAndCellOverlap(const CircleObject& c, const CDGridCell& cell) const
    {
        glm::vec2 deltaPos{ c.pos().x - std::max(cell.m_rect.x, std::min(c.pos().x, cell.m_rect.z)),
                            c.pos().y - std::max(cell.m_rect.y, std::min(c.pos().y, cell.m_rect.w)) };
        return glm::dot(deltaPos, deltaPos) <= c.radius() * c.radius();
    };

    inline bool circlesOverlap(const CircleObject& c1, const CircleObject& c2) const
    {
        glm::vec2 deltaPos{ c1.pos() - c2.pos() };
        return glm::dot(deltaPos, deltaPos) <= c1.radius() * c1.radius() + c2.radius() * c2.radius();
    }

private:
    std::vector<CDGridCell>         m_cells{};
    glm::u16vec2                    m_gridDimensions{ };
    glm::vec2                       m_gridSize{ };
    glm::vec2                       m_cellSize{ };
};