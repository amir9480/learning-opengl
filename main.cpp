#include <iostream>
#include <mathfu/vector.h>
#include "helpers.h"
#include "Shader.h";
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
using namespace std;

void init();
void destory();
void update();
void render();
void renderScene(Camera* camera);
void processMouse(f64, f64);

f32 mouseX = 0.0f;
f32 mouseY = 0.0f;
f32 mouseLastX = 0.0f;
f32 mouseLastY = 0.0f;
f32 mouseDeltaX = 0.0f;
f32 mouseDeltaY = 0.0f;
f32 mouseSens = 0.001f;


GLFWwindow* window;
Mesh* mesh1;
Shader* shader1;
Texture* texture1;
Camera* mainCamera;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(1024, 768, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow * window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	init();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	static f64 mX, mY;
	while (!glfwWindowShouldClose(window))
	{
		glfwGetCursorPos(window, &mX, &mY);
		processMouse(mX, mY);
		update();
		glClear(GL_COLOR_BUFFER_BIT);
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	destory();
	glfwTerminate();
	return 0;
}

void processMouse(double xpos, double ypos)
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

void init()
{
	mainCamera = new Camera(1024, 768, Camera::ProjectionType::Perspective);
	mainCamera->setPosition(mathfu::vec3(0, 0, -1));
	mainCamera->addPostProccessShader("assets/shaders/bw_fragment.frag");
	mainCamera->addPostProccessShader("assets/shaders/red_fragment.frag");
	mesh1 = new Mesh({
		Vertex( 0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
		Vertex( 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
		Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f)
	}, {
		0, 1, 3,
		1, 2, 3,
		0, 3, 1,
		2, 1, 3
	});

	shader1 = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	texture1 = new Texture("./assets/textures/wall.jpg");
}

void destory()
{
	delete mesh1;
	delete shader1;
	delete texture1;
	delete mainCamera;
}

void update()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
	f32 speed = 0.1f;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			mesh1->move(mesh1->getForward() * speed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			mesh1->move(mesh1->getBackward() * speed);
		}
	} else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getForward() * speed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getBackward() * speed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getRight() * speed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getLeft() * speed);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getUp() * speed);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getDown() * speed);
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		mainCamera->rotate(mathfu::vec3(0, 1, 0), -mouseDeltaX * mouseSens);
		//mainCamera->rotate(mathfu::vec3(1, 0, 0), mouseDeltaY * mouseSens);
	}
}

void render()
{
	mainCamera->render(renderScene);
	mainCamera->draw();
}

void renderScene(Camera* camera)
{
	shader1->use();
	shader1->setTexture("diffuse", texture1, 0);
	shader1->setMatrix("MVP", mesh1->getTransformMatrix() * camera->getViewProjection());
	mesh1->draw();
	mathfu::vec3 test = mainCamera->getUp();
	std::cout << mesh1->toString() << std::endl;
}
