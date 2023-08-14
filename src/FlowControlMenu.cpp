#include "FlowControlMenu.h"
#include <iostream>

FlowControlMenu::FlowControlMenu()
{

}

FlowControlMenu::~FlowControlMenu()
{

}

void FlowControlMenu::draw()
{
    drawHead();
    verticalSpacing(3);
    indent(1);
    drawDspSimulationTime();
    drawBtnState();
    ImGui::SameLine();
    drawBtnStepForward();
    verticalSpacing(3);
    drawBtnReset();
    verticalSpacing(3);
    drawSldrTimeStep();
    verticalSpacing(3);
}

void FlowControlMenu::drawHead() const
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void FlowControlMenu::drawBtnState()
{
    if (ImGui::Button(m_isRunning ? m_btnStateRun.c_str() : m_btnStateStop.c_str(), ImVec2(80.0f, 0.0f)))
    {
        m_flowControlParams->m_simState = m_flowControlParams->m_simState == SIM_STATE::RUN ? SIM_STATE::STOP : SIM_STATE::RUN;
        m_isRunning = !m_isRunning;
    }
}

void FlowControlMenu::drawSldrTimeStep() const
{
    if (m_flowControlParams->m_timeStepRef)
    {
        ImGui::Text(m_sldrTimestep.c_str());
        ImGui::SliderFloat("[ms]", m_flowControlParams->m_timeStepRef, FlowControlParams::TIMESTEP_MIN, FlowControlParams::TIMESTEP_MAX);
    }
}

void FlowControlMenu::drawBtnReset() const
{
    if (ImGui::Button(m_btnReset.c_str()) && m_btnResetCallback)
        m_btnResetCallback();
}

void FlowControlMenu::drawBtnStepForward()
{
    if (ImGui::Button(m_btnStepForward.c_str()))
    {
        m_isRunning = false;
        m_flowControlParams->m_simState = SIM_STATE::STEP;
    }
}

void FlowControlMenu::drawDspSimulationTime() const
{
    if (m_flowControlParams->m_timeRef)
        ImGui::Text("Elapsed Simulation Time: %.2f [s]", 0.001f * (*m_flowControlParams->m_timeRef));
}

