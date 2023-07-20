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


void GravityCalculator::applyForces(std::shared_ptr<std::vector<CircleObject>>& circles) const
{
    bruteForceAlgorithm(circles);
}

void GravityCalculator::updateVelAndPos(std::shared_ptr<std::vector<CircleObject>>& circles, const GLfloat& dt) const
{
    std::for_each(circles->begin(), circles->end(), [&](CircleObject& circle) { verlet(circle, dt); });
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

void GravityCalculator::verlet(CircleObject& circle, const GLfloat& dt) const
{
    if (circle.isStationary())
        return;
    glm::vec2 prevPos{ circle.prevPos() };
    glm::vec2 pos{ circle.pos() };
    circle.setPos(pos + dt * circle.velocityVec() + 0.5f * circle.force() / circle.mass() * dt * dt);
    circle.setPrevPos(std::move(pos));
    circle.setVelocityVec( (circle.pos() - circle.prevPos()) / dt );
}