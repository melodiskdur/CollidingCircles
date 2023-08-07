#include "Simulation.h"
#include "FlowControlMenu.h"
#include "ShaderSettingsMenu.h"
#include "CircleCreatorMenu.h"
#include "CircleRenderData.h"
#include <chrono>

constexpr const GLuint INIT_WIDTH{ 1280 };
constexpr const GLuint INIT_HEIGHT{ 720 };
constexpr glm::vec2 WORLD_SIZE{ 10000.0f, 10000.0f };
constexpr GLuint NUM_CIRCLES{ 1000000 };
constexpr const float FRAME_LENGTH{ 1.0f / 60.0f };
enum class STATE { RUN, STOP, STEP };

static STATE g_sim_state{ STATE::STOP };
static std::size_t SHADER_TYPE{ 0 };

static glm::vec3 CIRCLE_COLOR{ 0.99f, 0.2f, 0.0f };
static GLfloat CIRCLE_RADIUS{ 1.0f };
static GLfloat CIRCLE_MASS{ 1.0f };

static GLuint rWidth{};
static GLuint rHeight{};

static bool STATIONARY_CHECKED{ false };

static bool TO_RESIZE{ false };

static GLsizei N_CIRCLES{0};

void stateBtnCallback()
{
	g_sim_state = g_sim_state == STATE::RUN ? STATE::STOP : STATE::RUN;
}

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
	TO_RESIZE = false;
}

Simulation::Simulation() : m_width(INIT_WIDTH), m_height(INIT_HEIGHT)
{

}

Simulation::~Simulation()
{

}

void Simulation::setScreenDimensions(const GLuint& width, const GLuint& height)
{

}

void Simulation::updateViewPosition(const glm::vec2& clickPos, const glm::vec2& currentPos)
{
	glm::vec2 newPos{ currentPos - clickPos };
	m_view->setPosition(newPos);
}

