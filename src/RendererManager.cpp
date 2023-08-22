#include "RendererManager.h"

RendererManager::RendererManager()
{ /* ... */ }

RendererManager::~RendererManager()
{ /* ... */ }

bool RendererManager::initRenderers()
{
    m_gridRenderer = std::make_shared<GridRenderer>();
	m_gridRenderer->init(glm::vec2(10, 10));
	m_lineRenderer = std::make_shared<LineRenderer>();
	m_lineRenderer->init();
	m_circleRenderer = std::make_shared<CircleRendererInstanced>();
	m_circleRenderer->setViewportSize(m_viewportSize.x, m_viewportSize.y);
	m_circleRenderer->init();
	m_bloomRenderer = std::make_shared<BloomRenderer>();
	m_bloomRenderer->init();
	m_bloomRenderer->initFBO(m_viewportSize.x, m_viewportSize.y);
	return true;
}

void RendererManager::renderCircles(const std::vector<CircleRenderData>& renderData, const glm::mat4& viewProjection)
{
	if (m_shaderSettingsParams->shaderType() == 0)
	{
		m_circleRenderer->setOutputFBO(0);
	}
	else
	{
		m_circleRenderer->setOutputFBO(m_bloomRenderer->inputFBO());
		m_circleRenderer->setOutputFBOTextures(m_bloomRenderer->inputFBOSceneTex(), m_bloomRenderer->inputFBOBloomTex());
	}
    auto shaderProgram_cren{ m_circleRenderer->shaderProgram() };
	glUseProgram(shaderProgram_cren->id());
	shaderProgram_cren->updateUniformMatrix4fv("viewProjectionMatrix", viewProjection);
	GLfloat glowIntensity{ *m_bloomRenderer->glowIntensityRef() };
	GLfloat colorIntensity{ *m_bloomRenderer->colorIntensityRef() };
	shaderProgram_cren->updateUniform1f("glowIntensity", glowIntensity);
	shaderProgram_cren->updateUniform1f("colorIntensity", colorIntensity);
	m_circleRenderer->render(renderData);
    if (m_shaderSettingsParams->shaderType() == 1) m_bloomRenderer->render();
}

void RendererManager::renderLines(const std::vector<LineObject>& lines, const glm::mat4& viewProjection)
{
    m_lineRenderer->render(lines, viewProjection);
}

void RendererManager::renderGrid(const glm::mat4& model, const glm::mat4& viewProjection)
{
    if (m_shaderSettingsParams->isDisplayGridAvailable())
	{
		auto shaderProgram_grid{ m_gridRenderer->shaderProgram() };
		glUseProgram( shaderProgram_grid->id());
		shaderProgram_grid->updateUniformMatrix4fv("modelMatrix", model);
		shaderProgram_grid->updateUniformMatrix4fv("viewProjectionMatrix", viewProjection);
		m_gridRenderer->render();
	}
}
