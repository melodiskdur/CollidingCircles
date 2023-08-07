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
    if (!m_colorReference)
        return;
    ImGui::Text(m_colorHeader.c_str());
    ImGui::ColorEdit3(m_colorText.c_str(), (float*)m_colorReference);
}

void CircleCreatorMenu::drawRadiusSelector()
{
    if (!m_radiusReference)
        return;
    ImGui::Text(m_radiusHeader.c_str());
    ImGui::SliderFloat("[m]", m_radiusReference, 0.5f, 40.0f);
    *m_massReference = std::max(0.5f * (*m_radiusReference), std::min(*m_massReference, 1.5f * (*m_radiusReference)));
}

void CircleCreatorMenu::drawMassSelector()
{
    if (!m_massReference)
        return;
    ImGui::Text(m_massHeader.c_str());
    ImGui::SliderFloat("[kg]", m_massReference, 0.5f * (*m_radiusReference), 1.5f * (*m_radiusReference));
}

void CircleCreatorMenu::drawStationaryCheckBox()
{
    if (ImGui::Checkbox(m_stationaryHeader.c_str(), &m_stationaryChecked) && m_stationaryCheckedCallback)
        m_stationaryCheckedCallback(m_stationaryChecked);
}

void CircleCreatorMenu::drawNumCircles()
{
    ImGui::Text("Number of circle: %d", *m_numCirclesReference);
}