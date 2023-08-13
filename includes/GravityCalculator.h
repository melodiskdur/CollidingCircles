#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "CircleObject.h"
#include "CircleQuadNode.h"
#include "CircleQuadTree.h"

class GravityCalculator
{
public:
    GravityCalculator();
    ~GravityCalculator();

    constexpr void setGravitationalConstant(const GLfloat& G) { m_G = G; };
    inline void setQuadTree(std::shared_ptr<CircleQuadTree> quadTree) { m_quadTree = quadTree; };

    void applyForces(std::shared_ptr<std::vector<CircleObject>>& circles);
    void updateVelAndPos(std::shared_ptr<std::vector<CircleObject>>& circles, const GLfloat& dt) const;

private:
    void bruteForceAlgorithm(std::shared_ptr<std::vector<CircleObject>>& circles) const;
    void barnesHutAlgorithm(std::shared_ptr<std::vector<CircleObject>>& circles);
    void barnesHutAlgorithmRecursive(CircleObject* circle, const std::shared_ptr<CircleQuadNode> node);
    void verlet(CircleObject& circle, const GLfloat& dt) const;
private:
    std::shared_ptr<CircleQuadTree>     m_quadTree{ };
    GLfloat                             m_G{ };
    GLfloat                             m_barnesHutThreshold{ 1.f };
};