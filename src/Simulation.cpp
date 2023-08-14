#include "Simulation.h"
#include "FlowControlMenu.h"
#include "ShaderSettingsMenu.h"
#include "CircleCreatorMenu.h"
#include "CircleRenderData.h"
#include <chrono>

constexpr const GLuint INIT_WIDTH{ 1280 };
constexpr const GLuint INIT_HEIGHT{ 720 };
constexpr glm::vec2 WORLD_SIZE{ 10000.0f, 10000.0f };
constexpr GLuint NUM_CIRCLES{ 0 };
constexpr const float FRAME_LENGTH{ 1.0f / 60.0f };

static std::size_t SHADER_TYPE{ 0 };

static GLuint rWidth{};
static GLuint rHeight{};

static bool TO_RESIZE{ false };

void Simulation::onWindowResized(GLFWwindow* window, int w, int h)
{
	TO_RESIZE = true;
	rWidth = static_cast<GLuint>(w);
	rHeight = static_cast<GLuint>(h);
}

void Simulation::resizeWindow()
{
	glViewport(0, 0, rWidth, rHeight);
	m_width = rWidth;
	m_height = rHeight;
	m_view->setDimensions(glm::vec2(m_width, m_height));
	m_renderManager->setViewportSize(m_width, m_height);
	m_renderManager->bloomRenderer()->setViewportSize(m_width, m_height);
	m_renderManager->bloomRenderer()->resizeTextures();
	TO_RESIZE = false;
}

Simulation::Simulation() : m_width(INIT_WIDTH), m_height(INIT_HEIGHT)
{ /* ... */ }

Simulation::~Simulation()
{ /* ... */ }

void Simulation::updateViewPosition(const glm::vec2& clickPos, const glm::vec2& currentPos)
{
	glm::vec2 newPos{ currentPos - clickPos };
	m_view->setPosition(newPos);
}

void Simulation::init()
{
	if (!this->initializeGLContext())
	{
		std::cerr << "Simulation::init(): Failed to initialize OpenGL and GLFW context (Simulation::initializeGLContext()).\n";
		exit(EXIT_FAILURE);
	}

	if (!this->initializeRenderers())
	{
		exit(EXIT_FAILURE);
	}

	if (!this->initializeEnvironmentParams())
	{
		exit(EXIT_FAILURE);
	}

	if (!this->initializeInput())
	{
		exit(EXIT_FAILURE);
	}

	if (!this->initializeImGui())
	{
		exit(EXIT_FAILURE);
	}

	if (!this->initializeSettingsMenus())
	{
		exit(EXIT_FAILURE);
	}

	m_frameTimeTracker = std::make_shared<FrameTimeTracker>();
	m_frameTimeTracker->setFPSLimit(60);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Simulation::run()
{
    while (!glfwWindowShouldClose(m_window))
	{
		if (TO_RESIZE)
			resizeWindow();

		m_inputManager->handleInputEvents();
		updateSimulation();
		glfwPollEvents();
		render();
		renderImGui();

		glfwSwapBuffers(m_window);

		if (m_flowControlParams->m_simState == SIM_STATE::STEP)
			m_flowControlParams->m_simState = SIM_STATE::STOP;

		m_frameTimeTracker->waitForEndOfFrame();
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Simulation::updateSimulation()
{
	std::shared_ptr<std::vector<CircleObject>> m_circles{ m_world->circles() };
	// New circles.
	if (m_newCircles.size() > 0)
	{
		m_circles->insert(m_circles->end(), m_newCircles.begin(), m_newCircles.end());
		m_newCircles.clear();
	}
	// Update world state (includes physics).
	if (m_flowControlParams->m_simState != SIM_STATE::STOP)
	{
		m_timeFlow->updateTime();
		m_world->updateWorldState(m_timeFlow->deltaTime());
	}
	*(m_creatorSettings->numCirclesRef()) = m_circles->size();
}

void Simulation::render()
{
	std::vector<CircleRenderData> circleRenderData(m_world->circles()->size());
	std::shared_ptr<std::vector<CircleObject>> m_circles{ m_world->circles() };
	glm::mat4 viewProjectionMatrix{ m_view->viewProjectionMatrix() };
	for (int i{0}; i < m_circles->size(); i++)
	{
		circleRenderData[i] = CircleRenderData{
			m_circles->at(i).pos(),
			m_circles->at(i).color(),
			m_circles->at(i).radius()
			};
	}
	// Circle renderer.
	m_renderManager->renderCircles(circleRenderData, viewProjectionMatrix);
	// Grid renderer.
	glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(m_world->worldCenter(), 0.0f)) };
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f * WORLD_SIZE, 0.0));
	m_renderManager->renderGrid(modelMatrix, viewProjectionMatrix);
	// Line renderer.
	if (m_lines.size() > 0)
		m_renderManager->renderLines(m_lines, viewProjectionMatrix);
	m_renderManager->renderLines(m_worldBorder, viewProjectionMatrix);
}

