#include <iostream>
#include <mathfu/vector.h>
#include "helpers.h"
#include "Shader.h";
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "App.h"
using namespace std;


class MyApp : public App
{
public:
	MyApp();

	virtual void init() override;
	virtual void destroy() override;
	virtual void update() override;
	virtual void render() override;
	virtual void renderScene(Camera* camera) override;
protected:
	f32 mouseSens = 0.001f;

	Mesh* mesh1;
	Shader* shader1;
	Texture* texture1;
	Camera* mainCamera;
};


int main()
{
	MyApp* myApp = new MyApp();
	myApp->run();
	return 0;
}


MyApp::MyApp():
	App(std::string("Hello!"))
{
	init();
}

void MyApp::init()
{
	mainCamera = new Camera(1368, 768, Camera::ProjectionType::Perspective, 4.0/3.0);
	mainCamera->setPosition(mathfu::vec3(0, 1, -2));
	//mainCamera->addPostProccessShader("assets/shaders/bw_fragment.frag");
	//mainCamera->addPostProccessShader("assets/shaders/red_fragment.frag");
	mesh1 = Mesh::createPlane(20);
	mesh1->setScale(mathfu::vec3(100, 100, 100));

	shader1 = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	texture1 = new Texture("./assets/textures/wall.jpg");
}

void MyApp::destroy()
{
	delete mesh1;
	delete shader1;
	delete texture1;
	delete mainCamera;
}

void MyApp::update()
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

void MyApp::render()
{
    mainCamera->render(&MyApp::renderScene, this); 
	mainCamera->draw();
}

void MyApp::renderScene(Camera* camera)
{
	shader1->use();
	shader1->setTexture("diffuse", texture1, 0);
	shader1->setMatrix("MVP", mesh1->getTransformMatrix() * camera->getViewProjection());
	mesh1->draw();
}
