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
{

}

void World::setWorldDimensions(const glm::vec2& dimensions)
{
    m_worldDimensions = { std::max(dimensions.x, DIMENSIONS_MIN.x),
                          std::max(dimensions.y, DIMENSIONS_MIN.y) };
}

void World::generateCircles(const std::size_t& n)
{
    std::srand(std::time(0));
	m_circles->resize(n);
    std::size_t iDim_x{ static_cast<std::size_t>(0.5f * m_worldDimensions.x) };
    std::size_t iDim_y{ static_cast<std::size_t>(0.5f * m_worldDimensions.y) };
	std::transform(m_circles->cbegin(), m_circles->cend(), m_circles->begin(),
		[=](CircleObject c)
		{
			return CircleObject(glm::vec2(std::rand() % iDim_x, std::rand() % iDim_y));
		});
	int max{ 255 };
	std::for_each(m_circles->begin(), m_circles->end(),
		[=](CircleObject& c)
		{
			c.setColor(1.0f / max * glm::vec3(std::rand() % max, std::rand() % max, std::rand() % max));
			// c.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
			c.setRadius((std::rand() % 20) + 1);
			c.setMass(c.radius());
			GLfloat o{ static_cast<GLfloat>(std::rand() % 360) };
			c.setVelocityVec(5.f * glm::vec2(std::cos(o / (2 * 3.14159f)), std::sin(o / (2 * 3.14159f))));
			c.setPrevPos(c.pos() - c.velocityVec());
		});
}