#include "InstructionsMenu.h"
#include <algorithm>

InstructionsMenu::InstructionsMenu() { /* ... */ }

InstructionsMenu::~InstructionsMenu() { /* ... */ }

void InstructionsMenu::draw()
{
    drawHead();
    verticalSpacing(3);
    drawInstructionPairs();
}

void InstructionsMenu::drawHead() const
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(m_name.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), m_name.c_str());
    ImGui::Separator();
}

void InstructionsMenu::drawInstructionPairs() const
{
    std::for_each(m_instructions.begin(), m_instructions.end(),[&](const std::pair<std::string, std::string>& instructionPair)
    {
        std::string outputString{ instructionPair.first};
        ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.2f, 1.0f), instructionPair.first.c_str());
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 0.2f, 0.85f, 1.0f), instructionPair.second.c_str());
        verticalSpacing(2);
    });
}
