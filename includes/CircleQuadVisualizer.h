#pragma once

#include <memory>
#include <vector>

#include <LineObject.h>
#include "CircleQuadNode.h"

class CircleQuadVisualizer
{
public:
    CircleQuadVisualizer();
    ~CircleQuadVisualizer();

    void addNodeLines();

    inline void setRootNode(std::shared_ptr<CircleQuadNode> node) { m_quadRoot = node; };
    inline std::vector<LineObject>& lines() { return m_lines; };

private:
    void addNodeLinesRecursive(std::shared_ptr<CircleQuadNode> node, std::vector<LineObject>& lines);

private:
    std::shared_ptr<CircleQuadNode>             m_quadRoot{};
    std::vector<LineObject>                     m_lines{};
};