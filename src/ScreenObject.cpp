#include "ScreenObject.h"

ScreenObject::ScreenObject() { /* ... */}

ScreenObject::ScreenObject(const GLuint& width, const GLuint& height) : m_width(width), m_height(height)
{ /* ... */}

ScreenObject::~ScreenObject() { /* ... */}

void ScreenObject::setDimensions(const GLuint&& width, const GLuint& height)
{
    m_width = width;
    m_height = height;
}

void ScreenObject::initialize()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, ScreenObject::POINTS_SIZE * sizeof(GLfloat), (void*)0);
}

void ScreenObject::setEBO(GLuint& ebo)
{
    // Element buffer.
    m_ebo = ebo;
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_faces), m_faces.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}