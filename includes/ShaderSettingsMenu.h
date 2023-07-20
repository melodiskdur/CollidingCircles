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

class ShaderSettingsMenu : public IMenu
{
public:
    ShaderSettingsMenu();
    ~ShaderSettingsMenu();

    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    void setCheckboxDisplayGridCallback(const std::function<void(const bool&)>& callback) { m_chckbxDisplayGridCallback = callback; };
    void setSelectShaderTypeCallback(const std::function<void(const std::size_t& i)> callback) { m_slctShaderTypeCallback = callback; };

private:
    void drawHead() const;
    void drawCheckboxDisplayGrid();
    void drawSelectShaderType();

    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                                 m_name{ "Shader Settings" };
    std::string                                 m_chkbxDisplayGrid{ "Display Grid" };
    bool                                        m_chckbxDisplayGridIsChecked{ true };
    std::function<void(const bool&)>            m_chckbxDisplayGridCallback{};
    std::string                                 m_slctShaderType{ "Shader Type" };
    std::vector<std::string>                    m_shaders{ "Simple", "Glow" };
    std::size_t                                 m_selected{ 0 };
    std::function<void(const std::size_t& i)>   m_slctShaderTypeCallback{ };
};