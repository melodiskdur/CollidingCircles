#include "World.h"

#include <time.h>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <iostream>

World::World() : m_circles(std::make_shared<std::vector<CircleObject>>())
{ /* ... */ }

World::World(const World& w)
{
    m_circles = w.m_circles;
}

World::~World()
{ /* ... */ }

void World::init()
{
	m_physicsManager = std::make_shared<PhysicsManager>();
	m_physicsManager->init(m_worldDimensions);
}

void World::updateWorldState(const float& timeStep)
{
	m_physicsManager->handlePhysics(m_circles, timeStep);
    clearDestroyedCircles();
}

void World::moveInsert(std::vector<CircleObject>& circles)
{
    m_circles->insert(m_circles->end(), std::make_move_iterator(circles.begin()), std::make_move_iterator(circles.end()));
    circles.clear();
}

void World::setWorldDimensions(const glm::vec2& dimensions)
{
    m_worldDimensions = { std::max(dimensions.x, DIMENSIONS_MIN.x),
                          std::max(dimensions.y, DIMENSIONS_MIN.y) };
}

void World::clearDestroyedCircles()
{
    m_circles->erase(std::remove_if(m_circles->begin(), m_circles->end(), [&](const CircleObject& circle) -> bool
    {
        return circle.toBeDestroyed() || !isWithinBoundaries(circle.pos());
    }), m_circles->end());
}