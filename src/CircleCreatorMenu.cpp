#include "CircleCreatorMenu.h"

CircleCreatorMenu::CircleCreatorMenu()
{

}
CircleCreatorMenu::~CircleCreatorMenu()
{

}
void CircleCreatorMenu::draw()
{
    drawHead();
    verticalSpacing(3);
    indent(1);
    drawColorSelector();
    verticalSpacing(4);
    drawRadiusSelector();
    verticalSpacing(4);
    drawMassSelector();
    verticalSpacing(3);
    drawStationaryCheckBox();
    verticalSpacing(3);
    drawNumCircles();
}

void CircleCreatorMenu::drawHead() const
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void CircleCreatorMenu::drawColorSelector()
{
    if (!m_circleCreatorSettings->circleColorRef())
        return;
    ImGui::Text(m_colorHeader.c_str());
    ImGui::ColorEdit3(m_colorText.c_str(), (float*)m_circleCreatorSettings->circleColorRef());
}

void CircleCreatorMenu::drawRadiusSelector()
{
    if (!m_circleCreatorSettings->circleRadiusRef())
        return;
    ImGui::Text(m_radiusHeader.c_str());
    ImGui::SliderFloat("[m]", m_circleCreatorSettings->circleRadiusRef(),
                            CircleCreatorSettings::CIRCLE_RADIUS_MIN,
                            CircleCreatorSettings::CIRCLE_RADIUS_MAX);
}

void CircleCreatorMenu::drawMassSelector()
{
    if (!m_circleCreatorSettings->circleMassRef())
        return;
    ImGui::Text(m_massHeader.c_str());
    ImGui::SliderFloat("[kg]", m_circleCreatorSettings->circleMassRef(),
                            CircleCreatorSettings::CIRCLE_MASS_MIN,
                            CircleCreatorSettings::CIRCLE_MASS_MAX);
}

void CircleCreatorMenu::drawStationaryCheckBox()
{
    ImGui::Checkbox(m_stationaryHeader.c_str(), m_circleCreatorSettings->stationaryCheckedRef());
}

void CircleCreatorMenu::drawNumCircles()
{
    ImGui::Text("Number of circle: %d", *m_circleCreatorSettings->numCirclesRef());
}