#include "CircleQuadTree.h"

#include <iostream>

CircleQuadTree::CircleQuadTree(const glm::vec2& topLeft, const float& size)
    : m_rootSize(size), m_rootTopLeft(topLeft), m_root(std::make_shared<CircleQuadNode>())
{
    m_root->m_center = m_rootTopLeft + 0.5f * m_rootSize * glm::vec2(1.f);
    m_root->m_size = m_rootSize;
}

CircleQuadTree::~CircleQuadTree()
{ /* ... */ }

void CircleQuadTree::insertAll(std::shared_ptr<std::vector<CircleObject>>& circles)
{
    if (!m_root)
        return;

    std::for_each(circles->begin(), circles->end(), [&](CircleObject& c)
    {
        insert(m_root, &c);
    });
}

void CircleQuadTree::clearAll()
{
    clear(m_root, 0);
}

bool CircleQuadTree::insert(std::shared_ptr<CircleQuadNode> node, CircleObject* circle)
{
    if (!node) return false;
    node->m_num += 1;
    // Place circle in subnode if node is already occupied.
    if (node->isOccupied())
    {
        // Subdivide node if necessary.
        if (!node->m_childNodes[0])
            node->subdivide();
        // Recursive call with the appropriate subnode.
        return insert(node->m_childNodes[node->whichSubnodeIndex(circle->pos())], circle);
    }
    node->add(circle);
}

void CircleQuadTree::clear(std::shared_ptr<CircleQuadNode> node, size_t level)
{
    if (!node) return;
    node->reset();
    if (!node->m_childNodes[0]) return;
    for (auto& child : node->m_childNodes)
        clear(child, level + 1);
}