#include "PhysicsManager.h"

#include <iostream>

PhysicsManager::PhysicsManager()
{ /* ... */ }

PhysicsManager::~PhysicsManager()
{ /* ... */ }

void PhysicsManager::init(const glm::vec2& worldDimensions)
{
	// CollisionDetection.
	m_collisionDetection = std::make_shared<CollisionDetectionGrid>();
	m_collisionDetection->setGridSize(worldDimensions);
	m_collisionDetection->setGridDimensions({ 50, 50 });
	m_collisionDetection->init();
    // CircleQuadTree.
    m_quadTree = std::make_shared<CircleQuadTree>(glm::vec2(0.f), std::max(worldDimensions.x, worldDimensions.y));
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
    if (m_collisionDetectionEnabled)
    {
        m_collisionDetection->storeCirclesIntoGridCells(circles);
	    m_collisionDetection->detectCollisions();
    }
}