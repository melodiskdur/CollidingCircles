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
    ImGui::Begin("SettingsWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::BeginTabBar("Tabs");
    std::for_each(m_tabs.begin(), m_tabs.end(), [](std::shared_ptr<IMenu>& tabMenu)
    {
        if (ImGui::BeginTabItem(tabMenu->name()))
        {
            tabMenu->draw();
            ImGui::EndTabItem();
        }
    });
    ImGui::EndTabBar();
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
}