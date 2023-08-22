#include "PhysicsSettingsMenu.h"

PhysicsSettingsMenu::PhysicsSettingsMenu()
{ /* ... */ }

PhysicsSettingsMenu::~PhysicsSettingsMenu()
{ /* ... */ }

void PhysicsSettingsMenu::draw()
{
    drawHead();
    verticalSpacing(3);
    indent(1);
    drawCheckboxCollisionDetection();
    verticalSpacing(3);
    drawGravityConstantSelector();
}

void PhysicsSettingsMenu::drawHead() const
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void PhysicsSettingsMenu::drawGravityConstantSelector()
{
    if (!m_physicsSettingsParams->m_gravityConstantRef)
        return;
    ImGui::Text(m_gravityHeader.c_str());
    ImGui::SliderFloat("[]", m_physicsSettingsParams->m_gravityConstantRef,
                        PhysicsSettingsParams::GRAVITY_CONSTANT_MIN,
                        PhysicsSettingsParams::GRAVITY_CONSTANT_MAX,
                        "%.11f",
                        ImGuiSliderFlags_Logarithmic);
}

void PhysicsSettingsMenu::drawCheckboxCollisionDetection()
{
    ImGui::Checkbox(m_chkbxCollisionDetectionEnabled.c_str(), m_physicsSettingsParams->m_collisionDetectionEnabledRef);
}