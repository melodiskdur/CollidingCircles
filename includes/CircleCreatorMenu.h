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
#include "ISubMenu.h"
#include "SpiralPatternGenerator.h"
#include "CircleCreatorSettings.h"

class CircleCreatorMenu : public IMenu
{
public:
    CircleCreatorMenu();
    ~CircleCreatorMenu();
    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    void setCircleCreatorSettings(std::shared_ptr<CircleCreatorSettings> ccs) { m_circleCreatorSettings = ccs; };
    void addPatternSubMenu(std::shared_ptr<ISubMenu> subMenu) { m_patternSubMenus.push_back(subMenu); };

private:
    void drawHead() const;
    void drawColorSelector();
    void drawRadiusSelector();
    void drawMassSelector();
    void drawStationaryCheckBox();
    void drawNumCircles();
    void drawGeneratorHeader();
    void drawGeneratorType();
    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                                 m_name{ "Circle Creator" };
    std::shared_ptr<CircleCreatorSettings>      m_circleCreatorSettings{};
    std::string                                 m_colorHeader{ "Color" };
    std::string                                 m_colorText{ "Chosen Color" };
    std::string                                 m_radiusHeader{ "Radius"};
    std::string                                 m_massHeader{ "Mass" };
    std::string                                 m_stationaryHeader{ "Set Stationary" };
    std::string                                 m_numCirclesHeader{ "Number of circles" };
    std::string                                 m_generatorTypeHeader{ "Creation Type" };
    std::vector<std::string>                    m_generators{ "Single Circle", "Galaxy Pattern" };
    GLsizei                                     m_selectedGenerator{ 0 };
    std::vector<std::shared_ptr<ISubMenu>>      m_patternSubMenus{  };
};