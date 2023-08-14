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

    switch(m_shaderSettingsParams->shaderType())
    {
        case 0: break;
        case 1:
            verticalSpacing(3);
            drawSldrGlowIntensity();
            verticalSpacing(3);
            drawSldrColorIntensity();
            break;
    }
}

void ShaderSettingsMenu::drawHead() const
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void ShaderSettingsMenu::drawCheckboxDisplayGrid()
{
    ImGui::Checkbox(m_chkbxDisplayGrid.c_str(), m_shaderSettingsParams->displayGridAvailableRef());
}

void ShaderSettingsMenu::drawSelectShaderType()
{
    for (std::size_t i{0}; i < m_shaders.size(); i++)
    {
        if (ImGui::Selectable(m_shaders[i].c_str(), m_selected == i))
        {
            *m_shaderSettingsParams->shaderTypeRef() = i;
            m_selected = i;
            break;
        }
    }
}

void ShaderSettingsMenu::drawSldrGlowIntensity()
{
    ImGui::Text(m_sldrGlowIntensityHeader.c_str());
    ImGui::SliderFloat("",
                    m_shaderSettingsParams->glowIntensityRef(),
                    ShaderSettingsParams::GLOW_INTENSITY_MIN,
                    ShaderSettingsParams::GLOW_INTENSITY_MAX);
}

void ShaderSettingsMenu::drawSldrColorIntensity()
{
    ImGui::Text(m_sldrColorIntensityHeader.c_str());
    ImGui::SliderFloat("",
                    m_shaderSettingsParams->colorIntensityRef(),
                    ShaderSettingsParams::COLOR_INTENSITY_MIN,
                    ShaderSettingsParams::COLOR_INTENSITY_MAX);
}