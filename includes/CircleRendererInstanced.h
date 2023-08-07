#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "CircleObject.h"
#include "ShaderProgram.h"
#include "CircleUniformBuffer.h"

class CircleRendererInstanced
{
public:
    CircleRendererInstanced();
    CircleRendererInstanced(std::shared_ptr<ShaderProgram> program);
    CircleRendererInstanced(const CircleRendererInstanced& cri);
    ~CircleRendererInstanced();

    void init();
    void render(const std::vector<CircleRenderData>& circles);

    constexpr void setViewportSize(const GLuint& width, const GLuint& height) { m_viewportSize = { width, height }; };

    std::shared_ptr<ShaderProgram> shaderProgram() const { return m_shaderProgram; };

    inline void setOutputFBO(const GLuint& fbo) { m_outputFBO = fbo; };
    inline void setOutputFBOTextures(const GLuint& sceneTex, const GLuint& bloomTex) { m_sceneFBOTexture = sceneTex; m_glowFBOTexture = bloomTex; };

private:
    void renderCircles();
    void initVBOs(GLuint& vbo);
    void initVertexAttrib(const GLuint& vbo, const GLuint& index, const size_t& numFloats, const size_t& strideFloats, const size_t& offsetFloats, const bool& divisor);
    void initShaderProgram();
    void initFullscreenQuad();
    void calculateCircleBufferData(const std::vector<CircleRenderData>& circles);

private:
    GLuint                          m_circleVAO{};
    GLuint                          m_circleVtxVBO{};
    GLuint                          m_circlePositionVBO{};
    GLuint                          m_circleRadiusVBO{};
    GLuint                          m_circleColorVBO{};
    GLuint                          m_circleModelMatrixVBO{};

    glm::uvec2                      m_viewportSize{};

    GLuint                          m_outputFBO{0};
    GLuint                          m_sceneFBOTexture{};
    GLuint                          m_glowFBOTexture{};

    GLuint                          m_fullscreenQuadVAO{};
    GLuint                          m_fullscreenQuadVBO{};

    std::shared_ptr<ShaderProgram>  m_shaderProgram{};
    std::shared_ptr<ShaderProgram>  m_outputShaderProgram{};

    std::vector<glm::vec2>          m_positionData{};
    std::vector<GLfloat>            m_radiusData{};
    std::vector<glm::vec4>          m_colorData{};
    std::vector<glm::mat4>          m_modelMatrixData{};

    std::array<GLfloat, 8>          m_circleVertices
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    std::array<GLfloat, 12>         m_fullscreenQuadVertices
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };
};