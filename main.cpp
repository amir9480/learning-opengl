#include <iostream>
#include <mathfu/vector.h>
#include "helpers.h"
#include "Shader.h";
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Scene.h";
#include "App.h"
using namespace std;


std::ostream& operator << (std::ostream& ostream, mathfu::vec3 vec);

class MyApp : public App
{
public:
	MyApp();

	virtual void init() override;
	virtual void destroy() override;
	virtual void update() override;
	virtual void render() override;
protected:
	f32 mouseSens = 0.1f;

	Shader* shader1;
	Texture* texture1;
	Camera* mainCamera;
	Scene* scene;
	Shader* lightShader;

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
	scene = new Scene("level1");
	mainCamera = new Camera(1368, 768, Camera::ProjectionType::Perspective, 4.0/3.0);
	mainCamera->setPosition(mathfu::vec3(0, 1, -2));
	//mainCamera->addPostProccessShader("assets/shaders/bw_fragment.frag");
	//mainCamera->addPostProccessShader("assets/shaders/red_fragment.frag");

	shader1 = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	texture1 = new Texture("./assets/textures/wall.jpg");

	lightShader = new Shader("assets/shaders/simple_vertex.vert", "assets/shaders/light.frag");

	scene->addNode(Mesh::createPlane(20)->setMaterial(shader1)->setScale(mathfu::vec3(100, 100, 100)));
	scene->addNode(Mesh::createCube()->setMaterial(shader1)->setPosition(mathfu::vec3(0, 1, 1)));
	scene->addNode(mainCamera);
	scene->setMainCamera(mainCamera);
}

void MyApp::destroy()
{
	delete shader1;
	delete texture1;
	delete mainCamera;
	delete scene;
	delete lightShader;
}

void MyApp::update()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
	f32 speed = 10.0f;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			//mesh1->move(mesh1->getForward() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			//mesh1->move(mesh1->getBackward() * speed * this->deltaTime);
		}
	} else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getForward() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getBackward() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getRight() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getLeft() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getUp() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			mainCamera->move(mainCamera->getDown() * speed * this->deltaTime);
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		mainCamera->rotate(mainCamera->getUp(), -mouseDeltaX * mouseSens * this->deltaTime);
		mainCamera->rotate(mainCamera->getLeft(), mouseDeltaY * mouseSens * this->deltaTime);
	}
}

void MyApp::render()
{
	shader1->use();
	shader1->setTexture("diffuse", texture1, 0);
	scene->render();
	mainCamera->postProccess(lightShader);
    mainCamera->draw();
}

std::ostream& operator << (std::ostream& ostream, mathfu::vec3 vec)
{
	ostream << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return ostream;
}
