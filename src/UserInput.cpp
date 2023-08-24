#include "UserInput.h"
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

UserInput::UserInput(GLFWwindow* parentWindow)
    : m_window(parentWindow)
{ redirectCallbacks(); }

UserInput::UserInput(const UserInput& uin)
    : m_window(uin.m_window)
{ redirectCallbacks(); }

UserInput::~UserInput()
{

}

void UserInput::onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    double x, y;
    auto it{ m_btnActionPairs.find({button, action}) };
    if (it != m_btnActionPairs.end())
        glfwGetCursorPos(window, &x, &y);
        this->push(MouseInputEvent(it->second, x, y, button, 0.0));
}

void UserInput::onMouseScrollEvent(GLFWwindow* window, double dx, double dy)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    MOUSE_EVENT_TYPE scrollType{(dy > 0) ? MOUSE_EVENT_TYPE::SCROLL_UP : MOUSE_EVENT_TYPE::SCROLL_DOWN};
    this->push(MouseInputEvent(scrollType, x, y, dy));
}

void UserInput::onMouseMoveEvent(GLFWwindow* window, double dx, double dy)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    this->push(MouseInputEvent(MOUSE_EVENT_TYPE::MOVE, dx, dy));
}

void UserInput::onKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    KEY_INPUT_TYPE inputType{ };
    switch(action)
    {
        case GLFW_PRESS:    inputType = KEY_INPUT_TYPE::PRESSED; break;
        case GLFW_REPEAT:   inputType = KEY_INPUT_TYPE::HELD; break;
        case GLFW_RELEASE:  inputType = KEY_INPUT_TYPE::RELEASED; break;
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    this->push(KeyInputEvent(key, action, x, y, inputType));
}

std::unique_ptr<MouseInputEvent> UserInput::popMouseInputEvent()
{
    if (m_mouseInputQueue.size() == 0)
        return std::unique_ptr<MouseInputEvent>(nullptr);

    std::unique_ptr<MouseInputEvent> event{ std::make_unique<MouseInputEvent>(m_mouseInputQueue.front()) };
    m_mouseInputQueue.pop_front();
    return event;
}

std::unique_ptr<KeyInputEvent> UserInput::popKeyInputEvent()
{
    if (m_keyInputQueue.empty())
        return std::unique_ptr<KeyInputEvent>(nullptr);
    std::unique_ptr<KeyInputEvent> event{ std::make_unique<KeyInputEvent>(m_keyInputQueue.front()) };
    m_keyInputQueue.pop_front();
    return event;
}

void UserInput::redirectCallbacks()
{
    glfwSetWindowUserPointer(m_window, this);

    auto f_mouseButtonEvent{[](GLFWwindow* window, int btn, int action, int mods)
    {
        static_cast<UserInput*>(glfwGetWindowUserPointer(window))->onMouseButtonEvent(window, btn, action, mods);
    }};

    auto f_mouseScrollEvent{[](GLFWwindow* window, double dx, double dy)
    {
        static_cast<UserInput*>(glfwGetWindowUserPointer(window))->onMouseScrollEvent(window, dx, dy);
    }};

    auto f_mouseMoveEvent{[](GLFWwindow* window, double dx, double dy)
    {
        static_cast<UserInput*>(glfwGetWindowUserPointer(window))->onMouseMoveEvent(window, dx, dy);
    }};

    auto f_keyEvent{ [](GLFWwindow* window, int key, int scanCode, int action, int mods)
    {
        static_cast<UserInput*>(glfwGetWindowUserPointer(window))->onKeyEvent(window, key, scanCode, action, mods);
    }};

    glfwSetMouseButtonCallback(m_window, f_mouseButtonEvent);
    glfwSetScrollCallback(m_window, f_mouseScrollEvent);
    glfwSetCursorPosCallback(m_window, f_mouseMoveEvent);
    glfwSetKeyCallback(m_window, f_keyEvent);
}

std::pair<double, double> UserInput::cursorPos()
{
    return std::pair<double, double>(1, 1);
}

void UserInput::push(MouseInputEvent&& mouseInputEvent)
{
    while (m_mouseInputQueue.size() >= INPUT_QUEUE_MAX)
    {
        m_mouseInputQueue.pop_front();
    }
    m_mouseInputQueue.push_back(mouseInputEvent);
}

void UserInput::push(KeyInputEvent&& keyInputEvent)
{
    while (m_keyInputQueue.size() >= INPUT_QUEUE_MAX)
    {
        m_keyInputQueue.pop_front();
    }
    m_keyInputQueue.push_back(keyInputEvent);
}