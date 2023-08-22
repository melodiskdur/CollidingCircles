#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "IMenu.h"
#include "ShaderSettingsParams.h"

class ShaderSettingsMenu : public IMenu
{
public:
    ShaderSettingsMenu();
    ~ShaderSettingsMenu();

    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    void setShaderSettingsParams(std::shared_ptr<ShaderSettingsParams> ssp) { m_shaderSettingsParams = ssp; };

private:
    void drawHead() const;
    void drawCheckboxDisplayGrid();
    void drawCheckboxDisplayQuadTree();
    void drawSelectShaderType();

    void drawSldrGlowIntensity();
    void drawSldrColorIntensity();

    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                                 m_name{ "Shader Settings" };
    std::string                                 m_chkbxDisplayGrid{ "Display Grid" };
    std::string                                 m_chckbxDisplayQuadTree{ "Disply Barnes-Hut Quadtree (Note: Performance Heavy)" };
    std::string                                 m_slctShaderType{ "Shader Type" };
    std::vector<std::string>                    m_shaders{ "Simple", "Glow" };
    std::size_t                                 m_selected{ 1 };

    std::string                                 m_sldrGlowIntensityHeader{ "Glow Intensity" };
    std::string                                 m_sldrColorIntensityHeader{ "Color Intensity" };

    std::shared_ptr<ShaderSettingsParams>       m_shaderSettingsParams{ };
};