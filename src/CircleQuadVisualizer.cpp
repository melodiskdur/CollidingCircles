#include "CircleQuadVisualizer.h"

static constexpr glm::vec3 COLOR{ 0.0f, 0.0f, 1.0f };

CircleQuadVisualizer::CircleQuadVisualizer()
{ /* ... */ }

CircleQuadVisualizer::~CircleQuadVisualizer()
{ /* ... */ }

void CircleQuadVisualizer::addNodeLines()
{
    m_lines.clear();
    glm::vec2 topLeft{ m_quadRoot->m_center + 0.5f * m_quadRoot->m_size * glm::vec2(-1.f, 1.f) };
    glm::vec2 topRight{ m_quadRoot->m_center + 0.5f * m_quadRoot->m_size * glm::vec2(1.f, 1.f) };
    glm::vec2 bottomLeft{ m_quadRoot->m_center + 0.5f * m_quadRoot->m_size * glm::vec2(-1.f, -1.f) };
    glm::vec2 bottomRight{ m_quadRoot->m_center + 0.5f * m_quadRoot->m_size * glm::vec2(1.f, -1.f) };
    m_lines.push_back(LineObject(glm::vec2(bottomLeft), glm::vec2(bottomRight), glm::vec3(COLOR)));
    m_lines.push_back(LineObject(glm::vec2(bottomRight), glm::vec2(topRight), glm::vec3(COLOR)));
    m_lines.push_back(LineObject(glm::vec2(topRight), glm::vec2(topLeft), glm::vec3(COLOR)));
    m_lines.push_back(LineObject(glm::vec2(topLeft), glm::vec2(bottomLeft), glm::vec3(COLOR)));
    addNodeLinesRecursive(m_quadRoot, m_lines);
}

void CircleQuadVisualizer::addNodeLinesRecursive(std::shared_ptr<CircleQuadNode> node, std::vector<LineObject>& lines)
{
    if (node->m_childNodes[0] && node->m_num > 0)
    {
        // Recursion.
        addNodeLinesRecursive(node->m_childNodes[SUBNODE_INDEX::NW], lines);
        addNodeLinesRecursive(node->m_childNodes[SUBNODE_INDEX::NE], lines);
        addNodeLinesRecursive(node->m_childNodes[SUBNODE_INDEX::SW], lines);
        addNodeLinesRecursive(node->m_childNodes[SUBNODE_INDEX::SE], lines);
        // Horizontal.
        lines.push_back(LineObject(node->m_center + glm::vec2(-0.5f * node->m_size, 0.f),
                    node->m_center + glm::vec2(0.5f * node->m_size, 0.f),
                    glm::vec3(COLOR)));
        // Vertical.
        lines.push_back(LineObject(node->m_center + glm::vec2(0.f, -0.5f * node->m_size),
                    node->m_center + glm::vec2(0.f, 0.5f * node->m_size),
                    glm::vec3(COLOR)));
    }
}