void Simulation::renderImGui()
{
	// ImGui frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_settingsWindow->draw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


[[nodiscard]] bool Simulation::initializeGLContext()
{
	if (!glfwInit())
	{
		glfwTerminate();
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "Colliding Circles", NULL, NULL);
	if (!m_window) {
		glfwTerminate();
		return false;
	}
    glfwMakeContextCurrent(m_window);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
    glewExperimental = GL_TRUE;
    glewInit();
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
	auto f_onWindowResized{[](GLFWwindow* window, int w, int h)
    {
        static_cast<Simulation*>(glfwGetWindowUserPointer(window))->onWindowResized(window, w, h);
    }};
	glfwSetFramebufferSizeCallback(m_window, f_onWindowResized);
	return true;
}

[[nodiscard]] bool Simulation::initializeRenderers()
{
	m_renderManager = std::make_shared<RendererManager>();
	m_renderManager->setViewportSize(m_width, m_height);
	if (!m_renderManager->initRenderers())
		return false;
	return true;
}

[[nodiscard]] bool Simulation::initializeEnvironmentParams()
{
	m_view = std::make_shared<View>(glm::vec2(static_cast<GLfloat>(m_width), static_cast<GLfloat>(m_height)));
	m_view->setPosition({-125.f, -125.f});
	m_world = std::make_shared<World>();
	m_world->setWorldDimensions(WORLD_SIZE);
	m_world->init();
	m_timeFlow = std::make_shared<TimeFlow>();
	m_timeFlow->setDeltaTime(0.3f);

	// World border.
	glm::vec3 borderColor{ 1.0f, 0.0f, 0.0f };
	m_worldBorder = std::vector<LineObject>
	{
		LineObject(glm::vec2(0.f), glm::vec2(0.f, m_world->worldDimensions().y), glm::vec3(borderColor)),
		LineObject(glm::vec2(0.f, m_world->worldDimensions().y), glm::vec2(m_world->worldDimensions().x, m_world->worldDimensions().y), glm::vec3(borderColor)),
		LineObject(glm::vec2(m_world->worldDimensions().x, m_world->worldDimensions().y), glm::vec2(m_world->worldDimensions().x, 0.f), glm::vec3(borderColor)),
		LineObject(glm::vec2(m_world->worldDimensions().x, 0.f), glm::vec2(0.f), glm::vec3(borderColor))
	};

	return true;
}

[[nodiscard]] bool Simulation::initializeInput()
{
	m_userInput = std::make_shared<UserInput>(UserInput(m_window));
	m_inputManager = std::make_shared<InputManager>();
	m_inputManager->setUserInput(m_userInput);
	setupMouseInputCallbacks();
	setupScrollInputCallbacks();

	return true;
}

[[nodiscard]] bool Simulation::initializeImGui()
{
	// Initialize ImGui.
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	return true;
}

[[nodiscard]] bool Simulation::initializeSettingsMenus()
{
	m_settingsWindow = std::make_shared<SettingsWindow>(SettingsWindow());
	// FlowControl.
	std::shared_ptr<FlowControlMenu> flowControl{ std::make_shared<FlowControlMenu>() };
	m_flowControlParams = std::make_shared<FlowControlParams>(m_timeFlow->timeReference(), m_timeFlow->deltaTimeReference());
	flowControl->setFlowControlParams(m_flowControlParams);
	setupFlowControlMenuCallbacks(flowControl);
	m_settingsWindow->addMenu(flowControl);
	// ShaderSettings.
	std::shared_ptr<ShaderSettingsMenu> shaderSettings{ std::make_shared<ShaderSettingsMenu>() };
	setupShaderSettingsMenuCallbacks(shaderSettings);
	m_settingsWindow->addMenu(shaderSettings);
	// Circle Creator.
	std::shared_ptr<CircleCreatorMenu> circleCreator{ std::make_shared<CircleCreatorMenu>() };
	setupCircleCreatorMenuCallbacks(circleCreator);
	m_settingsWindow->addMenu(circleCreator);
	// Physics Settings.
	std::shared_ptr<PhysicsSettingsMenu> physicsSettings{ std::make_shared<PhysicsSettingsMenu>() };
	setupPhysicsSettingsMenuCallbacks(physicsSettings);
	m_settingsWindow->addMenu(physicsSettings);
	return true;
}

void Simulation::setupMouseInputCallbacks()
{
	m_inputManager->setRightClickDragCallback(
		[=](const glm::vec2& clickPos, const glm::vec2& cursorPos, const glm::vec2& prevCursorPos)
		{
			glm::vec2 w2wc{ m_view->worldToWindowCoordinates(cursorPos) };
			glm::vec2 w2wpc{ m_view->worldToWindowCoordinates(prevCursorPos) };
			m_view->move(glm::vec2(w2wpc.x, w2wpc.y) - glm::vec2(w2wc.x, w2wc.y));
		});

	m_inputManager->setLeftClickDragCallback(
		[=](const glm::vec2& clickPos, const glm::vec2& currentPos, const glm::vec2& dpos)
		{
			glm::vec2 w2w{ m_view->windowToWorldCoordinates(clickPos) };
			glm::vec2 w2wc{ m_view->windowToWorldCoordinates(currentPos) };
			if (m_lines.size() == 0)
			{
				m_lines.push_back(LineObject(glm::vec2(w2w), glm::vec2(w2wc), glm::vec3(m_creatorSettings->circleColor())));
			}
			else
			{
				m_lines[0].setLine(glm::vec2(w2w), glm::vec2(w2wc));
			}
		});

	m_inputManager->setLeftClickReleaseCallback(
		[=](const glm::vec2& clickPos, const glm::vec2& cursorPos)
		{
			glm::vec2 w2w{ m_view->windowToWorldCoordinates(clickPos) };
			glm::vec2 w2wc{ m_view->windowToWorldCoordinates(cursorPos) };
			glm::vec2 velocity{ 1e-2f * (w2w - w2wc) };
			CircleObject newObj(glm::vec2(w2w), m_creatorSettings->circleColor(), std::move(velocity));
			newObj.setRadius(m_creatorSettings->circleRadius());
			newObj.setMass(m_creatorSettings->circleMass());
			if (m_creatorSettings->stationaryChecked())
				newObj.setStationary(m_creatorSettings->stationaryChecked());

			m_newCircles.push_back(newObj);
			if (m_lines.size() > 0)
			{
				m_lines.pop_back();
			}
		}
	);
}

void Simulation::setupScrollInputCallbacks()
{
	m_inputManager->setScrollCallback(
		[=](const glm::vec2& cursorPos, const GLfloat& zoom)
		{
			GLfloat zoomVal{ zoom < 0 ? 0.90f : 1.10f };
			m_view->zoom(zoomVal);
		});
}

void Simulation::setupFlowControlMenuCallbacks(std::shared_ptr<FlowControlMenu> flowControl)
{
	flowControl->setBtnResetCallback([=]()
	{
		m_world->circles()->clear();
		m_timeFlow->resetTime();
	});
}

void Simulation::setupShaderSettingsMenuCallbacks(std::shared_ptr<ShaderSettingsMenu> shaderSettings)
{
	m_shaderSettingsParams = std::make_shared<ShaderSettingsParams>(
				m_renderManager->bloomRenderer()->glowIntensityRef(),
				m_renderManager->bloomRenderer()->colorIntensityRef());
	shaderSettings->setShaderSettingsParams(m_shaderSettingsParams);
}

void Simulation::setupCircleCreatorMenuCallbacks(std::shared_ptr<CircleCreatorMenu> circleCreator)
{
	m_creatorSettings = std::make_shared<CircleCreatorSettings>();
	circleCreator->setCircleCreatorSettings(m_creatorSettings);
}

void Simulation::setupPhysicsSettingsMenuCallbacks(std::shared_ptr<PhysicsSettingsMenu> physicsSettings)
{
	m_physicsParams = std::make_shared<PhysicsSettingsParams>(m_world->physicsManager()->gravityCalculator()->gravityConstantRef());
	physicsSettings->setPhysicsSettingsParams(m_physicsParams);
}