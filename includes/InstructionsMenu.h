#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "IMenu.h"
#include <map>

class InstructionsMenu : public IMenu
{
public:
    InstructionsMenu();
    ~InstructionsMenu();
    const char* name() const override { return m_name.c_str(); }
    void draw() override;

    void addInstructionsPair(std::string&& action, std::string&& key) { m_instructions.insert({ std::move(action), std::move(key) }); };

private:
    void drawHead() const;
    void drawInstructionPairs() const;
    constexpr void verticalSpacing(const std::size_t& n) const { for(auto i{0}; i < n; i++) ImGui::Spacing(); };

private:
    std::string                                 m_name{ "Instructions Menu" };
    std::map<std::string, std::string>          m_instructions{ };
};