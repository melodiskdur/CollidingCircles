#include "CircleRenderer.h"
#include <iostream>
#include <string>
#include <fstream>

CircleRenderer::CircleRenderer() { /* ... */}

CircleRenderer::CircleRenderer(std::shared_ptr<ShaderProgram> shaderProgram, std::shared_ptr<ScreenObject> screenObject)
    : m_shaderProgram(shaderProgram), m_screenObject(screenObject)
{
    /* ... */ }

CircleRenderer::CircleRenderer(const CircleRenderer& cr)
{
    this->m_screenObject = cr.m_screenObject;
    this->m_shaderProgram = cr.m_shaderProgram;
}

CircleRenderer::~CircleRenderer() { /* ... */}

void CircleRenderer::init()
{
    glViewport(0, 0, m_screenObject->width(), m_screenObject->height());
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    //glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

    // TEST: Buffer textures instead of uniform arrays.

    initUniformBuffers();
    glUseProgram(m_shaderProgram->id());
    //GLuint radiiBlockIndex = glGetUniformBlockIndex(m_shaderProgram->id(), "radiiBuffer");
    GLuint positionsBlockIndex = glGetUniformBlockIndex(m_shaderProgram->id(), "positionsBuffer");
    GLuint colorsBlockIndex = glGetUniformBlockIndex(m_shaderProgram->id(), "colorsBuffer");

    //glUniformBlockBinding(m_shaderProgram->id(), radiiBlockIndex, 0);
    glUniformBlockBinding(m_shaderProgram->id(), positionsBlockIndex, 0);
    glUniformBlockBinding(m_shaderProgram->id(), colorsBlockIndex, 1);

}

void CircleRenderer::render(const std::vector<CircleRenderData>& circles)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCirclesData(circles);

    glUseProgram(m_shaderProgram->id());
    // glBindBuffer(GL_UNIFORM_BUFFER, m_radiiBuffer);
    // glBufferSubData(GL_UNIFORM_BUFFER, 0, circles.size() * 4 * sizeof(GLfloat), m_radiiData.data());

    glBindBuffer(GL_UNIFORM_BUFFER, m_positionsBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, circles.size() * 4 * sizeof(GLfloat), reinterpret_cast<const GLfloat*>(m_positionsData.data()));

    glBindBuffer(GL_UNIFORM_BUFFER, m_colorsBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, circles.size() * 4 * sizeof(GLfloat), reinterpret_cast<const GLfloat*>(m_colorsData.data()));

    // glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_radiiBuffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_positionsBuffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_colorsBuffer);

    glBindVertexArray(m_screenObject->VAO());
    glDrawElements(GL_TRIANGLES, ScreenObject::FACES_SIZE, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void CircleRenderer::initUniformBuffers()
{
    const GLsizei MAX_SIZE{ 1000 };

    m_radiiData.resize(MAX_SIZE);
    m_positionsData.resize(MAX_SIZE);
    m_colorsData.resize(MAX_SIZE);

    // glGenBuffers(1, &m_radiiBuffer);
    // glBindBuffer(GL_UNIFORM_BUFFER, m_radiiBuffer);
    // glBufferData(GL_UNIFORM_BUFFER, MAX_SIZE * 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &m_positionsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_positionsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, MAX_SIZE * 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &m_colorsBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_colorsBuffer);
    glBufferData(GL_UNIFORM_BUFFER, MAX_SIZE * 4 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}


void CircleRenderer::updateCirclesData(const std::vector<CircleRenderData>& circles)
{
    // m_radiiData.clear(); m_radiiData.resize(circles.size());
    // m_positionsData.clear(); m_positionsData.resize(circles.size());
    // m_colorsData.clear(); m_colorsData.resize(circles.size());
    for (size_t i{0}; i < circles.size(); i++)
    {
        // m_radiiData[i] = glm::vec4(circles[i].m_radius, 0.f, 0.f, 0.f);
        m_positionsData[i] = glm::vec4(circles[i].m_pos, circles[i].m_radius, 0.f);
        m_colorsData[i] = glm::vec4(circles[i].m_color, 1.0f);
    }
}

void CircleRenderer::scaleViewportToWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void CircleRenderer::bindUniformBuffersToBindingPoints()
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_radiiBuffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_positionsBuffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_colorsBuffer);
}