#include <iostream>
#include <mathfu/vector.h>
#include "helpers.h"
#include "Shader.h";
#include "Mesh.h"
#include "Light.h"
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
	void postProccessLight(Camera* camera, Node* light);
	virtual void render() override;
protected:
	f32 mouseSens = 0.1f;

	Shader* shader1;
	Texture* texture1;
	Texture* texture2;
	Camera* mainCamera;
	Node* mainCameraController;
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
	App(std::string("Hello World!"))
{
	init();
}

void MyApp::init()
{
	scene = new Scene("level1");
	mainCameraController = (new Node())->setName("cameraController");
	mainCamera = new Camera(1368, 768, Camera::ProjectionType::Perspective, 4.0/3.0);
	mainCamera->setPosition(mathfu::vec3(0, 1, -2));
	mainCamera->setParent(mainCameraController);
	mainCamera->setName("mainCamera");
	//mainCamera->addPostProccessShader("assets/shaders/bw_fragment.frag");
	//mainCamera->addPostProccessShader("assets/shaders/red_fragment.frag");

	shader1 = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	texture1 = new Texture("./assets/textures/wall.jpg");
	texture2 = new Texture("./assets/textures/grass.jpg");

	lightShader = new Shader("assets/shaders/simple_vertex.vert", "assets/shaders/light.frag");

	scene->addNode(Mesh::createPlane(30)->setDiffuse(texture2)->setMaterial(shader1)->setName("plane_mesh")->setScale(mathfu::vec3(100, 100, 100)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh2")->setPosition(mathfu::vec3(0, 1, 1)));
	scene->find("mesh2")->addChild(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh3")->setPosition(mathfu::vec3(5, 0, 5)));
	scene->find("mesh3")->addChild(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh4")->setPosition(mathfu::vec3(-5, 0, 0)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh5")->setPosition(mathfu::vec3(-20, 1, 1)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh6")->setPosition(mathfu::vec3(20, 1, 20))->rotate(mathfu::vec3(0, 1, 0), 45));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh7")->setPosition(mathfu::vec3(-20, 1, 20)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh8")->setPosition(mathfu::vec3(20, 1, -20)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setMaterial(shader1)->setName("mesh9")->setPosition(mathfu::vec3(-20, 1, -20)));
	scene->addNode(mainCameraController);
	scene->setMainCamera(reinterpret_cast<Camera*>(mainCameraController->findChild("mainCamera")));
	scene->addNode((new Light(Light::Type::Directional))->setColor(mathfu::vec3(0.98, 0.8, 0.7))->rotate(mathfu::vec3(1, 0, 0), -45.0 * mathfu::kDegreesToRadians));
	scene->addNode((new Light(Light::Type::Point))->setColor(mathfu::vec3(1.0, 0.0, 0.0))->setPosition(mathfu::vec3(10, 3, 10)));
	scene->addNode((new Light(Light::Type::Point))->setColor(mathfu::vec3(0.3, 0.5, 1.0))->setPosition(mathfu::vec3(-10, 3, 10)));
}

void MyApp::destroy()
{
	delete shader1;
	delete texture1;
	delete texture2;
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
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 20.0f;
	} else {
		speed = 10.0f;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			scene->find("mesh2")->move(mathfu::vec3(0, 0, 1) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			scene->find("mesh2")->move(mathfu::vec3(0, 0, -1) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			scene->find("mesh2")->move(mathfu::vec3(-1, 0, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			scene->find("mesh2")->move(mathfu::vec3(1, 0, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			scene->find("mesh2")->move(mathfu::vec3(0, 1, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			scene->find("mesh2")->move(mathfu::vec3(0, -1, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			scene->find("mesh2")->rotate(mathfu::vec3(0, 0, 1), this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			scene->find("mesh2")->rotate(mathfu::vec3(1, 0, 0), this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			scene->find("mesh2")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
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
		mainCameraController->rotate(mathfu::vec3(mainCameraController->getLeft().x, 0, mainCameraController->getLeft().z).Normalized(), -mouseDeltaY * mouseSens * this->deltaTime);
		mainCamera->rotate(mathfu::vec3(0, 1, 0), mouseDeltaX * mouseSens * this->deltaTime);
	}
	scene->find("mesh2")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
	scene->find("mesh3")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
	scene->find("mesh4")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
}

void MyApp::postProccessLight(Camera* mainCamera, Node* node)
{
	if (node->getClass() == "Light") {
		Light* light = reinterpret_cast<Light*>(node);

		lightShader->setInt("lightType", (int)light->getType());
		lightShader->setFloat3("lightColor", light->getColor());
		lightShader->setFloat3("lightDirection", light->getForward());
		lightShader->setFloat("lightPower", light->getPower());
		lightShader->setFloat("lightRadius", light->getRadius());
		lightShader->setFloat3("lightPosition", light->getPosition());
		mainCamera->postProccess(lightShader);
	}

	for (auto& node : node->getChildren()) {
		postProccessLight(mainCamera, node);
	}
}

void MyApp::render()
{
	scene->render();
	for (auto& node : scene->getNodes()) {
		 postProccessLight(mainCamera, node);
	}
    mainCamera->draw();
}

std::ostream& operator << (std::ostream& ostream, mathfu::vec3 vec)
{
	ostream << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return ostream;
}
