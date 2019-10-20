#include "App.h"

App::App(std::string name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(1360, 768, name.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
}

App::~App()
{
	this->destroy();
	glfwTerminate();
}

void App::run()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	static f64 mX, mY, theClock;
	while (!glfwWindowShouldClose(window))
	{
		theClock = clock();
		deltaTime = (theClock - lastTime) / 1000.0f;
		lastTime = theClock;
		glfwGetCursorPos(window, &mX, &mY);
		processMouse(mX, mY);
		this->update();
		glClear(GL_COLOR_BUFFER_BIT);
		this->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::init()
{
}

void App::update()
{
}

void App::render()
{
}

void App::destroy()
{
}

void App::processMouse(double xpos, double ypos)
{
	static bool first = true;
	if (first) {
		first = false;
		mouseX = xpos;
		mouseY = ypos;
	}
	mouseLastX = mouseX;
	mouseLastY = mouseY;
	mouseX = xpos;
	mouseY = ypos;
	mouseDeltaX = mouseLastX - mouseX;
	mouseDeltaY = mouseLastY - mouseY;
}
