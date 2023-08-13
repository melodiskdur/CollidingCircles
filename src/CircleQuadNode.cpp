#include "CircleQuadNode.h"

#include <map>
#include <iostream>

void CircleQuadNode::calculateCenterOfMass()
{
    m_centerOfMass = glm::vec3(0.f);
    m_totalMass = 0.f;

    if (m_num == 0) return;

    if (m_num > 1)
    {
        for (auto& childNode : m_childNodes)
        {
            childNode->calculateCenterOfMass();
            m_totalMass += childNode->m_totalMass;
            m_centerOfMass += childNode->m_totalMass * childNode->m_centerOfMass;
        }
        m_centerOfMass /= m_totalMass;
    }
    else
    {
        m_totalMass = m_circle->mass();
        m_centerOfMass = m_circle->pos();
    }
}

void CircleQuadNode::subdivide()
{
    std::map<SUBNODE_INDEX, glm::vec2> centers
    {
        { SUBNODE_INDEX::NW, m_center + 0.25f * m_size * glm::vec2(-1.f, 1.f) },
        { SUBNODE_INDEX::NE, m_center + 0.25f * m_size * glm::vec2(1.f, 1.f)},
        { SUBNODE_INDEX::SW, m_center + 0.25f * m_size * glm::vec2(-1.f, -1.f)},
        { SUBNODE_INDEX::SE, m_center + 0.25f * m_size * glm::vec2(1.f, -1.f) }
    };

    for (const auto& subEntry : centers)
    {
        m_childNodes[subEntry.first] = std::make_shared<CircleQuadNode>();
        m_childNodes[subEntry.first]->m_center = subEntry.second;
        m_childNodes[subEntry.first]->m_size = 0.5f * m_size;
    }
}