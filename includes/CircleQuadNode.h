#pragma once

#include <vector>
#include <array>
#include <memory>

#include "glm/glm.hpp"

#include "CircleObject.h"

enum SUBNODE_INDEX { NW = 0, NE, SW, SE }; // Northwest, northeast ...

struct CircleQuadNode
{
    void calculateCenterOfMass();
    void subdivide();
    inline bool contains(const glm::vec2& pos, const GLfloat& radius) const
    {
        GLfloat halfSize{ 0.5f * m_size };
        return ((pos.x >= m_center.x - halfSize && pos.x < m_center.x + halfSize) &&
                (pos.y >= m_center.y - halfSize && pos.y < m_center.y + halfSize));
    };

    constexpr SUBNODE_INDEX whichSubnodeIndex(const glm::vec2& pos) const
    {
        SUBNODE_INDEX index{ (pos.x > m_center.x) ? SUBNODE_INDEX::NE : SUBNODE_INDEX::NW };
        return (pos.y < m_center.y) ? (SUBNODE_INDEX)(index+2) : index;
    }
    constexpr void add(CircleObject* c) { m_circle = c; };
    constexpr void reset() { m_circle = nullptr; m_num = 0; };

    static constexpr GLsizei NODE_DEPTH_MAX{ 50 };
    glm::vec2                                       m_center{};
    GLfloat                                         m_size{};
    glm::vec2                                       m_centerOfMass{};
    GLfloat                                         m_totalMass{};
    CircleObject*                                   m_circle{};
    GLsizei                                         m_num{};
    std::array<std::shared_ptr<CircleQuadNode>, 4>  m_childNodes{};
    GLuint                                          m_level{0};
};