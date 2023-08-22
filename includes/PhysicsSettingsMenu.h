#pragma once

#include <string>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "IMenu.h"
#include "PhysicsSettingsParams.h"

class PhysicsSettingsMenu : public IMenu
{
public:
    PhysicsSettingsMenu();
    ~PhysicsSettingsMenu();

    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    void setPhysicsSettingsParams(std::shared_ptr<PhysicsSettingsParams> psp) { m_physicsSettingsParams = psp; };

private:
    void drawHead() const;
    void drawGravityConstantSelector();
    void drawCheckboxCollisionDetection();

    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                                 m_name{ "Physics Settings" };
    std::shared_ptr<PhysicsSettingsParams>      m_physicsSettingsParams{};
    std::string                                 m_gravityHeader{ "Gravitational Constant" };
    std::string                                 m_chkbxCollisionDetectionEnabled{ "Collision Detection (Circles Collide & Merge)" };
};