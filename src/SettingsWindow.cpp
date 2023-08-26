#include "SettingsWindow.h"

#include <algorithm>
#include <iostream>

SettingsWindow::SettingsWindow()
{
}

SettingsWindow::SettingsWindow(const SettingsWindow& sw)
{
    this->m_tabs = sw.m_tabs;
}

SettingsWindow::~SettingsWindow()
{

}

void SettingsWindow::draw()
{
    setup();
    if (m_newTabToggled)
        ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
    ImGui::Begin("SettingsWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    m_windowCollapsed = ImGui::IsWindowCollapsed();
    ImGui::BeginTabBar("Tabs");
    if (m_newTabToggled)
    {
        std::for_each(m_tabs.begin(), m_tabs.end(), [&](std::shared_ptr<IMenu>& tabMenu)
        {
            if (std::string(tabMenu->name()) == m_currentTabName)
            {
                ImGui::BeginTabItem(tabMenu->name(), 0, ImGuiTabItemFlags_SetSelected);
            }
        });
        m_newTabToggled = false;
    }
    std::for_each(m_tabs.begin(), m_tabs.end(), [&](std::shared_ptr<IMenu>& tabMenu)
    {
        if (ImGui::BeginTabItem(tabMenu->name()))
        {
            tabMenu->draw();
            m_currentTabName = std::string(tabMenu->name());
            ImGui::EndTabItem();
        }
    });
    ImGui::EndTabBar();
    ImGui::End();
}

void SettingsWindow::setCurrentTabMenu(const std::string& tabName)
{
    for (const std::shared_ptr<IMenu>& menu : m_tabs)
        if (std::string(menu->name()) == tabName)
        {
            m_newTabToggled = ( true ^ (tabName == m_currentTabName) ) || m_windowCollapsed;
            m_currentTabName = tabName;
            break;
        }
}

void SettingsWindow::toggleWindowCollapsed()
{
    ImGui::Begin("SettingsWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowCollapsed(!ImGui::IsWindowCollapsed());
    ImGui::End();
}

void SettingsWindow::setup()
{
    ImGuiStyle& style{ ImGui::GetStyle() };
    style.WindowPadding = ImVec2(0, 0);
    style.WindowPadding = ImVec2(0, 0);
    style.WindowRounding = 0.0f;
    style.FramePadding = ImVec2(0, 0);
    style.FrameRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    ImVec2& dispSize{ ImGui::GetIO().DisplaySize };
    ImGui::SetNextWindowSize(ImVec2(dispSize.x * m_width, dispSize.y * m_height));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    if (!m_init)
    {
        ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
        m_init = true;
    }
}