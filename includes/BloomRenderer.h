#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"

constexpr GLsizei NUM_MIPS{ 7 };

struct BloomMipmap
{
    GLuint          m_texture{};
    glm::vec2       m_texSize{};
};

/*
Implementation of the "Physics Based Bloom" rendering model presented here:
https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
*/
class BloomRenderer
{
public:
    BloomRenderer();
    BloomRenderer(const BloomRenderer& blr);
    ~BloomRenderer();

    void init();
    void initFBO(const GLsizei& screenWidth, const GLsizei& screenHeight);
    void render();
    void resizeTextures();

    const GLuint& inputFBO() const { return m_inputFBO; };
    const GLuint& inputFBOSceneTex() const { return m_inputFBOSceneTex; };
    const GLuint& inputFBOBloomTex() const { return m_inputFBOBloomTex; };

    constexpr void setViewportSize(const GLsizei& width, const GLsizei& height) { m_viewportSize = { width, height }; };

    GLfloat* glowIntensityRef() { return &m_glowIntensity; };
    GLfloat* colorIntensityRef() { return &m_colorIntensity; };

private:
    void downsample();
    void upsample();
    void initFullscreenQuad();
    void initShaderPrograms();


private:
    GLuint                          m_fullscreenQuadVAO{};
    GLuint                          m_fullscreenQuadVBO{};
    GLuint                          m_inputFBO{};
    GLuint                          m_outputFBO{0};
    GLuint                          m_bloomFBO{};
    GLuint                          m_inputFBOSceneTex{};
    GLuint                          m_inputFBOBloomTex{};
    GLuint                          m_outputFBOBloomTex{};
    std::vector<BloomMipmap>        m_bloomFBOBloomMipmaps{};

    glm::uvec2                      m_viewportSize{};
    std::shared_ptr<ShaderProgram>  m_gaussianShaderProgram{};
    std::shared_ptr<ShaderProgram>  m_finalizedBloomShaderProgram{};
    std::shared_ptr<ShaderProgram>  m_downsamplerShaderProgram{};
    std::shared_ptr<ShaderProgram>  m_upsamplerShaderProgram{};

    GLfloat                         m_glowIntensity{ 1.f };
    GLfloat                         m_colorIntensity{ 1.f };

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