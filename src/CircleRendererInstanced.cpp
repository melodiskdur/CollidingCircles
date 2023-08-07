#include "CircleRendererInstanced.h"

#include <iostream>

CircleRendererInstanced::CircleRendererInstanced()
{

}

CircleRendererInstanced::CircleRendererInstanced(std::shared_ptr<ShaderProgram> program)
    : m_shaderProgram(program)
{

}

CircleRendererInstanced::CircleRendererInstanced(const CircleRendererInstanced& cri)
{
}

CircleRendererInstanced::~CircleRendererInstanced()
{

}

void CircleRendererInstanced::init()
{
    initShaderProgram();

    initVBOs(m_circlePositionVBO);
    initVBOs(m_circleRadiusVBO);
    initVBOs(m_circleColorVBO);
    initVBOs(m_circleModelMatrixVBO);

    glGenBuffers(1, &m_circleVtxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_circleVtxVBO);
    glBufferData(GL_ARRAY_BUFFER, m_circleVertices.size() * sizeof(GLfloat), m_circleVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &m_circleVAO);
    glBindVertexArray(m_circleVAO);

    initVertexAttrib(m_circleVtxVBO, 0, 2, 2, 0, false);
    initVertexAttrib(m_circlePositionVBO, 1, 2, 2, 0, true);
    initVertexAttrib(m_circleRadiusVBO, 2, 1, 1, 0, true);
    initVertexAttrib(m_circleColorVBO, 3, 4, 4, 0, true);
    for (size_t i{4}; i < 8; i++)
    {
        initVertexAttrib(m_circleModelMatrixVBO, i, 4, 16, 4 * (i % 4), true);
    }

    glBindVertexArray(0);

    /*
    // FBO setup.
    glGenFramebuffers(1, &m_outputFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFBO);

    // Scene FBO texture.
    glGenTextures(1, &m_sceneFBOTexture);
    glBindTexture(GL_TEXTURE_2D, m_sceneFBOTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_viewportSize.x, m_viewportSize.y, 0,  GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_sceneFBOTexture, 0);

    // Bloom FBO texture.
    glGenTextures(1, &m_glowFBOTexture);
    glBindTexture(GL_TEXTURE_2D, m_glowFBOTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_viewportSize.x, m_viewportSize.y, 0,  GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_glowFBOTexture, 0);

	GLuint attachments[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    initFullscreenQuad();
    */
}

void CircleRendererInstanced::render(const std::vector<CircleRenderData>& circles)
{
    calculateCircleBufferData(circles);
    glBindFramebuffer(GL_FRAMEBUFFER, m_outputFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_shaderProgram->id());
    renderCircles();
    glUseProgram(0);

    /*
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_outputShaderProgram->id());
    glBindVertexArray(m_fullscreenQuadVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sceneFBOTexture);
    m_outputShaderProgram->updateUniform1i("sceneBuffer", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_glowFBOTexture);
    m_outputShaderProgram->updateUniform1i("glowBuffer", 1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
    */

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CircleRendererInstanced::renderCircles()
{
    glBindVertexArray(m_circleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_circlePositionVBO);
    glBufferData(GL_ARRAY_BUFFER, m_positionData.size() * sizeof(glm::vec2), m_positionData.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_circleRadiusVBO);
    glBufferData(GL_ARRAY_BUFFER, m_positionData.size() * sizeof(GLfloat), m_radiusData.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_circleColorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_positionData.size() * sizeof(glm::vec4), m_colorData.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_circleModelMatrixVBO);
    glBufferData(GL_ARRAY_BUFFER, m_positionData.size() * 16 * sizeof(GLfloat), m_modelMatrixData.data(), GL_DYNAMIC_DRAW);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, static_cast<GLsizei>(m_positionData.size()));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CircleRendererInstanced::initVBOs(GLuint& vbo)
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CircleRendererInstanced::initVertexAttrib(const GLuint& vbo, const GLuint& index, const size_t& numFloats ,const size_t& strideFloats, const size_t& offsetFloats, const bool& divisor)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(index, numFloats, GL_FLOAT, GL_FALSE, strideFloats * sizeof(GLfloat), (void*)(offsetFloats * sizeof(GLfloat)));
    glEnableVertexAttribArray(index);
    if (divisor)
        glVertexAttribDivisor(index, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CircleRendererInstanced::initShaderProgram()
{
    m_shaderProgram = std::make_shared<ShaderProgram>();
    m_shaderProgram->addShader(std::string{"simpleInstanced_vtx"}, std::string("shader/simpleInstanced.vert"), GL_VERTEX_SHADER);
	m_shaderProgram->addShader(std::string{"simpleInstanced_frag"}, std::string("shader/simpleInstanced.frag"), GL_FRAGMENT_SHADER);
    m_shaderProgram->combineShadersIntoPair("simpleInstanced_vtx", "simpleInstanced_frag");
	m_shaderProgram->setCurrentShaderPair("simpleInstanced_vtx", "simpleInstanced_frag");
	m_shaderProgram->linkAndUse();

    /*
    m_outputShaderProgram = std::make_shared<ShaderProgram>();
    m_outputShaderProgram->addShader(std::string{"simpleOutput_vtx"}, std::string("shader/simpleFinalizedBloom.vert"), GL_VERTEX_SHADER);
	m_outputShaderProgram->addShader(std::string{"simpleOutput_frag"}, std::string("shader/simpleFinalizedBloom.frag"), GL_FRAGMENT_SHADER);
    m_outputShaderProgram->combineShadersIntoPair("simpleOutput_vtx", "simpleOutput_frag");
	m_outputShaderProgram->setCurrentShaderPair("simpleOutput_vtx", "simpleOutput_frag");
	m_outputShaderProgram->linkAndUse();
    */
}

void CircleRendererInstanced::calculateCircleBufferData(const std::vector<CircleRenderData>& circles)
{
    m_positionData.clear(); m_positionData.resize(circles.size());
    m_radiusData.clear(); m_radiusData.resize(circles.size());
    m_colorData.clear(); m_colorData.resize(circles.size());
    m_modelMatrixData.clear(); m_modelMatrixData.resize(circles.size());

    for (size_t i{0}; i < circles.size(); i++)
    {
        m_positionData[i] = circles[i].m_pos;
        m_radiusData[i] = circles[i].m_radius;
        m_colorData[i] = glm::vec4(circles[i].m_color, 1.0f);
        m_modelMatrixData[i] = glm::scale( glm::translate(glm::mat4(1.0f), glm::vec3(circles[i].m_pos, 0.0f)),
                                         glm::vec3(circles[i].m_radius, circles[i].m_radius, 0.f));
    }
}

void CircleRendererInstanced::initFullscreenQuad()
{
    glGenVertexArrays(1, &m_fullscreenQuadVAO);
    glBindVertexArray(m_fullscreenQuadVAO);

    glGenBuffers(1, &m_fullscreenQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, m_fullscreenQuadVertices.size() * sizeof(GLfloat), m_fullscreenQuadVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}