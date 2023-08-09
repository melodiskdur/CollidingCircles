#pragma once

#include <chrono>

class FrameTimeTracker
{
public:
    FrameTimeTracker();
    ~FrameTimeTracker();

    void waitForEndOfFrame();

    constexpr void setFPSLimit(const unsigned int& fps) { m_frameDuration = 1.0f / static_cast<float>(fps); };

private:
    std::chrono::steady_clock::time_point       m_prevTime{};
    std::chrono::steady_clock::time_point       m_currentTime{};
    float                                       m_frameDuration{};
};