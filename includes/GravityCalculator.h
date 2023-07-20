#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "CircleObject.h"

class GravityCalculator
{
public:
    GravityCalculator();
    ~GravityCalculator();

    constexpr void setGravitationalConstant(const GLfloat& G) { m_G = G; };

    void applyForces(std::shared_ptr<std::vector<CircleObject>>& circles) const;
    void updateVelAndPos(std::shared_ptr<std::vector<CircleObject>>& circles, const GLfloat& dt) const;

private:
    void bruteForceAlgorithm(std::shared_ptr<std::vector<CircleObject>>& circles) const;
    void verlet(CircleObject& circle, const GLfloat& dt) const;
private:
    GLfloat             m_G{ };

};