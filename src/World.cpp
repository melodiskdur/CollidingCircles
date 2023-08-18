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
	m_physicsManager->init();
}

void World::updateWorldState(const float& timeStep)
{
	m_physicsManager->handlePhysics(m_circles, timeStep);
}

void World::moveInsert(std::vector<CircleObject>& circles)
{
    m_circles->insert(m_circles->end(), std::make_move_iterator(circles.begin()), std::make_move_iterator(circles.end()));
    // circles.erase(circles.begin(), circles.end());
    circles.clear();
}

void World::setWorldDimensions(const glm::vec2& dimensions)
{
    m_worldDimensions = { std::max(dimensions.x, DIMENSIONS_MIN.x),
                          std::max(dimensions.y, DIMENSIONS_MIN.y) };
}