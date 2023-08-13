#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "CircleObject.h"
#include "CircleQuadNode.h"

class CircleQuadTree
{
public:
    CircleQuadTree(const glm::vec2& topLeft, const float& size);
    ~CircleQuadTree();

    void insertAll(std::shared_ptr<std::vector<CircleObject>>& circles);
    void clearAll();

    std::shared_ptr<CircleQuadNode> root() const { return m_root; };

private:
    bool insert(std::shared_ptr<CircleQuadNode> node, CircleObject* circle);
    void clear(std::shared_ptr<CircleQuadNode> node, size_t level);

private:
    float                               m_rootSize{};
    glm::vec2                           m_rootTopLeft{};
    std::shared_ptr<CircleQuadNode>     m_root{};
};