#include "Window.h"
#include "Rendering/glRenderer.h"


glWindow::errorMessage glWindow::init()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		return glWindow::errorMessage::glfwInitFailure;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	std::unique_ptr<GLFWwindow, decltype(glfwDeleter)> window(glfwCreateWindow(m_width, m_height, "vxEngine", nullptr, nullptr), glfwDeleter);
	m_glWindow.swap(window);

	if (m_glWindow.get() == nullptr) {
		glfwTerminate();
		return glWindow::errorMessage::glfwWindowFailure;
	}
	glfwMakeContextCurrent(m_glWindow.get());

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		fprintf(stderr, "%s here\n", glewGetErrorString(glewInit()));
		return glWindow::errorMessage::glewInitFailure;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_glWindow.get(), GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(m_glWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(m_glWindow.get(), m_width / 2, m_height / 2);

	// set background
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	
	this->m_renderContext.init();

	return glWindow::errorMessage::NoError;
}

glWindow::glWindow()
{
	m_height = 1064;
	m_width = 860;
	m_backgroundColor = Vector4(0.0f, 0.0f, 0.4f, 0.4f);
	m_windowType = ENGINE_TYPE::GL_ENGINE;
	m_windowID = -1;
	
	switch (init())
	{
		case glWindow::errorMessage::glfwWindowFailure: fprintf(stderr, "glfw window not initialized");
													    break;
		case glWindow::errorMessage::glewInitFailure:	fprintf(stderr, "glew not initialized");
													    break;
		case glWindow::errorMessage::glfwInitFailure:	fprintf(stderr, "glfw not initialized");
														break;
		case glWindow::errorMessage::NoError:			break;
		default:										break;
	}
}

glWindow::glWindow(const unsigned int& width, const unsigned int& height, const Vector4& backgroundColor)
{
	m_height = height;
	m_width = width;
	m_backgroundColor = backgroundColor;
	m_windowType = ENGINE_TYPE::GL_ENGINE;

	switch (init())
	{
		case glWindow::errorMessage::glfwWindowFailure: fprintf(stderr, "glfw window not initialized");
														break;
		case glWindow::errorMessage::glewInitFailure:	fprintf(stderr, "glew not initialized");
														break;
		case glWindow::errorMessage::glfwInitFailure:	fprintf(stderr, "glfw not initialized");
														break;
		case glWindow::errorMessage::NoError:			break;
		default:										break;
	}
}

void glWindow::update()
{
	m_renderContext.render(this->m_glWindow.get());
}