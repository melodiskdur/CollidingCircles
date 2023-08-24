#pragma once

#include <string>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "ISubMenu.h"
#include "SpiralPatternParams.h"

class SpiralPatternSubMenu : public ISubMenu
{
public:
    SpiralPatternSubMenu();
    ~SpiralPatternSubMenu();

    const char* name() const override { return m_name.c_str(); };
    void draw() override;

    void setSpiralPatternParams(std::shared_ptr<SpiralPatternParams> params) { m_params = params; };

private:
    void drawHead();
    void drawSliders();
    void drawCheckboxes();

    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                                     m_name{ "SpiralPatternSubMenu" };
    std::shared_ptr<SpiralPatternParams>            m_params{};

};