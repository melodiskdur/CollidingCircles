#include "Simulation.h"
#include "FlowControlMenu.h"
#include "ShaderSettingsMenu.h"
#include "CircleCreatorMenu.h"
#include <chrono>

constexpr const GLuint INIT_WIDTH{ 1280 };
constexpr const GLuint INIT_HEIGHT{ 720 };
constexpr glm::vec2 WORLD_SIZE{ 10000.0f, 10000.0f };
constexpr GLuint NUM_CIRCLES{ 100 };
constexpr const float FRAME_LENGTH{ 1.0f / 60.0f };
enum class STATE { RUN, STOP, STEP };

static STATE g_sim_state{ STATE::STOP };
static std::size_t SHADER_TYPE{ 0 };

static glm::vec3 CIRCLE_COLOR{ 0.99f, 0.2f, 0.0f };
static GLfloat CIRCLE_RADIUS{ 1.0f };
static GLfloat CIRCLE_MASS{ 1.0f };

static bool STATIONARY_CHECKED{ false };

void stateBtnCallback()
{
	g_sim_state = g_sim_state == STATE::RUN ? STATE::STOP : STATE::RUN;
	std::cout << "State changed\n";
}

void Simulation::onWindowResized(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
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

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "Hello Circle", NULL, NULL);
	if (!m_window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;
    glewInit();

    std::shared_ptr<ShaderProgram> shaderProgram{ std::make_shared<ShaderProgram>(ShaderProgram()) };
	shaderProgram->addShader(std::string{"simple_vtx"}, std::string("shader/simple.vert"), GL_VERTEX_SHADER);
	shaderProgram->addShader(std::string{"simple_frag"}, std::string("shader/simple.frag"), GL_FRAGMENT_SHADER);
	shaderProgram->combineShadersIntoPair("simple_vtx", "simple_frag");
	shaderProgram->setCurrentShaderPair("simple_vtx", "simple_frag");
	shaderProgram->linkAndUse();

	m_gridRenderer = std::make_shared<GridRenderer>();
	m_gridRenderer->init();

	m_lineRenderer = std::make_shared<LineRenderer>();
	m_lineRenderer->init();

    std::shared_ptr<ScreenObject> screenObject{ std::make_shared<ScreenObject>(ScreenObject(m_width, m_height)) };
    GLuint EBO;
	glGenBuffers(1, &EBO);
	screenObject->initialize();
	screenObject->setEBO(EBO);
    m_circleRenderer = std::make_shared<CircleRenderer>(CircleRenderer(shaderProgram, screenObject));
	m_circleRenderer->init();

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
				std::cout << "Line created \n";
				m_lines.push_back(LineObject(glm::vec2(w2w), glm::vec2(w2wc), glm::vec3(CIRCLE_COLOR)));
			}
			else
			{
				//std::cout << "------------------------\n";
				//std::cout << w2w.x << " " << w2w.y << "\n";
				//std::cout << w2wc.x << " " << w2wc.y << "\n";
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
				std::cout << "Line destroyed\n";
			}
		}
	);
	m_inputManager->setScrollCallback(
		[=](const glm::vec2& cursorPos, const GLfloat& zoom)
		{
			m_view->zoom(zoom);
		});


	auto f_onWindowResized{[](GLFWwindow* window, int w, int h)
    {
        static_cast<Simulation*>(glfwGetWindowUserPointer(window))->onWindowResized(window, w, h);
    }};

	glfwSetFramebufferSizeCallback(m_window, f_onWindowResized);

	m_view = std::make_shared<View>(glm::vec2(static_cast<GLfloat>(m_width), static_cast<GLfloat>(m_height)));

	m_world = std::make_shared<World>();
	m_world->setWorldDimensions(WORLD_SIZE);

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
	circleCreator->setCheckboxStationaryCallback([=](const bool& state)
	{
		STATIONARY_CHECKED = state;
	});
	m_settingsWindow->addMenu(circleCreator);

	// Gravity.
	m_gravity = std::make_shared<GravityCalculator>();
	m_gravity->setGravitationalConstant(6.674e2f);

	// CollisionDetection.
	m_collisionDetection = std::make_shared<CollisionDetectionGrid>();
	m_collisionDetection->setGridSize(m_world->worldDimensions());
	m_collisionDetection->setGridDimensions({ 20, 20 });
	m_collisionDetection->init();
}

