#pragma once

#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include <cstdlib>
#include <time.h>
#include <ctime>
#include <deque>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>

#include "CircleObject.h"
#include "BloomRenderer.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "UserInput.h"
#include "View.h"
#include "SettingsWindow.h"
#include "GridRenderer.h"
#include "World.h"
#include "TimeFlow.h"
#include "LineObject.h"
#include "LineRenderer.h"
#include "CircleRendererInstanced.h"
#include "FlowControlMenu.h"
#include "ShaderSettingsMenu.h"
#include "CircleCreatorMenu.h"
#include "CircleCreatorSettings.h"
#include "FrameTimeTracker.h"
#include "RendererManager.h"
#include "ShaderSettingsParams.h"
#include "CircleQuadTree.h"
#include "CircleQuadVisualizer.h"
#include "PhysicsSettingsMenu.h"
#include "PhysicsSettingsParams.h"
#include "FlowControlParams.h"
#include "CircleCreator.h"

class Simulation
{
public:
    Simulation();
    ~Simulation();

    void updateViewPosition(const glm::vec2& clickPos, const glm::vec2& currentPos);
    void onWindowResized(GLFWwindow* window, int w, int h);
    void resizeWindow();
    void init();
    void run();

private:
    void updateSimulation();
    void render();
    void renderImGui();

    [[nodiscard]] bool initializeGLContext();
    [[nodiscard]] bool initializeRenderers();
    [[nodiscard]] bool initializeEnvironmentParams();
    [[nodiscard]] bool initializeInput();
    [[nodiscard]] bool initializeImGui();
    [[nodiscard]] bool initializeSettingsMenus();

    void setupMouseInputCallbacks();
    void setupScrollInputCallbacks();
    void setupKeyInputCallbacks();
    void setupFlowControlMenuCallbacks(std::shared_ptr<FlowControlMenu> flowControl);
    void setupShaderSettingsMenuCallbacks(std::shared_ptr<ShaderSettingsMenu> shaderSettings);
    void setupCircleCreatorMenuCallbacks(std::shared_ptr<CircleCreatorMenu> circleCreator);
    void setupPhysicsSettingsMenuCallbacks(std::shared_ptr<PhysicsSettingsMenu> physicsSettings);

private:
    GLFWwindow*                                 m_window{};
    std::shared_ptr<View>                       m_view{};
    std::shared_ptr<World>                      m_world{};
    std::shared_ptr<TimeFlow>                   m_timeFlow{};
    std::shared_ptr<InputManager>               m_inputManager{};
    std::shared_ptr<UserInput>                  m_userInput{};
    std::shared_ptr<SettingsWindow>             m_settingsWindow{};
    std::shared_ptr<CircleCreator>              m_circleCreator{};
    std::shared_ptr<CircleCreatorSettings>      m_creatorSettings{};
    std::shared_ptr<ShaderSettingsParams>       m_shaderSettingsParams{};
    std::shared_ptr<PhysicsSettingsParams>      m_physicsParams{};
    std::shared_ptr<FlowControlParams>          m_flowControlParams{};
    std::shared_ptr<FrameTimeTracker>           m_frameTimeTracker{};
    std::shared_ptr<RendererManager>            m_renderManager{};

    GLuint                                      m_width{};
    GLuint                                      m_height{};

    std::vector<LineObject>                     m_lines{ };
    std::vector<LineObject>                     m_worldBorder{ };
};