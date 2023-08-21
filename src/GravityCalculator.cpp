#include "GravityCalculator.h"

#include <algorithm>
#include <iostream>

constexpr GLfloat MIN_DISTANCE{ 1e1f };
constexpr GLfloat MAX_DISTANCE{ 1e5f };

GravityCalculator::GravityCalculator()
{

}

GravityCalculator::~GravityCalculator()
{

}


void GravityCalculator::applyForces(std::shared_ptr<std::vector<CircleObject>>& circles)
{
    // bruteForceAlgorithm(circles);
    barnesHutAlgorithm(circles);
}

void GravityCalculator::updateVelAndPos(std::shared_ptr<std::vector<CircleObject>>& circles, const GLfloat& dt) const
{
    std::for_each(circles->begin(), circles->end(), [&](CircleObject& circle) { verlet(circle, dt); });
    // std::cout << "--------------------------------------------------\n";
}

void GravityCalculator::bruteForceAlgorithm(std::shared_ptr<std::vector<CircleObject>>& circles) const
{
    std::for_each(circles->begin(), circles->end(), [&](CircleObject& current)
    {
        if (current.isStationary())
            return;
        glm::vec2 totalForceOnCurrent{ 0.0f };

        std::for_each(circles->begin(), circles->end(), [&](const CircleObject& other)
        {
            if (&current == &other) return;
            glm::vec2 deltaPos{ other.pos() - current.pos() };
            GLfloat r2{ glm::dot(deltaPos, deltaPos) };
            //if (r2 < MIN_DISTANCE) return;
            totalForceOnCurrent += (1.f/ std::max(std::min(r2, MAX_DISTANCE), MIN_DISTANCE) * m_G * current.mass() * other.mass()) * glm::normalize(deltaPos);
        });
        current.setForce(std::move(totalForceOnCurrent));
    });
}

void GravityCalculator::barnesHutAlgorithm(std::shared_ptr<std::vector<CircleObject>>& circles)
{
    if (!m_quadTree->root()) return;
    GLsizei i{0};
    for (CircleObject& circle: *circles)
    {
        if (circle.isStationary()) continue;
        circle.setForce(glm::vec2(0.f));
        barnesHutAlgorithmRecursive(&circle, m_quadTree->root());
        i++;
    }
}

void GravityCalculator::barnesHutAlgorithmRecursive(CircleObject* circle, const std::shared_ptr<CircleQuadNode> node)
{
    if (!node || node->m_num == 0) return;
    glm::vec2 forceDirection{ node->m_centerOfMass - circle->pos() };
    GLfloat distance{ glm::length(forceDirection) };
    if (node->m_num > 1)
    {
        // Barnes Hut conditional, i.e is the node "close enough" to the current circle such
        // that it warrants a more exact force approximation (through recursion).
        if (node->m_size / distance < m_barnesHutThreshold)
        {
            GLfloat forceMag{ m_G * circle->mass() * node->m_totalMass / (distance * distance) };
            circle->addForce(forceMag * forceDirection);
        }
        else
        {
            for (auto& childNode : node->m_childNodes)
                barnesHutAlgorithmRecursive(circle, childNode);
        }
    }
    // If there's only one particle in the region (and it's NOT equivalent to the current circle).
    else if (circle != node->m_circle)
    {
        GLfloat forceMag{ m_G * circle->mass() * node->m_totalMass / (distance * distance) };
        circle->addForce(forceMag * forceDirection);
    }
}

void GravityCalculator::verlet(CircleObject& circle, const GLfloat& dt) const
{
    if (circle.isStationary())
        return;
    glm::vec2 prevPos{ circle.prevPos() };
    glm::vec2 pos{ circle.pos() };
    circle.setPos(pos + dt * circle.velocityVec() + 0.5f * circle.force() / circle.mass() * dt * dt);
    circle.setPrevPos(std::move(pos));
    circle.setVelocityVec( (circle.pos() - circle.prevPos()) / dt );
    // std::cout << "Circle Force: " << glm::length(circle.force()) << " | Circle Velocity" << glm::length(circle.velocityVec()) << "\n";
}