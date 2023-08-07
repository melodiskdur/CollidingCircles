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
#include "CircleRenderer.h"
#include "BloomRenderer.h"
#include "ScreenObject.h"
#include "ShaderProgram.h"
#include "InputManager.h"
#include "UserInput.h"
#include "View.h"
#include "SettingsWindow.h"
#include "GridRenderer.h"
#include "World.h"
#include "CollisionDetectionGrid.h"
#include "GravityCalculator.h"
#include "TimeFlow.h"
#include "LineObject.h"
#include "LineRenderer.h"
#include "CircleRendererInstanced.h"

class Simulation
{
public:
    Simulation();
    ~Simulation();

    void updateViewPosition(const glm::vec2& clickPos, const glm::vec2& currentPos);
    void setScreenDimensions(const GLuint& width, const GLuint& height);
    void onWindowResized(GLFWwindow* window, int w, int h);
    void resizeWindow();
    void init();
    void run();

private:
    glm::vec2 worldToWindowCoordinates(const glm::vec2& clickPos);
    glm::vec2 windowToWorldCoordinates(const glm::vec2& clickPos);

private:
    GLFWwindow*                                 m_window{};
    std::deque<CircleObject>                    m_newCircles{};
    std::shared_ptr<View>                       m_view{};
    std::shared_ptr<World>                      m_world{};
    std::shared_ptr<CollisionDetectionGrid>     m_collisionDetection{};
    std::shared_ptr<GravityCalculator>          m_gravity{};
    std::shared_ptr<TimeFlow>                   m_timeFlow{};
    std::shared_ptr<CircleRenderer>             m_circleRenderer{};
    std::shared_ptr<BloomRenderer>              m_bloomRenderer{};
    std::shared_ptr<GridRenderer>               m_gridRenderer{};
    std::shared_ptr<InputManager>               m_inputManager{};
    std::shared_ptr<UserInput>                  m_userInput{};
    std::shared_ptr<SettingsWindow>             m_settingsWindow{};
    GLuint                                      m_width{};
    GLuint                                      m_height{};

    std::shared_ptr<LineRenderer>               m_lineRenderer{};
    std::vector<LineObject>                     m_lines{ };
    std::vector<LineObject>                     m_worldBorder{ };

    std::shared_ptr<CircleRendererInstanced>    m_circleRendererInstanced{};

};