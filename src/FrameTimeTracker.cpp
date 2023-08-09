#include "FrameTimeTracker.h"

FrameTimeTracker::FrameTimeTracker()
    : m_prevTime(std::chrono::steady_clock::now())
{ /* ... */ }

FrameTimeTracker::~FrameTimeTracker()
{ /* ... */}

void FrameTimeTracker::waitForEndOfFrame()
{
    m_currentTime = std::chrono::steady_clock::now();
    while (std::chrono::duration<float>(m_currentTime - m_prevTime).count() < m_frameDuration)
        m_currentTime = std::chrono::steady_clock::now();
    m_prevTime = m_currentTime;
}