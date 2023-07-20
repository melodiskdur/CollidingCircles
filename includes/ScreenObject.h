#pragma once

#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class ScreenObject
{
public:
    ScreenObject();
    ScreenObject(const GLuint& width, const GLuint& height);
    ~ScreenObject();

    inline glm::mat4 modelMatrix(const GLuint& width, const GLuint& height)
    { return glm::scale(glm::mat4(1.0f), glm::vec3((GLfloat)width, (GLfloat)height, 0.0f)); };
    void setEBO(GLuint& ebo);
    void setDimensions(const GLuint&& width, const GLuint& height);
    void initialize();

    constexpr GLuint VAO() const          { return m_vao; }
    constexpr GLuint VBO() const          { return m_vbo; }
    constexpr GLuint EBO() const          { return m_ebo; }
    constexpr GLsizei height() const      { return m_height; }
    constexpr GLsizei width() const       { return m_width; }

    static const GLsizei        POINTS_SIZE{ 8 };
    static const GLsizei        FACES_SIZE{ 6 };
private:
    GLuint                      m_vao{ 0 };
    GLuint                      m_vbo{ 0 };
    GLuint                      m_ebo{ 0 };
    GLsizei                     m_height{ 0 };
    GLsizei                     m_width{ 0 };

    std::array<GLfloat, POINTS_SIZE> m_points
    {
        -1.0f, -1.0f,
		-1.0f,	1.0f,
		 1.0f,	1.0f,
		 1.0f, -1.0f
    };

    std::array<GLshort, FACES_SIZE> m_faces
    {
        0, 1, 2,
		0, 2, 3
    };
};