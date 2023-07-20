#include "ShaderSettingsMenu.h"
#include <iostream>

ShaderSettingsMenu::ShaderSettingsMenu()
{

}
ShaderSettingsMenu::~ShaderSettingsMenu()
{

}

void ShaderSettingsMenu::draw()
{
    drawHead();
    verticalSpacing(3);
    indent(1);
    drawCheckboxDisplayGrid();
    verticalSpacing(3);
    drawSelectShaderType();
}

void ShaderSettingsMenu::drawHead() const
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void ShaderSettingsMenu::drawCheckboxDisplayGrid()
{
    if (ImGui::Checkbox(m_chkbxDisplayGrid.c_str(), &m_chckbxDisplayGridIsChecked) && m_chckbxDisplayGridCallback)
        m_chckbxDisplayGridCallback(m_chckbxDisplayGridIsChecked);
}

void ShaderSettingsMenu::drawSelectShaderType()
{
    for (std::size_t i{0}; i < m_shaders.size(); i++)
    {
        if (ImGui::Selectable(m_shaders[i].c_str(), m_selected == i))
        {
            m_selected = i;
            m_slctShaderTypeCallback(i);
        }
    }
}

