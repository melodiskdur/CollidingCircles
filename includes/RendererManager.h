#pragma once

#include <memory>
#include <vector>
#include "glm/glm.hpp"

#include "CircleRendererInstanced.h"
#include "CircleRenderData.h"
#include "LineRenderer.h"
#include "GridRenderer.h"
#include "BloomRenderer.h"
#include "ShaderSettingsParams.h"

class RendererManager
{
public:
    RendererManager();
    ~RendererManager();

    bool initRenderers();
    void renderCircles(const std::vector<CircleRenderData>& renderData, const glm::mat4& viewProjection);
    void renderLines(const std::vector<LineObject>& lines, const glm::mat4& viewProjection);
    void renderGrid(const glm::mat4& model, const glm::mat4& viewProjection);

    constexpr void setViewportSize(unsigned int& width, unsigned int& height) { m_viewportSize = {width, height}; };
    inline void setShaderSettingsParams(std::shared_ptr<ShaderSettingsParams> ssp) { m_shaderSettingsParams = ssp; };

    inline std::shared_ptr<CircleRendererInstanced> circleRenderer() { return m_circleRenderer; };
    inline std::shared_ptr<BloomRenderer> bloomRenderer() { return m_bloomRenderer; };
    inline std::shared_ptr<LineRenderer> lineRenderer() { return m_lineRenderer; };
    inline std::shared_ptr<GridRenderer> gridRenderer() { return m_gridRenderer; };

private:
    glm::uvec2                                      m_viewportSize{};
    std::shared_ptr<ShaderSettingsParams>           m_shaderSettingsParams{};
    std::shared_ptr<CircleRendererInstanced>        m_circleRenderer{};
    std::shared_ptr<BloomRenderer>                  m_bloomRenderer{};
    std::shared_ptr<LineRenderer>                   m_lineRenderer{};
    std::shared_ptr<GridRenderer>                   m_gridRenderer{};
};