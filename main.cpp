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
	virtual void render() override;
	virtual void renderGUI() override;
protected:
	f32 mouseSens = 10.0f;

	Shader* shader1;
	Texture* texture1;
	Texture* texture2;
	Texture* towerTexture;
	Texture* towerNormalTexture;
	Texture* parDiffuse;
	Texture* parNormal;
	Texture* parDisplacment;
	Camera* mainCamera;
	Node* mainCameraController;
	Node* spotLight = nullptr;
	Scene* scene;
	Mesh* robot = nullptr;
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
	scene->setAmbientColor(mathfu::vec4(0.15, 0.2, 0.3, 1.0));
	mainCameraController = (new Node())->setName("cameraController");
	mainCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Camera::ProjectionType::Perspective, 4.0/3.0);
	mainCamera->setPosition(mathfu::vec3(0, 1, -2));
	mainCamera->setParent(mainCameraController);
	mainCamera->setName("mainCamera");
	//mainCamera->addPostProccessShader("assets/shaders/sky.frag");
	//mainCamera->addPostProccessShader("assets/shaders/blurv.frag");
	//mainCamera->addPostProccessShader("assets/shaders/blurh.frag");
	//mainCamera->addPostProccessShader("assets/shaders/red_fragment.frag");

	shader1 = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
	texture1 = new Texture("./assets/textures/wall.jpg");
	texture2 = new Texture("./assets/textures/grass.jpg");
	towerTexture = new Texture("./assets/models/tower/textures/Wood_Tower_Col.jpg");
	towerNormalTexture = new Texture("./assets/models/tower/textures/Wood_Tower_Nor.jpg");
	parDiffuse = new Texture("./assets/textures/brick/bricks.jpg");
	parNormal = new Texture("./assets/textures/brick/bricks_norm.jpg");
	parDisplacment = new Texture("./assets/textures/brick/bricks_disp.jpg");


	scene->addNode(Mesh::createPlane(300)->setDiffuse(texture2)->setName("plane_mesh")->setScale(mathfu::vec3(1000, 1000, 1000)));
	scene->addNode(Mesh::createCube()->setDiffuse(parDiffuse)->setDisplacment(parDisplacment)->setNormal(parNormal)->setName("mesh2")->setPosition(mathfu::vec3(0, 1, 1)));

	scene->addNode(Mesh::createFromFile("./assets/models/tower/wooden_watch_tower2.fbx")
		->setDiffuse(towerTexture)->setNormal(towerNormalTexture)->setScale(mathfu::vec3(3, 3, 3))->setPosition(mathfu::vec3(0, 0, 30))
		->setName("Tower")->rotate(mathfu::vec3(1, 0, 0), -90));

	robot = (Mesh*)(scene->addNode(Mesh::createFromFile("./assets/models/robot/robot.fbx")->setScale(mathfu::vec3(0.02, 0.02, 0.02))->setPosition(mathfu::vec3(2, 0, 0))->setName("robot")));

	scene->find("mesh2")->addChild(Mesh::createCube()->setDiffuse(texture1)->setName("mesh3")->setPosition(mathfu::vec3(5, 0, 5)));
	scene->find("mesh3")->addChild(Mesh::createCube()->setDiffuse(texture1)->setName("mesh4")->setPosition(mathfu::vec3(-5, 0, 0)));
	scene->addNode(Mesh::createCone()->setDiffuse(texture1)->setName("mesh5")->setPosition(mathfu::vec3(-20, 1,  1)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setName("mesh6")->setPosition(mathfu::vec3( 20, 1,  20))->rotate(mathfu::vec3(0, 1, 0), 45));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setName("mesh7")->setPosition(mathfu::vec3(-20, 1,  20)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setName("mesh8")->setPosition(mathfu::vec3( 20, 1, -20)));
	scene->addNode(Mesh::createCube()->setDiffuse(texture1)->setName("mesh9")->setPosition(mathfu::vec3(-20, 1, -20)));
	scene->addNode(mainCameraController);
	scene->setMainCamera(reinterpret_cast<Camera*>(mainCameraController->findChild("mainCamera")));
	scene->addNode((new Light(Light::Type::Directional))->setColor(mathfu::vec3(255.0 / 255.0, 221.0 / 255.0, 153.0 / 255.0))->setPower(0.4)->rotate(mathfu::vec3(1, 0, 0), -120.0))->rotate(mathfu::vec3(0, 1, 0), 30)->setName("directional_light");
	spotLight = scene->addNode((new Light(Light::Type::Spot))->setColor(mathfu::vec3(1.0, 0.5, 0.4))->setCone(60)->setPower(3)->setRadius(120)->rotate(mathfu::vec3(1,0,0), -90)->setPosition(mathfu::vec3(0, 23.0, 25.8))->setName("theSpotLight"));
	for (int i = 0; i < 50; i++) {
		Node* theNewLight = (new Light(i%2 == 0 ? Light::Type::Spot : Light::Type::Point))
			->setColor(mathfu::vec3(randomNumber(0, 250) / 250.0, randomNumber(0, 250) / 250.0, randomNumber(0, 250) / 250.0))
			->setRadius(randomNumber(5, 40))
			->setCone(randomNumber(45, 170))
			->setPosition(mathfu::vec3(randomNumber(-100, 100), randomNumber(1, 3), randomNumber(-100, 100)))
			->rotate(mathfu::vec3(0, 1, 0), randomNumber(0, 360));
		scene->addNode(theNewLight);
	}
}

