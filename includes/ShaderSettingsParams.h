#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

struct GlowParams
{
    GLfloat         m_glowIntensity{};
    GLfloat         m_colorIntensity{};
};

class ShaderSettingsParams
{
public:
    ShaderSettingsParams() { /* ... */ }

    ~ShaderSettingsParams() { /* ... */ }

    constexpr bool isDisplayGridAvailable() const { return m_displayGridAvailable; };
    constexpr size_t shaderType() const { return m_shaderType; };
    constexpr GlowParams glowParams() const { return m_glowParams; };

    bool* displayGridAvailableRef() { return &m_displayGridAvailable; };
    size_t* shaderTypeRef() { return &m_shaderType; };
    GLfloat* glowIntensityRef() { return &(m_glowParams.m_glowIntensity); };
    GLfloat* colorIntensity() { return &(m_glowParams.m_colorIntensity); };

private:
    bool                m_displayGridAvailable{};
    size_t              m_shaderType{};
    GlowParams          m_glowParams{};
};