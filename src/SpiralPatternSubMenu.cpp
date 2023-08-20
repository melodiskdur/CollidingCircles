#include "SpiralPatternSubMenu.h"

SpiralPatternSubMenu::SpiralPatternSubMenu()
{ /* ... */ }

SpiralPatternSubMenu::~SpiralPatternSubMenu()
{ /* ... */ }

void SpiralPatternSubMenu::draw()
{
    if (!m_params) return;
    drawHead();
    verticalSpacing(3);
    drawSliders();
}

void SpiralPatternSubMenu::drawHead()
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(0.2f, 0.2f, 1.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void SpiralPatternSubMenu::drawSliders()
{
    ImGui::Text("Number of main arms");
    ImGui::SliderInt(" ", &m_params->m_numMainArms,
                            0, SpiralPatternParams::MAINARMS_NUM_MAX);

    verticalSpacing(2);

    ImGui::Text("Number of sub arms");
    ImGui::SliderInt("  ", &m_params->m_numSubArms,
                            0, SpiralPatternParams::SUBARMS_NUM_MAX);

    verticalSpacing(2);

    ImGui::Text("Circles per main arm");
    ImGui::SliderInt("   ", &m_params->m_numCirclesPerMainArm,
                            SpiralPatternParams::MAINARM_NUMCIRCLES_MIN,
                            SpiralPatternParams::MAINARM_NUMCIRCLES_MAX);

    verticalSpacing(2);

    ImGui::Text("Circles per sub arm");
    ImGui::SliderInt("    ", &m_params->m_numCirclesPerSubArm,
                            SpiralPatternParams::SUBARM_NUMCIRCLES_MIN,
                            SpiralPatternParams::SUBARM_NUMCIRCLES_MAX);

    verticalSpacing(2);

    ImGui::Text("Disc radius");
    ImGui::SliderFloat("     ", &m_params->m_discRadius,
                            SpiralPatternParams::DISCRADIUS_MIN,
                            SpiralPatternParams::DISCRADIUS_MAX);

    verticalSpacing(2);

    ImGui::Text("Pattern rotation");
    ImGui::SliderFloat("      ", &m_params->m_initialRotationDeg,
                            0, SpiralPatternParams::PATTERN_ROTATION_MAX);

    verticalSpacing(2);

    ImGui::Text("Arm spiral degree");
    ImGui::SliderFloat("       ", &m_params->m_armMaxDeg,
                            SpiralPatternParams::SPIRAL_DEG_MIN,
                            SpiralPatternParams::SPIRAL_DEG_MAX);

    verticalSpacing(2);

    ImGui::Text("Log constant");
    ImGui::SliderFloat("        ", &m_params->m_logConstant,
                            SpiralPatternParams::LOG_CONSTANT_MIN,
                            SpiralPatternParams::LOG_CONSTANT_MAX);

    verticalSpacing(2);

    ImGui::Text("Fuzz factor");
    ImGui::SliderFloat("         ", &m_params->m_fuzzFactor,
                            SpiralPatternParams::FUZZ_FACTOR_MIN,
                            SpiralPatternParams::FUZZ_FACOTR_MAX);
}
