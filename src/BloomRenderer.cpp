#include "BloomRenderer.h"

BloomRenderer::BloomRenderer()
{ /* ... */ }

BloomRenderer::BloomRenderer(const BloomRenderer& blr)
{
    this->m_gaussianShaderProgram = blr.m_gaussianShaderProgram;
    this->m_finalizedBloomShaderProgram = blr.m_finalizedBloomShaderProgram;
}

BloomRenderer::~BloomRenderer()
{ /* ... */ }

void BloomRenderer::init()
{
    initShaderPrograms();
    initFullscreenQuad();
}

void BloomRenderer::initFBO(const GLsizei& screenWidth, const GLsizei& screenHeight)
{
    m_viewportSize = {screenWidth, screenHeight};

    glGenFramebuffers(1, &m_inputFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_inputFBO);

    // Scene FBO texture.
    glGenTextures(1, &m_inputFBOSceneTex);
    glBindTexture(GL_TEXTURE_2D, m_inputFBOSceneTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0,  GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_inputFBOSceneTex, 0);

    // Bloom FBO texture.
    glGenTextures(1, &m_inputFBOBloomTex);
    glBindTexture(GL_TEXTURE_2D, m_inputFBOBloomTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0,  GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_inputFBOBloomTex, 0);

	GLenum attachments[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Bloom Mip map FBO.
    glGenFramebuffers(1, &m_bloomFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_bloomFBO);

    glm::vec2 mipSize((float)m_viewportSize.x, (float)m_viewportSize.y);

    for (unsigned int i = 0; i < NUM_MIPS; i++)
    {
        BloomMipmap mip;

        mipSize *= 0.5f;
        mip.m_texSize = mipSize;

        glGenTextures(1, &mip.m_texture);
        glBindTexture(GL_TEXTURE_2D, mip.m_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                     (int)mipSize.x, (int)mipSize.y,
                     0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_bloomFBOBloomMipmaps.emplace_back(mip);

    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bloomFBOBloomMipmaps[0].m_texture, 0);
    glDrawBuffers(1, attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomRenderer::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_bloomFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    downsample();
    upsample();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

    glUseProgram(m_finalizedBloomShaderProgram->id());
    glBindVertexArray(m_fullscreenQuadVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_inputFBOSceneTex);
    m_finalizedBloomShaderProgram->updateUniform1i("sceneBuffer", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_bloomFBOBloomMipmaps[0].m_texture);
    m_finalizedBloomShaderProgram->updateUniform1i("glowBuffer", 1);

    m_finalizedBloomShaderProgram->updateUniform1f("glowIntensity", m_glowIntensity);
    m_finalizedBloomShaderProgram->updateUniform1f("colorIntensity", m_colorIntensity);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

void BloomRenderer::resizeTextures()
{
    glBindTexture(GL_TEXTURE_2D, m_inputFBOSceneTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_viewportSize.x, m_viewportSize.y, 0,  GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, m_inputFBOBloomTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_viewportSize.x, m_viewportSize.y, 0,  GL_RGB, GL_FLOAT, nullptr);

    // Mipmaps.
    glm::vec2 mipSize((float)m_viewportSize.x, (float)m_viewportSize.y);
    for (unsigned int i = 0; i < NUM_MIPS; i++)
    {
        BloomMipmap& mip { m_bloomFBOBloomMipmaps[i] };

        mipSize *= 0.5f;
        mip.m_texSize = mipSize;

        glBindTexture(GL_TEXTURE_2D, mip.m_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                     (int)mipSize.x, (int)mipSize.y,
                     0, GL_RGB, GL_FLOAT, nullptr);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void BloomRenderer::downsample()
{
    glUseProgram(m_downsamplerShaderProgram->id());
    m_downsamplerShaderProgram->updateUniform2f("srcResolution", m_viewportSize);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_inputFBOBloomTex);
    m_downsamplerShaderProgram->updateUniform1i("srcTexture", 0);

    std::for_each(m_bloomFBOBloomMipmaps.begin(), m_bloomFBOBloomMipmaps.end(),
    [&](const BloomMipmap& bmm)
    {
        glViewport(0, 0, static_cast<GLsizei>(bmm.m_texSize.x), static_cast<GLsizei>(bmm.m_texSize.y));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bmm.m_texture, 0);
        GLenum status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("gbuffer FBO error, status: 0x\%x\n", status);
        }
        glBindVertexArray(m_fullscreenQuadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        m_downsamplerShaderProgram->updateUniform2f("srcResolution", bmm.m_texSize);
        glBindTexture(GL_TEXTURE_2D, bmm.m_texture);
    });

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void BloomRenderer::upsample()
{
    glUseProgram(m_upsamplerShaderProgram->id());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_inputFBOBloomTex);
    m_upsamplerShaderProgram->updateUniform1i("srcTexture", 0);
    m_upsamplerShaderProgram->updateUniform1f("filterRadius", 0.001f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    for (size_t i{m_bloomFBOBloomMipmaps.size() - 1}; i > 0; i--)
    {
        const BloomMipmap& currentMip{ m_bloomFBOBloomMipmaps[i] };
        const BloomMipmap& nextMip{ m_bloomFBOBloomMipmaps[i - 1] };

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currentMip.m_texture);

        glViewport(0, 0, static_cast<GLsizei>(nextMip.m_texSize.x), static_cast<GLsizei>(nextMip.m_texSize.y));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nextMip.m_texture, 0);

        glBindVertexArray(m_fullscreenQuadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void BloomRenderer::initFullscreenQuad()
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

void BloomRenderer::initShaderPrograms()
{
    m_gaussianShaderProgram = std::make_shared<ShaderProgram>();
    m_gaussianShaderProgram->addShader(std::string{"gaussian_vtx"}, std::string("shader/simpleInstancedGaussian.vert"), GL_VERTEX_SHADER);
	m_gaussianShaderProgram->addShader(std::string{"gaussian_frag"}, std::string("shader/simpleInstancedGaussian.frag"), GL_FRAGMENT_SHADER);
    m_gaussianShaderProgram->combineShadersIntoPair("gaussian_vtx", "gaussian_frag");
	m_gaussianShaderProgram->setCurrentShaderPair("gaussian_vtx", "gaussian_frag");
	m_gaussianShaderProgram->linkAndUse();

    m_finalizedBloomShaderProgram = std::make_shared<ShaderProgram>();
    m_finalizedBloomShaderProgram->addShader(std::string{"bloom_vtx"}, std::string("shader/simpleFinalizedBloom.vert"), GL_VERTEX_SHADER);
	m_finalizedBloomShaderProgram->addShader(std::string{"bloom_frag"}, std::string("shader/simpleFinalizedBloom.frag"), GL_FRAGMENT_SHADER);
    m_finalizedBloomShaderProgram->combineShadersIntoPair("bloom_vtx", "bloom_frag");
	m_finalizedBloomShaderProgram->setCurrentShaderPair("bloom_vtx", "bloom_frag");
	m_finalizedBloomShaderProgram->linkAndUse();

    m_downsamplerShaderProgram = std::make_shared<ShaderProgram>();
    m_downsamplerShaderProgram->addShader(std::string("downsample_vtx"), std::string("shader/simpleInstancedGaussian.vert"), GL_VERTEX_SHADER);
    m_downsamplerShaderProgram->addShader(std::string("downsample_frag"), std::string("shader/downsampler.frag"), GL_FRAGMENT_SHADER);
    m_downsamplerShaderProgram->combineShadersIntoPair("downsample_vtx", "downsample_frag");
    m_downsamplerShaderProgram->setCurrentShaderPair("downsample_vtx", "downsample_frag");
    m_downsamplerShaderProgram->linkAndUse();

    m_upsamplerShaderProgram = std::make_shared<ShaderProgram>();
    m_upsamplerShaderProgram->addShader(std::string("upsample_vtx"), std::string("shader/simpleInstancedGaussian.vert"), GL_VERTEX_SHADER);
    m_upsamplerShaderProgram->addShader(std::string("upsample_frag"), std::string("shader/upsampler.frag"), GL_FRAGMENT_SHADER);
    m_upsamplerShaderProgram->combineShadersIntoPair("upsample_vtx", "upsample_frag");
    m_upsamplerShaderProgram->setCurrentShaderPair("upsample_vtx", "upsample_frag");
    m_upsamplerShaderProgram->linkAndUse();
}