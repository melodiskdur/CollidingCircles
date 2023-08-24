#pragma once

#include <memory>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "MouseInputEvent.h"
#include "UserInput.h"

class InputManager
{
public:
    InputManager();
    InputManager(const InputManager& ipm);
    ~InputManager();

    void setUserInput(std::shared_ptr<UserInput> userInput) { m_userInput = userInput; }
    void setLeftClickDragCallback(std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec2&)> callback) { m_fLeftClickDrag = callback; }
    void setLeftClickReleaseCallback(std::function<void(const glm::vec2&, const glm::vec2&)> callback) { m_fLeftClickRelease = callback; }
    void setRightClickDragCallback(std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec2&)> callback) { m_fRightClickDrag = callback; }
    void setRightClickReleaseCallback(std::function<void(const glm::vec2&, const glm::vec2&)> callback) { m_fRightClickRelease = callback; }
    void setScrollCallback(std::function<void(const glm::vec2&, const GLfloat&)> callback) { m_fScroll = callback; }

    void setKeyPressedCallback(const int& key, std::function<void(const glm::vec2&)> callback) { m_fKeyPressedCallbacks[key] = callback; };
    void setKeyHeldCallback(const int& key, std::function<void(const glm::vec2&)> callback) { m_fKeyHeldCallbacks[key] = callback; };
    void setKeyReleasedCallback(const int& key, std::function<void(const glm::vec2&)> callback) { m_fKeyReleasedCallbacks[key] = callback; };

    void handleInputEvents();

private:
    void chooseCallbackByMouseEventType(const std::unique_ptr<MouseInputEvent>& mie);
    void chooseCallbackByKeyEventType(const std::unique_ptr<KeyInputEvent>& kie);
    void onLeftClickDrag();
    void onLeftClickRelease();
    void onRightClickDrag();
    void onRightClickRelease();
    void onScroll();

private:
    std::shared_ptr<UserInput>                                                  m_userInput{};
    bool                                                                        m_leftClickActive{ false };
    bool                                                                        m_rightClickActive{ false };

    GLfloat                                                                     m_scroll{};
    glm::vec2                                                                   m_leftClickPos{};
    glm::vec2                                                                   m_rightClickPos{};
    glm::vec2                                                                   m_prevCursorPos{};
    glm::vec2                                                                   m_cursorPos{};

    std::function<void(const glm::vec2& clickPos, const glm::vec2& currentPos, const glm::vec2& dpos)>  m_fLeftClickDrag{};
    std::function<void(const glm::vec2& clickPos, const glm::vec2& releasePos)>                         m_fLeftClickRelease{};
    std::function<void(const glm::vec2& clickPos, const glm::vec2& currentPos, const glm::vec2& dpos)>  m_fRightClickDrag{};
    std::function<void(const glm::vec2& clickPos, const glm::vec2& releasePos)>                         m_fRightClickRelease{};
    std::function<void(const glm::vec2& cursorPos, const GLfloat& zoom)>                                m_fScroll{};

    std::array<std::function<void(const glm::vec2&)>, GLFW_KEY_LAST>    m_fKeyPressedCallbacks{ };
    std::array<std::function<void(const glm::vec2&)>, GLFW_KEY_LAST>    m_fKeyHeldCallbacks{ };
    std::array<std::function<void(const glm::vec2&)>, GLFW_KEY_LAST>    m_fKeyReleasedCallbacks{ };
};