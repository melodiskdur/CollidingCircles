#pragma once

#include <functional>
#include "glm/glm.hpp"

class IPatternGenerator
{
public:
    virtual const char* name() const = 0;
    virtual void generate() = 0;

protected:
    std::function<void(glm::vec2&& wp, glm::vec2&& vel)>           m_generatorCallback{ };
};