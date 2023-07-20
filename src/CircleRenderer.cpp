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
}

void CircleRenderer::render(const std::vector<CircleObject>& circles)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    glUseProgram(m_shaderProgram->id());
    glBindVertexArray(m_screenObject->VAO());
    glDrawElements(GL_TRIANGLES, ScreenObject::FACES_SIZE, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void CircleRenderer::scaleViewportToWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}