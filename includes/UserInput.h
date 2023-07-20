#pragma once

#include <memory>
#include <vector>
#include <map>
#include <deque>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "MouseInputEvent.h"

constexpr static const std::size_t INPUT_QUEUE_MAX{ 10 };

enum class INPUT_TYPE { MOUSE, KEYBOARD, NONE };

class UserInput
{
public:
    UserInput(GLFWwindow* parentWindow);
    UserInput(const UserInput& uin);
    ~UserInput();

    void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
    void onMouseScrollEvent(GLFWwindow* window, double dx, double dy);
    void onMouseMoveEvent(GLFWwindow* window, double dx, double dy);

    std::unique_ptr<MouseInputEvent> popMouseInputEvent();

private:
    void redirectCallbacks();
    std::pair<double, double> cursorPos();
    void push(MouseInputEvent&& mouseInputEvent);

private:
    GLFWwindow*                                                 m_window{};
    std::deque<MouseInputEvent>                                 m_mouseInputQueue{};
    std::map<std::pair<int, int>, MOUSE_EVENT_TYPE>             m_btnActionPairs
    {
        {std::make_pair(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS), MOUSE_EVENT_TYPE::RIGHT_CLICK},
        {std::make_pair(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE), MOUSE_EVENT_TYPE::RIGHT_RELEASE},
        {std::make_pair(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS), MOUSE_EVENT_TYPE::LEFT_CLICK},
        {std::make_pair(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE), MOUSE_EVENT_TYPE::LEFT_RELEASE}
    };
};