void Simulation::run()
{
	std::chrono::duration<float> frameDuration(FRAME_LENGTH);
	std::chrono::steady_clock::time_point prevTime{ std::chrono::steady_clock::now() };

	//m_world->generateCircles(NUM_CIRCLES);
	std::shared_ptr<std::vector<CircleObject>> m_circles{ m_world->circles() };

    while (!glfwWindowShouldClose(m_window))
	{
		if (m_newCircles.size() > 0)
		{
			m_circles->insert(m_circles->end(), m_newCircles.begin(), m_newCircles.end());
			m_newCircles.clear();
		}
		std::vector<GLfloat> radii(m_circles->size());
		std::vector<glm::vec2> positions(m_circles->size());
		std::vector<glm::vec3> colors(m_circles->size());

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
				m_gravity->applyForces(m_circles);
				m_gravity->updateVelAndPos(m_circles, m_timeFlow->deltaTime());
				m_collisionDetection->storeCirclesIntoGridCells(m_circles);
				m_collisionDetection->detectCollisions();
				m_collisionDetection->resolveCollisions(m_circles);
			}

			for (int i = 0; i < m_circles->size(); i++)
			{
				glm::vec2 halfScreen{ 0.5f * static_cast<GLfloat>(m_width), 0.5f * static_cast<GLfloat>(m_height) };
				glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(m_circles->at(i).pos(), 0.0f)) };
				glm::mat4 projectionMatrix{ m_view->projectionMatrix(glm::vec2((GLfloat)m_width, (GLfloat)m_height)) };
				glm::vec4 pos{ projectionMatrix * m_view->viewMatrix() * modelMatrix * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) };
				glm::vec2 ndc{ pos.x / pos.w, pos.y / pos.w };
				positions[i] = { (ndc.x + 1.0f) * halfScreen.x, (ndc.y + 1.0f) * halfScreen.y };
				radii[i] = m_view->zoomVal() * m_circles->at(i).radius();
				colors[i] = m_circles->at(i).color();
			}


			auto shaderProgram { m_circleRenderer->shaderProgram() };
			glUseProgram(shaderProgram->id());
			shaderProgram->updateUniform1fv("radii", radii);
			shaderProgram->updateUniform2fv("positions", positions);
			shaderProgram->updateUniform3fv("colors", colors);
			shaderProgram->updateUniform1i("shaderType", static_cast<GLint>(SHADER_TYPE));
			shaderProgram->updateUniform1i("num_circles", static_cast<GLint>(m_circles->size()));
			m_circleRenderer->render(*m_circles);

			if (m_gridRenderer->isGridEnabled())
			{
				auto shaderProgram_grid{ m_gridRenderer->shaderProgram() };
				glUseProgram( shaderProgram_grid->id() );
				shaderProgram_grid->updateUniformMatrix4fv("viewMatrix", m_view->viewMatrix());
				glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(m_world->worldCenter(), 0.0f)) };
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f * WORLD_SIZE, 0.0));
				shaderProgram_grid->updateUniformMatrix4fv("modelMatrix", modelMatrix);
				shaderProgram_grid->updateUniformMatrix4fv("projectionMatrix", m_view->projectionMatrix(glm::vec2((GLfloat)m_width, (GLfloat) m_height)));
				m_gridRenderer->render();
			}

			if (m_lines.size() > 0)
				m_lineRenderer->render(m_lines, m_view->viewMatrix(), m_view->projectionMatrix(glm::vec2((GLfloat)m_width, (GLfloat)m_height)));
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
	glm::vec2 dimensions{ 0.5f * glm::vec2((GLfloat)m_width, (GLfloat)m_height) };
	glm::vec2 cCoords{ clickPos.x - dimensions.x, dimensions.y - clickPos.y };
	glm::vec2 wCoords{ 1.0f / m_view->zoomVal() * 2.0f * (cCoords + m_view->position()) };
	return wCoords;
}