void MyApp::destroy()
{
	delete shader1;
	delete texture1;
	delete texture2;
	delete towerTexture;
	delete towerNormalTexture;
	delete parDiffuse;
	delete parDisplacment;
	delete parNormal;
	delete mainCamera;
	delete scene;
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			scene->find("theSpotLight")->move(mathfu::vec3(0, 0, 1) * speed  * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			scene->find("theSpotLight")->move(mathfu::vec3(0, 0, -1) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			scene->find("theSpotLight")->move(mathfu::vec3(-1, 0, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			scene->find("theSpotLight")->move(mathfu::vec3(1, 0, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			scene->find("theSpotLight")->move(mathfu::vec3(0, 1, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			scene->find("theSpotLight")->move(mathfu::vec3(0, -1, 0) * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			scene->find("theSpotLight")->rotate(mathfu::vec3(0, 0, 1), speed * this->deltaTime * 30);
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			scene->find("theSpotLight")->rotate(mathfu::vec3(1, 0, 0), speed * this->deltaTime * 30);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			scene->find("theSpotLight")->rotate(mathfu::vec3(0, 1, 0), speed * this->deltaTime * 30);
		}
	} else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			scene->find("robot")->move(scene->find("robot")->getForward() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			scene->find("robot")->move(scene->find("robot")->getBackward() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			scene->find("robot")->move(scene->find("robot")->getLeft() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			scene->find("robot")->move(scene->find("robot")->getRight() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			scene->find("robot")->move(scene->find("robot")->getUp() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			scene->find("robot")->move(scene->find("robot")->getDown() * speed * this->deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			scene->find("robot")->rotate(mathfu::vec3(0, 0, 1), this->deltaTime * speed * 3);
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			scene->find("robot")->rotate(mathfu::vec3(1, 0, 0), this->deltaTime * speed * 3);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			scene->find("robot")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime * speed * 3);
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

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			mainCamera->renderType = "final";
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			mainCamera->renderType = "depth";
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			mainCamera->renderType = "albedo";
		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
			mainCamera->renderType = "normal";
		}
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
			mainCamera->renderType = "tangent";
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			scene->find("directional_light")->rotate(mathfu::vec3(0, 0, 1), speed * this->deltaTime * 30);
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			scene->find("directional_light")->rotate(mathfu::vec3(1, 0, 0), speed * this->deltaTime * 30);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			scene->find("directional_light")->rotate(mathfu::vec3(0, 1, 0), speed * this->deltaTime * 30);
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		mainCameraController->rotate(mainCameraController->getLeft().Normalized(), -mouseDeltaY * mouseSens * this->deltaTime);
		mainCamera->rotate(mathfu::vec3(0, 1, 0), mouseDeltaX * mouseSens * this->deltaTime);
	}
	//scene->find("mesh2")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
	//scene->find("mesh3")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
	//scene->find("mesh4")->rotate(mathfu::vec3(0, 1, 0), this->deltaTime);
}

void MyApp::render()
{
	scene->preRender();
	scene->render();
	scene->postRender();
	//dynamic_cast<Light*>(scene->find("directional_light"))->mShadowCamera->renderType = "depth";
	//dynamic_cast<Light*>(scene->find("directional_light"))->mShadowCamera->draw();
	mainCamera->draw();
}

void MyApp::renderGUI()
{
	{
		static float f = 0.0f;
		static float f2 = 0.0f;
		static float f3 = 0.0f;
		static float f4 = 0.0f;
		static float f5 = 0.0f;
		static float f6 = 0.0f;
		static float f7 = 0.0f;
		static int counter = 0;
		static bool hovered;

		f2 = mainCamera->getPosition().x;
		f3 = mainCamera->getPosition().y;
		f4 = mainCamera->getPosition().z;

		f5 = mainCamera->getRotation().ToEulerAngles().x;
		f6 = mainCamera->getRotation().ToEulerAngles().y;
		f7 = mainCamera->getRotation().ToEulerAngles().z;

		ImGui::Begin("Hello, world!"); 
		hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AnyWindow);

		ImGui::Text("This is some useful text."); 

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::Columns(3);
		ImGui::DragFloat("X", &f2, 0.1f);
		ImGui::NextColumn();
		ImGui::DragFloat("Y", &f3, 0.1f);
		ImGui::NextColumn();
		ImGui::DragFloat("Z", &f4, 0.1f);
		ImGui::NextColumn();
		ImGui::DragFloat("X", &f5, 0.1f);
		ImGui::NextColumn();
		ImGui::DragFloat("Y", &f6, 0.1f);
		ImGui::NextColumn();
		ImGui::DragFloat("Z", &f7, 0.1f);
		ImGui::Columns(1);

		if (ImGui::Button("Button"))                          
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text(hovered ? "Is Hover" : "Is not hover");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

std::ostream& operator << (std::ostream& ostream, mathfu::vec3 vec)
{
	ostream << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return ostream;
}
