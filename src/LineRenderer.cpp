#include "LineRenderer.h"

LineRenderer::LineRenderer()
{

}

LineRenderer::~LineRenderer()
{

}

void LineRenderer::init()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

    m_shaderProgram = std::make_shared<ShaderProgram>();
    m_shaderProgram->addShader(std::string{"line_vtx"}, std::string("shader/line.vert"), GL_VERTEX_SHADER);
	m_shaderProgram->addShader(std::string{"line_frag"}, std::string("shader/line.frag"), GL_FRAGMENT_SHADER);
	m_shaderProgram->combineShadersIntoPair("line_vtx", "line_frag");
	m_shaderProgram->setCurrentShaderPair("line_vtx", "line_frag");
	m_shaderProgram->linkAndUse();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineRenderer::render(const std::vector<LineObject>& lines, const glm::mat4& viewProjectionMatrix)
{

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glUseProgram(m_shaderProgram->id());
    m_shaderProgram->updateUniformMatrix4fv("viewProjectionMatrix", viewProjectionMatrix);
    std::for_each(lines.begin(), lines.end(), [=](const LineObject& line)
    {
        m_shaderProgram->updateUniformMatrix4fv("modelMatrix", line.modelMatrix());
        m_shaderProgram->updateUniform3f("color", line.color());
        glDrawArrays(GL_LINES, 0, 2);
    });
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}