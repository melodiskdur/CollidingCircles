#include "PhysicsManager.h"

#include <iostream>

PhysicsManager::PhysicsManager()
{ /* ... */ }

PhysicsManager::~PhysicsManager()
{ /* ... */ }

void PhysicsManager::init()
{
	// CollisionDetection.
	m_collisionDetection = std::make_shared<CollisionDetectionGrid>();
	m_collisionDetection->setGridSize(glm::vec2(100000.f));
	m_collisionDetection->setGridDimensions({ 50, 50 });
	m_collisionDetection->init();
    // CircleQuadTree.
    m_quadTree = std::make_shared<CircleQuadTree>(glm::vec2(0.f), 10.f * 1000.f);
    // Gravity.
	m_gravity = std::make_shared<GravityCalculator>();
	m_gravity->setGravitationalConstant(6.674e-4f);
    m_gravity->setQuadTree(m_quadTree);
    // QuadTreeVisualizer.
    m_quadVisualizer = std::make_shared<CircleQuadVisualizer>();
    m_quadVisualizer->setRootNode(m_quadTree->root());
}

void PhysicsManager::handlePhysics(std::shared_ptr<std::vector<CircleObject>> circles, const float& timeStep)
{
    // Build the quad tree.
    m_quadTree->clearAll();
    m_quadTree->insertAll(circles);
    // Calculate centers of mass in the quad tree.
    m_quadTree->root()->calculateCenterOfMass();
    // Gravity.
	m_gravity->applyForces(circles);
	m_gravity->updateVelAndPos(circles, timeStep);
    // Collision detection.
	m_collisionDetection->storeCirclesIntoGridCells(circles);
	m_collisionDetection->detectCollisions();
}