void Simulation::init()
{
	if (!glfwInit())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "Colliding Circles", NULL, NULL);

	if (!m_window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    glfwMakeContextCurrent(m_window);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
    glewExperimental = GL_TRUE;
    glewInit();

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    std::shared_ptr<ShaderProgram> shaderProgram{ std::make_shared<ShaderProgram>(ShaderProgram()) };
	shaderProgram->addShader(std::string{"simple_vtx"}, std::string("shader/simple.vert"), GL_VERTEX_SHADER);
	shaderProgram->addShader(std::string{"simple_frag"}, std::string("shader/simple.frag"), GL_FRAGMENT_SHADER);
	shaderProgram->combineShadersIntoPair("simple_vtx", "simple_frag");
	shaderProgram->setCurrentShaderPair("simple_vtx", "simple_frag");
	shaderProgram->linkAndUse();

	m_gridRenderer = std::make_shared<GridRenderer>();
	m_gridRenderer->init(glm::vec2(10, 10));

	m_lineRenderer = std::make_shared<LineRenderer>();
	m_lineRenderer->init();

    std::shared_ptr<ScreenObject> screenObject{ std::make_shared<ScreenObject>(ScreenObject(m_width, m_height)) };
    GLuint EBO;
	glGenBuffers(1, &EBO);
	screenObject->initialize();
	screenObject->setEBO(EBO);
    m_circleRenderer = std::make_shared<CircleRenderer>(CircleRenderer(shaderProgram, screenObject));
	m_circleRenderer->init();

	m_circleRendererInstanced = std::make_shared<CircleRendererInstanced>();
	m_circleRendererInstanced->setViewportSize(m_width, m_height);
	m_circleRendererInstanced->init();

	m_bloomRenderer = std::make_shared<BloomRenderer>();
	m_bloomRenderer->init();
	m_bloomRenderer->initFBO(m_width, m_height);

    m_userInput = std::make_shared<UserInput>(UserInput(m_window));
	m_inputManager = std::make_shared<InputManager>();
	m_inputManager->setUserInput(m_userInput);
	m_inputManager->setRightClickDragCallback(
		[=](const glm::vec2& clickPos, const glm::vec2& cursorPos, const glm::vec2& prevCursorPos)
		{
			glm::vec2 w2wc{ worldToWindowCoordinates(cursorPos) };
			glm::vec2 w2wpc{ worldToWindowCoordinates(prevCursorPos) };
			m_view->move(glm::vec2(w2wpc.x, w2wpc.y) - glm::vec2(w2wc.x, w2wc.y));
		});

	m_inputManager->setLeftClickDragCallback(
		[=](const glm::vec2& clickPos, const glm::vec2& currentPos, const glm::vec2& dpos)
		{
			glm::vec2 w2w{ windowToWorldCoordinates(clickPos) };
			glm::vec2 w2wc{ windowToWorldCoordinates(currentPos) };
			if (m_lines.size() == 0)
			{
				m_lines.push_back(LineObject(glm::vec2(w2w), glm::vec2(w2wc), glm::vec3(CIRCLE_COLOR)));
			}
			else
			{
				m_lines[0].setLine(glm::vec2(w2w), glm::vec2(w2wc));
			}
		});

	m_inputManager->setLeftClickReleaseCallback(
		[=](const glm::vec2& clickPos, const glm::vec2& cursorPos)
		{
			glm::vec2 w2w{ windowToWorldCoordinates(clickPos) };
			glm::vec2 w2wc{ windowToWorldCoordinates(cursorPos) };
			glm::vec2 velocity{ 1e-2f * (w2w - w2wc) };
			CircleObject newObj(glm::vec2(w2w), glm::vec3{CIRCLE_COLOR}, std::move(velocity));
			newObj.setRadius(std::move(GLfloat{CIRCLE_RADIUS}));
			newObj.setMass(std::move(GLfloat{CIRCLE_MASS}));
			if (STATIONARY_CHECKED)
				newObj.setStationary(STATIONARY_CHECKED);

			m_newCircles.push_back(newObj);
			if (m_lines.size() > 0)
			{
				m_lines.pop_back();
			}
		}
	);
	m_inputManager->setScrollCallback(
		[=](const glm::vec2& cursorPos, const GLfloat& zoom)
		{
			GLfloat zoomVal{ zoom < 0 ? 0.90f : 1.10f };
			m_view->zoom(zoomVal);
		});


	auto f_onWindowResized{[](GLFWwindow* window, int w, int h)
    {
        static_cast<Simulation*>(glfwGetWindowUserPointer(window))->onWindowResized(window, w, h);
    }};

	glfwSetFramebufferSizeCallback(m_window, f_onWindowResized);

	m_view = std::make_shared<View>(glm::vec2(static_cast<GLfloat>(m_width), static_cast<GLfloat>(m_height)));

	m_world = std::make_shared<World>();
	m_world->setWorldDimensions(WORLD_SIZE);

	m_view->setPosition({-125.f, -125.f});

	m_timeFlow = std::make_shared<TimeFlow>();
	m_timeFlow->setDeltaTime(0.3f);

	// Initialize ImGui.
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	m_settingsWindow = std::make_shared<SettingsWindow>(SettingsWindow());
	// FlowControl.
	std::shared_ptr<FlowControlMenu> flowControl{ std::make_shared<FlowControlMenu>() };
	flowControl->setTimestepReference(m_timeFlow->deltaTimeReference());
	flowControl->setSimulationTimeReference(m_timeFlow->timeReference());
	flowControl->setBtnStateCallback(std::function<void()>(stateBtnCallback));
	flowControl->setBtnResetCallback([=]()
	{
		m_world->circles()->clear();
		m_timeFlow->resetTime();
	});
	flowControl->setBtnStepForwardCallback([=]()
	{
		g_sim_state = STATE::STEP;
	});
	m_settingsWindow->addMenu(flowControl);

	// ShaderSettings.
	std::shared_ptr<ShaderSettingsMenu> shaderSettings{ std::make_shared<ShaderSettingsMenu>() };
	shaderSettings->setCheckboxDisplayGridCallback([=](const bool& state)
	{
		m_gridRenderer->toggleGrid(state);
	});
	shaderSettings->setSelectShaderTypeCallback([=](const std::size_t &i)
	{
		SHADER_TYPE = i;
	});
	m_settingsWindow->addMenu(shaderSettings);

	// Circle Creator.
	std::shared_ptr<CircleCreatorMenu> circleCreator{ std::make_shared<CircleCreatorMenu>() };
	circleCreator->setColorReference(&CIRCLE_COLOR);
	circleCreator->setMassReference(&CIRCLE_MASS);
	circleCreator->setRadiusReference(&CIRCLE_RADIUS);
	circleCreator->setNumCirclesReference(&N_CIRCLES);
	circleCreator->setCheckboxStationaryCallback([=](const bool& state)
	{
		STATIONARY_CHECKED = state;
	});
	m_settingsWindow->addMenu(circleCreator);

	// Gravity.
	m_gravity = std::make_shared<GravityCalculator>();
	m_gravity->setGravitationalConstant(6.674e-7f);

	// CollisionDetection.
	m_collisionDetection = std::make_shared<CollisionDetectionGrid>();
	m_collisionDetection->setGridSize(m_world->worldDimensions());
	m_collisionDetection->setGridDimensions({ 20, 20 });
	m_collisionDetection->init();

	// World border.
	glm::vec3 borderColor{ 1.0f, 0.0f, 0.0f };
	m_worldBorder = std::vector<LineObject>
	{
		LineObject(glm::vec2(0.f), glm::vec2(0.f, m_world->worldDimensions().y), glm::vec3(borderColor)),
		LineObject(glm::vec2(0.f, m_world->worldDimensions().y), glm::vec2(m_world->worldDimensions().x, m_world->worldDimensions().y), glm::vec3(borderColor)),
		LineObject(glm::vec2(m_world->worldDimensions().x, m_world->worldDimensions().y), glm::vec2(m_world->worldDimensions().x, 0.f), glm::vec3(borderColor)),
		LineObject(glm::vec2(m_world->worldDimensions().x, 0.f), glm::vec2(0.f), glm::vec3(borderColor))
	};
}

