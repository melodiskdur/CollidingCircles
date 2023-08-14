#pragma once

#include <string>
#include <memory>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "IMenu.h"
#include "FlowControlParams.h"

class FlowControlMenu : public IMenu
{
public:
    FlowControlMenu();
    ~FlowControlMenu();

    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    void setFlowControlParams(std::shared_ptr<FlowControlParams> p) { m_flowControlParams = p; };
    void setBtnResetCallback(const std::function<void()>& callback) { m_btnResetCallback = callback; };

private:
    void drawHead() const;
    void drawBtnState();
    void drawSldrTimeStep() const;
    void drawBtnReset() const;
    void drawBtnStepForward();
    void drawDspSimulationTime() const;
    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };
    constexpr void indent(const std::size_t& n) const { for (auto i{0}; i < n; i++) ImGui::Indent(); };

private:
    std::string                         m_name{ "Flow Control" };
    std::shared_ptr<FlowControlParams>  m_flowControlParams{};
    std::string                         m_sldrTimestep{ "Time Interval" };
    std::string                         m_btnStateRun{ "Pause" };
    std::string                         m_btnStateStop{ "Run" };
    bool                                m_isRunning{ false };
    std::string                         m_btnReset{ "Reset Simulation" };
    std::function<void()>               m_btnResetCallback{};
    std::string                         m_btnStepForward{ "Step Forward" };

};