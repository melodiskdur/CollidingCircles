#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class ShaderSettingsParams
{
public:
    ShaderSettingsParams(GLfloat* _glowIntensitytRef, GLfloat* _colorIntensityRef) : m_glowIntensityRef(_glowIntensitytRef), m_colorIntensityRef(_colorIntensityRef)
    { /* ... */ }

    ~ShaderSettingsParams() { /* ... */ }

    constexpr bool isDisplayGridAvailable() const { return m_displayGridAvailable; };
    constexpr size_t shaderType() const { return m_shaderType; };

    bool* displayGridAvailableRef() { return &m_displayGridAvailable; };
    size_t* shaderTypeRef() { return &m_shaderType; };
    GLfloat* glowIntensityRef() { return m_glowIntensityRef; };
    GLfloat* colorIntensityRef() { return m_colorIntensityRef; };

    static constexpr GLfloat GLOW_INTENSITY_MIN{ 0.1f };
    static constexpr GLfloat GLOW_INTENSITY_MAX{ 5.0f };
    static constexpr GLfloat COLOR_INTENSITY_MIN{ 0.1f };
    static constexpr GLfloat COLOR_INTENSITY_MAX{ 5.0f };

private:
    GLfloat*            m_glowIntensityRef{ nullptr };
    GLfloat*            m_colorIntensityRef{ nullptr };
    bool                m_displayGridAvailable{ true };
    size_t              m_shaderType{ 1 };
};