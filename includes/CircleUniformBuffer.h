#pragma once

#include <vector>
#include "CircleRenderData.h"
#include "CircleObject.h"

class CircleUniformBuffer
{
public:
    CircleUniformBuffer();
    ~CircleUniformBuffer();

    std::vector<CircleRenderData>& circleRenderData() { return m_circleData; };
    constexpr GLuint bindingPoint() const { return m_bindingPoint; };

    void init();
    void bindToPoint(const GLuint& bindingPoint);
    void updateBufferData(const std::vector<CircleObject>& circles);
    void updateBufferSubData(const std::vector<CircleObject>& circles);

public:
    constexpr static GLsizei BUFFER_MAX{ 1000 };

private:
    constexpr CircleRenderData genCircleRenderData(const CircleObject& co) { return CircleRenderData{ co.pos(), co.color(), co.radius() }; };

private:
    GLuint                              m_ubo{};
    GLuint                              m_bindingPoint{};
    std::size_t                         m_bufferSize{};
    std::vector<CircleRenderData>       m_circleData{};
    std::vector<GLfloat>                m_buffer{};
};