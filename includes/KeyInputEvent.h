#pragma once

#include <array>

enum class KEY_INPUT_TYPE{ PRESSED, HELD, RELEASED };

class KeyInputEvent
{
public:
    KeyInputEvent(const int& key, const int& action, const double& x, const double& y, const KEY_INPUT_TYPE& type)
        : m_key(key), m_action(action), m_x(x), m_y(y), m_inputType(type)
    { /* ... */ }

    constexpr int key() const { return m_key; };
    constexpr int action() const { return m_action; };
    constexpr KEY_INPUT_TYPE inputType() const { return m_inputType; };
    constexpr double mouseX() const { return m_x; };
    constexpr double mouseY() const { return m_y; };

private:
    int                 m_key{ };
    int                 m_action{ };
    KEY_INPUT_TYPE      m_inputType{ };
    double              m_x{ };
    double              m_y{ };
};