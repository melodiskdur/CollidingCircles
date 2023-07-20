#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include <cstdlib>
#include <time.h>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm/gtc/type_ptr.hpp>

#include "Simulation.h"

int main()
{
	auto simulation{Simulation()};
	simulation.init();
	simulation.run();
}
