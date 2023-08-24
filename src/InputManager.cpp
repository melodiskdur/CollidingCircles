#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{

}

InputManager::InputManager(const InputManager& ipm)
{
    *this = ipm;
}

InputManager::~InputManager()
{

}

void InputManager::handleInputEvents()
{
    if (!m_userInput)
        return;

    while (auto& mouseInputEvent{ m_userInput->popMouseInputEvent() })
    {
        GLfloat x{ static_cast<GLfloat>(mouseInputEvent->x()) };
        GLfloat y{ static_cast<GLfloat>(mouseInputEvent->y()) };

        if (mouseInputEvent->eventType() == MOUSE_EVENT_TYPE::LEFT_CLICK)
            m_leftClickPos = glm::vec2(x, y);
        else if (mouseInputEvent->eventType() == MOUSE_EVENT_TYPE::RIGHT_CLICK)
            m_rightClickPos = glm::vec2(x, y);
        else
            m_cursorPos = glm::vec2(x, y);
        m_scroll = static_cast<GLfloat>(mouseInputEvent->scroll());
        chooseCallbackByMouseEventType(mouseInputEvent);
    }

    while (auto& keyInputEvent{ m_userInput->popKeyInputEvent() })
    {
        GLfloat x{ static_cast<GLfloat>(keyInputEvent->mouseX()) };
        GLfloat y{ static_cast<GLfloat>(keyInputEvent->mouseY()) };
        m_cursorPos = glm::vec2(x, y);
        chooseCallbackByKeyEventType(keyInputEvent);
    }

    m_prevCursorPos = m_cursorPos;
}

void InputManager::chooseCallbackByMouseEventType(const std::unique_ptr<MouseInputEvent>& mie)
{
    switch (mie->eventType())
        {
            case MOUSE_EVENT_TYPE::LEFT_CLICK:      m_leftClickActive = true; break;
            case MOUSE_EVENT_TYPE::RIGHT_CLICK:     m_rightClickActive = true; break;
            case MOUSE_EVENT_TYPE::LEFT_RELEASE:    m_leftClickActive = false;
                                                    onLeftClickRelease();
                                                    break;
            case MOUSE_EVENT_TYPE::RIGHT_RELEASE:   m_rightClickActive = false;
                                                    onRightClickRelease();
                                                    break;
            case MOUSE_EVENT_TYPE::MOVE:            if (m_leftClickActive) onLeftClickDrag();
                                                    if (m_rightClickActive) onRightClickDrag();
                                                    break;
            case MOUSE_EVENT_TYPE::SCROLL_UP:
            case MOUSE_EVENT_TYPE::SCROLL_DOWN:     onScroll();
                                                    break;
        };
}

void InputManager::chooseCallbackByKeyEventType(const std::unique_ptr<KeyInputEvent>& kie)
{
    switch (kie->inputType())
    {
        case KEY_INPUT_TYPE::PRESSED:   if (m_fKeyPressedCallbacks[kie->key()]) m_fKeyPressedCallbacks[kie->key()](m_cursorPos);
        case KEY_INPUT_TYPE::HELD:      if (m_fKeyHeldCallbacks[kie->key()]) m_fKeyHeldCallbacks[kie->key()](m_cursorPos);
        case KEY_INPUT_TYPE::RELEASED:  if (m_fKeyReleasedCallbacks[kie->key()]) m_fKeyReleasedCallbacks[kie->key()](m_cursorPos);
    }
}

void InputManager::onLeftClickDrag()
{
    if (m_fLeftClickDrag)
        m_fLeftClickDrag(m_leftClickPos, m_cursorPos, m_prevCursorPos);
}

void InputManager::onLeftClickRelease()
{
    if (m_fLeftClickRelease)
        m_fLeftClickRelease(m_leftClickPos, m_cursorPos);
}

void InputManager::onRightClickDrag()
{
    if (m_fRightClickDrag)
        m_fRightClickDrag(m_rightClickPos, m_cursorPos, m_prevCursorPos);
}

void InputManager::onRightClickRelease()
{
    if (m_fRightClickRelease)
        m_fRightClickRelease(m_rightClickPos, m_cursorPos);
}

void InputManager::onScroll()
{
    if (m_fScroll)
        m_fScroll(m_cursorPos, m_scroll);
}