void Simulation::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	std::chrono::duration<float> frameDuration(FRAME_LENGTH);
	std::chrono::steady_clock::time_point prevTime{ std::chrono::steady_clock::now() };

	m_world->generateCircles(NUM_CIRCLES);
	std::shared_ptr<std::vector<CircleObject>> m_circles{ m_world->circles() };

    while (!glfwWindowShouldClose(m_window))
	{
		if (TO_RESIZE)
			resizeWindow();

		if (m_newCircles.size() > 0)
		{
			m_circles->insert(m_circles->end(), m_newCircles.begin(), m_newCircles.end());
			m_newCircles.clear();
		}

		N_CIRCLES = m_circles->size();

		std::vector<CircleRenderData> circleRenderData(m_circles->size());

		std::chrono::steady_clock::time_point currentTime{ std::chrono::steady_clock::now() };
		if ( std::chrono::duration<float>(currentTime - prevTime).count() < FRAME_LENGTH)
			continue;
		prevTime = currentTime;

		m_inputManager->handleInputEvents();

		// ImGui frame.

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_settingsWindow->draw();

		ImGui::Render();

		// Update other events like keyboard/mouse input.
		glfwPollEvents();
		if (true)
		{
			if (g_sim_state != STATE::STOP)
			{
				m_timeFlow->updateTime();
				// m_gravity->applyForces(m_circles);
				m_gravity->updateVelAndPos(m_circles, m_timeFlow->deltaTime());
				// m_collisionDetection->storeCirclesIntoGridCells(m_circles);
				// m_collisionDetection->detectCollisions();
				// m_collisionDetection->resolveCollisions(m_circles);

			}

			glm::vec2 halfScreen{ 0.5f * static_cast<GLfloat>(m_width), 0.5f * static_cast<GLfloat>(m_height) };
			glm::mat4 viewProjectionMatrix{ m_view->viewProjectionMatrix() };

			/*
			for (int i = 0; i < m_circles->size(); i++)
			{
				glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(m_circles->at(i).pos(), 0.0f)) };
				glm::vec4 pos{ viewProjectionMatrix * modelMatrix * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) };
				glm::vec2 ndc{ pos.x / pos.w, pos.y / pos.w };
				circleRenderData[i] = CircleRenderData{
					{ (ndc.x + 1.0f) * halfScreen.x, (ndc.y + 1.0f) * halfScreen.y },
					m_circles->at(i).color(),
					m_view->zoomVal() * m_circles->at(i).radius()
					};
			}
			*/
			// auto shaderProgram { m_circleRenderer->shaderProgram() };
			// glUseProgram(shaderProgram->id());
			// shaderProgram->updateUniform1i("shaderType", static_cast<GLint>(SHADER_TYPE));
			// shaderProgram->updateUniform1i("num_circles", static_cast<GLint>(m_circles->size()));
			// m_circleRenderer->render(circleRenderData);

			for (int i{0}; i < m_circles->size(); i++)
			{
				GLfloat rad{ m_circles->at(i).radius() };
				glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(m_circles->at(i).pos(), 0.0f)) };
				modelMatrix = glm::scale(modelMatrix, glm::vec3(rad, rad, 0.f));
				glm::vec4 pos{ modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
				circleRenderData[i] = CircleRenderData{
					m_circles->at(i).pos(),
					m_circles->at(i).color(),
					rad
					};
			}

			m_circleRendererInstanced->setOutputFBO(m_bloomRenderer->inputFBO());
			m_circleRendererInstanced->setOutputFBOTextures(m_bloomRenderer->inputFBOSceneTex(), m_bloomRenderer->inputFBOBloomTex());

			auto shaderProgram_cren{ m_circleRendererInstanced->shaderProgram() };
			glUseProgram(shaderProgram_cren->id());
			shaderProgram_cren->updateUniformMatrix4fv("viewProjectionMatrix", viewProjectionMatrix);
			m_circleRendererInstanced->render(circleRenderData);

			m_bloomRenderer->render();

			if (m_gridRenderer->isGridEnabled())
			{
				auto shaderProgram_grid{ m_gridRenderer->shaderProgram() };
				glUseProgram( shaderProgram_grid->id() );
				glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(m_world->worldCenter(), 0.0f)) };
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f * WORLD_SIZE, 0.0));
				shaderProgram_grid->updateUniformMatrix4fv("modelMatrix", modelMatrix);
				shaderProgram_grid->updateUniformMatrix4fv("viewProjectionMatrix", m_view->viewProjectionMatrix());
				m_gridRenderer->render();
			}

			if (m_lines.size() > 0)
				m_lineRenderer->render(m_lines, m_view->viewProjectionMatrix());

			m_lineRenderer->render(m_worldBorder, m_view->viewProjectionMatrix());
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window);

		if (g_sim_state == STATE::STEP)
			g_sim_state = STATE::STOP;
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

glm::vec2 Simulation::worldToWindowCoordinates(const glm::vec2& clickPos)
{
	glm::vec2 viewPos{ m_view->position() };
	return glm::vec2(clickPos.x - viewPos.x, -clickPos.y + viewPos.y);
}

glm::vec2 Simulation::windowToWorldCoordinates(const glm::vec2& clickPos)
{
	glm::vec2 dimensions{ glm::vec2((GLfloat)m_width, (GLfloat)m_height) };
	glm::vec2 scaledDimensions{ m_view->dimensions() };
	glm::vec2 screenCoords{ clickPos.x, dimensions.y - clickPos.y };
	glm::vec2 scaledScreenCoords{ screenCoords.x * scaledDimensions.x / dimensions.x,  screenCoords.y * scaledDimensions.y / dimensions.y};
	glm::vec2 offsetCoordinates{ m_view->position() };
	glm::vec2 wCoords{ scaledScreenCoords + offsetCoordinates };
	return wCoords;
}