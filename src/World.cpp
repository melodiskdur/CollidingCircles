#include "World.h"

#include <time.h>
#include <ctime>
#include <algorithm>
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
	// Gravity.
	m_gravity = std::make_shared<GravityCalculator>();
	m_gravity->setGravitationalConstant(6.674e2f);

	// CollisionDetection.
	m_collisionDetection = std::make_shared<CollisionDetectionGrid>();
	m_collisionDetection->setGridSize(m_worldDimensions);
	m_collisionDetection->setGridDimensions({ 20, 20 });
	m_collisionDetection->init();
}

void World::updateWorldState(const float& timeStep)
{
	// Physics.
	m_gravity->applyForces(m_circles);
	m_gravity->updateVelAndPos(m_circles, timeStep);
	m_collisionDetection->storeCirclesIntoGridCells(m_circles);
	m_collisionDetection->detectCollisions();
	m_collisionDetection->resolveCollisions(m_circles);
}

void World::setWorldDimensions(const glm::vec2& dimensions)
{
    m_worldDimensions = { std::max(dimensions.x, DIMENSIONS_MIN.x),
                          std::max(dimensions.y, DIMENSIONS_MIN.y) };
}