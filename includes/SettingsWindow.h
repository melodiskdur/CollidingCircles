#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "IMenu.h"
#include "FlowControlMenu.h"

constexpr const float DEFAULT_SWINDOW_WIDTH { 0.25f };
constexpr const float DEFAULT_SWINDOW_HEIGHT{ 1.0f };
constexpr const float DEFAULT_SWINDOW_TOP{ 0.0f };
constexpr const float DEFAULT_SWINDOW_LEFT{ 0.0f };

class SettingsWindow
{
public:
    SettingsWindow();
    SettingsWindow(const SettingsWindow& sw);
    ~SettingsWindow();

    void draw();
    void addMenu(std::shared_ptr<IMenu> menu) { m_tabs.push_back(menu); if (m_tabs.size() == 1) m_currentTabName = std::string(menu->name()); };

    void setCurrentTabMenu(const std::string& tabName);
    void toggleWindowCollapsed();

    inline const std::string& currentTabMenu() const { return m_currentTabName; };
    constexpr bool isWindowCollapsed() const { return m_windowCollapsed; };

private:
    void setup();

    inline const bool isTabMenuAlreadySelected(const std::shared_ptr<IMenu>& menu) const { return std::string(menu->name()) == m_currentTabName; };

private:
    float                                               m_width{DEFAULT_SWINDOW_WIDTH};
    float                                               m_height{DEFAULT_SWINDOW_HEIGHT};
    float                                               m_top{DEFAULT_SWINDOW_TOP};
    float                                               m_left{DEFAULT_SWINDOW_LEFT};
    std::vector<std::shared_ptr<IMenu>>                 m_tabs{};
    bool                                                m_windowCollapsed{ false };
    bool                                                m_newTabToggled{ false };
    std::string                                         m_currentTabName{ "" };
    bool                                                m_init{ false };
};