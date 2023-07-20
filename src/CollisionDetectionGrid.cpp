#include "CollisionDetectionGrid.h"
#include <iostream>

CollisionDetectionGrid::CollisionDetectionGrid()
    : m_gridDimensions(glm::u16vec2(0.f)), m_gridSize(glm::vec2(0.f))
{ /* ... */ }

CollisionDetectionGrid::~CollisionDetectionGrid()
{ /* ... */ }

bool CollisionDetectionGrid::init()
{
    if (m_gridDimensions.x == 0 || m_gridDimensions.y == 0 || m_gridSize.x == 0 || m_gridSize.y == 0)
        return false;

    m_cells.resize(m_gridDimensions.x * m_gridDimensions.y);
    GLfloat fGridCols{ static_cast<GLfloat>(m_gridDimensions.x) };
    GLfloat fGridRows{ static_cast<GLfloat>(m_gridDimensions.y) };
    m_cellSize = { m_gridSize.x / fGridCols, m_gridSize.y / fGridRows };
    for (size_t i{0}; i < m_gridDimensions.y; i++)
    {
        for (size_t j{0}; j < m_gridDimensions.x; j++)
        {
            // Left, top, right, bottom.
            GLfloat left{ static_cast<GLfloat>(j) * m_cellSize.x };
            GLfloat top{ static_cast<GLfloat>(i) * m_cellSize.y };
            GLfloat right{ left + m_cellSize.x };
            GLfloat bottom{ top + m_cellSize.y };
            m_cells[i * m_gridDimensions.x + j].m_rect = { left, top, right, bottom };
        }
    }
    return true;
}

void CollisionDetectionGrid::storeCirclesIntoGridCells(std::shared_ptr<std::vector<CircleObject>>& circles)
{
    std::for_each(m_cells.begin(), m_cells.end(), [&](CDGridCell& cell)
    {
        cell.m_circles.clear();
        std::for_each(circles->begin(), circles->end(), [&](CircleObject& circle)
        {
            if (circleAndCellOverlap(circle, cell))
                cell.m_circles.push_back(&circle);
        });
    });
}

void CollisionDetectionGrid::detectCollisions()
{
    std::for_each(m_cells.begin(), m_cells.end(), [&](CDGridCell& cell)
    {
        for (std::size_t i{0}; i < cell.m_circles.size(); i++)
        {
            if (cell.m_circles[i]->toBeDestroyed()) continue;
            for (std::size_t j{i+1}; j < cell.m_circles.size(); j++)
            {
                if (cell.m_circles[j]->toBeDestroyed()) continue;
                if (circlesOverlap(*(cell.m_circles[i]), *(cell.m_circles[j])))
                {
                    if (cell.m_circles[i]->mass() >= cell.m_circles[j]->mass())
                        cell.m_circles[i]->mergeInto(*(cell.m_circles[j]));
                    else
                    {
                        cell.m_circles[j]->mergeInto(*(cell.m_circles[i]));
                        break;
                    }
                }
            };
        }
    });
}

void CollisionDetectionGrid::resolveCollisions(std::shared_ptr<std::vector<CircleObject>>& circles) const
{
    circles->erase(std::remove_if(circles->begin(), circles->end(), [](const CircleObject& circle) -> bool
    {
        return circle.toBeDestroyed();
    }), circles->end());
}