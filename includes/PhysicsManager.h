#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "CircleObject.h"
#include "GravityCalculator.h"
#include "CollisionDetectionGrid.h"
#include "CircleQuadTree.h"
#include "CircleQuadVisualizer.h"
#include "PhysicsSettingsParams.h"

class PhysicsManager
{
public:
    PhysicsManager();
    ~PhysicsManager();

    void init(const glm::vec2& worldDimensions);
    void handlePhysics(std::shared_ptr<std::vector<CircleObject>> circles, const float& timeStep);

    inline std::shared_ptr<GravityCalculator> gravityCalculator() { return m_gravity; };
    inline std::shared_ptr<CollisionDetectionGrid> collisionDetectionGrid() { return m_collisionDetection; };
    inline std::shared_ptr<CircleQuadTree> circleQuadTree() { return m_quadTree; };
    inline std::shared_ptr<CircleQuadVisualizer> circleQuadVisualizer() { return m_quadVisualizer; };

    bool* collisionDetectionEnabledRef() { return &m_collisionDetectionEnabled; };

private:
    std::shared_ptr<GravityCalculator>          m_gravity{};
    std::shared_ptr<CollisionDetectionGrid>     m_collisionDetection{};
    std::shared_ptr<CircleQuadTree>             m_quadTree{};
    std::shared_ptr<CircleQuadVisualizer>       m_quadVisualizer{};
    std::shared_ptr<PhysicsSettingsParams>      m_physicsSettingsParams{};
    bool                                        m_collisionDetectionEnabled{ true };
};