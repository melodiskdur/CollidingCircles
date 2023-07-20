#include "CircleUniformBuffer.h"
#include <algorithm>
#include <iostream>
CircleUniformBuffer::CircleUniformBuffer()
    : m_ubo(0), m_bindingPoint(0), m_bufferSize(0)
{ /* ... */ }

CircleUniformBuffer::~CircleUniformBuffer()
{ /* ... */ }

void CircleUniformBuffer::init()
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
}

void CircleUniformBuffer::bindToPoint(const GLuint& bindingPoint)
{
    m_bindingPoint = bindingPoint;
    glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_ubo);
}

void CircleUniformBuffer::updateBufferData(const std::vector<CircleObject>& circles)
{
    m_bufferSize = circles.size() * sizeof(CircleRenderData);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    if (m_circleData.size() != circles.size())
        m_circleData.resize(circles.size());
    std::transform(circles.cbegin(), circles.cend(), m_circleData.begin(),
    [=](const CircleObject& co) { return genCircleRenderData(co); });
    glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, reinterpret_cast<const GLfloat*>(m_circleData.data()), GL_DYNAMIC_DRAW);
}

void CircleUniformBuffer::updateBufferSubData(const std::vector<CircleObject>& circles)
{

}