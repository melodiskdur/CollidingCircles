#pragma once

#include <iostream>
#include <memory>

enum class MOUSE_EVENT_TYPE { LEFT_CLICK, LEFT_RELEASE, RIGHT_CLICK, RIGHT_RELEASE, SCROLL_UP, SCROLL_DOWN, MOVE };

class MouseInputEvent
{
public:
    MouseInputEvent(const MOUSE_EVENT_TYPE& type, const double& x, const double& y, const int& button, const double& scroll)
        : m_eventType(type), m_x(x), m_y(y), m_button(button), m_scroll(scroll)
    { /* ... */ }

    MouseInputEvent(const MOUSE_EVENT_TYPE& type, const double& x, const double& y, const int& button, const int& action)
        : m_eventType(type), m_x(x), m_y(y), m_button(button), m_action(action)
    { /* ... */ }

    MouseInputEvent(const MOUSE_EVENT_TYPE& type, const double& x, const double& y)
        : m_eventType(type), m_x(x), m_y(y)
    { /* ... */ }

    MouseInputEvent(const MOUSE_EVENT_TYPE& type, const double& x, const double& y, const double& scroll)
        : m_eventType(type), m_x(x), m_y(y), m_scroll(scroll)
    { /* ... */ }

    constexpr MOUSE_EVENT_TYPE eventType() const { return m_eventType; }
    constexpr double x() const { return m_x; }
    constexpr double y() const { return m_y; }
    constexpr int button() const {return m_button; }
    constexpr double scroll() const { return m_scroll; }
    constexpr int action() const { return m_action; }

private:
    MOUSE_EVENT_TYPE    m_eventType{};
    int                 m_action{};
    double              m_x{};
    double              m_y{};
    int                 m_button{};
    double              m_scroll{};
};