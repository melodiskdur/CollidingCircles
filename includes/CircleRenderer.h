#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "CircleObject.h"
#include "ScreenObject.h"
#include "ShaderProgram.h"
#include "CircleUniformBuffer.h"

class CircleRenderer
{
public:
    CircleRenderer();
    CircleRenderer(std::shared_ptr<ShaderProgram>, std::shared_ptr<ScreenObject> screenObject);
    CircleRenderer(const CircleRenderer&);
    ~CircleRenderer();

    void init();
    void render(const std::vector<CircleObject>& circles);
    std::shared_ptr<ShaderProgram> shaderProgram() const { return m_shaderProgram; };
    std::shared_ptr<ScreenObject> screenObject() const { return m_screenObject; };

    void scaleViewportToWindow(GLFWwindow* window, int width, int height);

private:
    std::shared_ptr<ScreenObject>               m_screenObject{};
    std::shared_ptr<ShaderProgram>              m_shaderProgram{};
    std::string                                 m_fragment_shader{};
    std::string                                 m_vertex_shader{};
};