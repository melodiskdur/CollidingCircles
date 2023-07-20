#include "GridRenderer.h"
#include <algorithm>
GridRenderer::GridRenderer()
{

}

GridRenderer::~GridRenderer()
{

}

void GridRenderer::init()
{
    generateGridVertices();

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_gridVertices.size() * sizeof(GLfloat), m_gridVertices.data(), GL_STATIC_DRAW);

    m_shaderProgram = std::make_shared<ShaderProgram>();
    m_shaderProgram->addShader(std::string{"grid_vtx"}, std::string("shader/grid.vert"), GL_VERTEX_SHADER);
	m_shaderProgram->addShader(std::string{"grid_frag"}, std::string("shader/grid.frag"), GL_FRAGMENT_SHADER);
	m_shaderProgram->combineShadersIntoPair("grid_vtx", "grid_frag");
	m_shaderProgram->setCurrentShaderPair("grid_vtx", "grid_frag");
	m_shaderProgram->linkAndUse();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    initEBO();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GridRenderer::initEBO()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}

void GridRenderer::render()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glUseProgram(m_shaderProgram->id());
    // glDrawElements(GL_LINES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_SHORT, 0);
    glDrawArrays(GL_LINES, 0, m_gridVertices.size() / 2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GridRenderer::generateGridVertices()
{
    const float gridSize = 2.0f; // Size of the grid (in NDC)
    const float gridSpacing = 0.2f; // Spacing between grid lines (in NDC)

    // Generate horizontal lines
    for (float y = -gridSize; y <= gridSize; y += gridSpacing)
    {
        m_gridVertices.push_back(-gridSize); // Start point (x-coordinate)
        m_gridVertices.push_back(y); // Start point (y-coordinate)
        m_gridVertices.push_back(gridSize); // End point (x-coordinate)
        m_gridVertices.push_back(y); // End point (y-coordinate)
    }

    // Generate vertical lines
    for (float x = -gridSize; x <= gridSize; x += gridSpacing)
    {
        m_gridVertices.push_back(x); // Start point (x-coordinate)
        m_gridVertices.push_back(-gridSize); // Start point (y-coordinate)
        m_gridVertices.push_back(x); // End point (x-coordinate)
        m_gridVertices.push_back(gridSize); // End point (y-coordinate)
    }
}