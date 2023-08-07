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

class CircleCreatorMenu : public IMenu
{
public:
    CircleCreatorMenu();
    ~CircleCreatorMenu();
    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    constexpr void setColorReference(glm::vec3* color) { m_colorReference = color; };
    constexpr void setRadiusReference(GLfloat* r) { m_radiusReference = r; };
    constexpr void setMassReference(GLfloat* m) { m_massReference = m; };
    void setCheckboxStationaryCallback(const std::function<void(const bool&)>& callback) { m_stationaryCheckedCallback = callback; };
    void setNumCirclesReference(GLsizei* nc) { m_numCirclesReference = nc; };

private:
    void drawHead() const;
    void drawColorSelector();
    void drawRadiusSelector();
    void drawMassSelector();
    void drawStationaryCheckBox();
    void drawNumCircles();
    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                 m_name{ "Circle Creator" };
    std::string                 m_colorHeader{ "Color" };
    std::string                 m_colorText{ "Chosen Color" };
    glm::vec3*                  m_colorReference{ nullptr };
    std::string                 m_radiusHeader{ "Radius"};
    GLfloat*                    m_radiusReference{ nullptr };
    std::string                 m_massHeader{ "Mass" };
    GLfloat*                    m_massReference{ nullptr };
    std::string                 m_stationaryHeader{ "Set Stationary" };
    bool                        m_stationaryChecked{ false };
    std::function<void(const bool&)>            m_stationaryCheckedCallback{};
    std::string                 m_numCirclesHeader{ "Number of circles" };
    GLsizei*                    m_numCirclesReference{};
};