#include<iostream>
#include "Common.h"
#include <Graphics.h>
#include <CameraOperation.h>
#include <gamemain.h>
#include <AssetUtils.h>

using namespace std;

RenderPipeline* pipeline;
GameMain gameMain;

void createScene()
{
	glm::ivec2 screenSize = Window::getInstance()->getSize();
	pipeline = new RenderPipeline();
	pipeline->init();
	Graphics::reset(pipeline);
	auto scene = shared_ptr<Scene>(new Scene());
	scene->setActived(true);
	scene->setMainScene(true);
	SceneManager::getInstance()->addScene(scene);
	// 材质
	auto opaqueShader = shared_ptr<Shader>(new Shader("StandardPhongBlin"));
	auto transparentShader = shared_ptr<Shader>(new Shader("StandardPhongBlin_Transparent"));
	//auto shader = shared_ptr<Shader>(new Shader("depthview"));

	auto mat1 = shared_ptr<Material>(new Material(opaqueShader));
	auto mat2 = shared_ptr<Material>(new Material(opaqueShader));
	auto mat3 = shared_ptr<Material>(new Material(transparentShader));
	// 需要引入一个从文件加载纹理的库

	auto model = AssetUtils::getInstance()->loadModel("Resources/monkeyHead.obj");
	auto wallTex = AssetUtils::getInstance()->loadTexture2D("Resources/wall.jpg");
	wallTex->m_name = "wall";
	wallTex->setInternalFormat(TextureInternalFormat::RGB8);
	wallTex->setLevels(4);
	wallTex->setPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
	wallTex->setWrapModeS(TextureWrapMode::Repeat);
	wallTex->setWrapModeT(TextureWrapMode::Repeat);
	wallTex->setTextureFilter(TextureFilterMode::Linear_Mipmap_Linear);

	mat1->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(Texture2D::getWhiteTex2D())));
	mat2->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(Texture2D::getGrayTex2D())));
	mat3->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(wallTex)));
	mat3->setProperty(ShaderPropertyNames::CommonAlpha, std::shared_ptr<MaterialFloatProperty>(new MaterialFloatProperty(0.5f)));
	// 网格
	auto cubeMesh = PrimitiveUtils::createCube();
	auto planeMesh = PrimitiveUtils::createQuad();

	std::vector<std::shared_ptr<Material>> mats(model->getMeshCount());
	for (int i = 0;i < model->getMeshCount();++i)
	{
		mats[i] = mat1;
	}
	auto go1 = SceneUtility::createMeshGameObject(model->getSubMeshes(), mats);
	auto cube1Transform = go1->getComponent<Transform>();
	cube1Transform->setPosition(glm::vec3(0.0f));
	cube1Transform->setScale(glm::vec3(1.0f));
	cube1Transform->setEularAngle(glm::vec3(0.0f));

	auto go2 = SceneUtility::createMeshGameObject(cubeMesh, mat3);
	go2->getComponent<Renderer>()->setRenderType(RenderType::Transparent);
	auto cube2Transform = go2->getComponent<Transform>();
	cube2Transform->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	cube2Transform->setScale(glm::vec3(1.0f));
	cube2Transform->setEularAngle(glm::vec3(45.0f, 45.0f, 0.0f));


	auto go3 = SceneUtility::createMeshGameObject(planeMesh, mat2);
	auto planeTransform = go3->getComponent<Transform>();
	planeTransform->setPosition(glm::vec3(1.5f, -3.0f, 0.0f));
	planeTransform->setScale(glm::vec3(10.0f));
	planeTransform->setEularAngle(glm::vec3(0.0f));

	auto cameraGo = SceneUtility::createEmptyGameObject();
	auto cameraTransform = cameraGo->getTransform();
	cameraTransform->setPosition(glm::vec3(10.0f, 2.0f, 10.0f));
	cameraTransform->setScale(glm::vec3(1.0f));
	//cameraTransform->setEularAngle(glm::vec3(0.0f));

	auto camera = cameraGo->addComponent<Camera>();
	camera->setViewPort(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	camera->setOrtho(false);
	camera->setAspect((GLfloat)screenSize.x / (GLfloat)screenSize.y);
	camera->setFOV(45.0f);
	camera->setNearFar(glm::vec2(1.0f, 1000.0f));
	camera->setHeight(4.0f);
	camera->setMainCameraFlag(true);
	cameraTransform->lookAt(cube1Transform->getPosition());
	camera->setCameraController(shared_ptr<CameraController>(new CameraController()));

	auto lightGo = SceneUtility::createEmptyGameObject();
	auto lightGo2 = SceneUtility::createEmptyGameObject();

	auto lightComp = lightGo->addComponent<Light>();
	lightComp->setLightType(LightType::DirectionalLight);
	auto lightComp2 = lightGo2->addComponent<Light>();
	lightComp2->setLightType(LightType::PointLight);
	lightComp->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightComp2->setColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

	lightGo->getTransform()->setPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	lightGo->getTransform()->setEularAngle(glm::vec3(90.0f, 45.0f, 0.f));

	auto forward = lightGo->getTransform()->getForward();
	std::cout << forward.x << "," << forward.y << "," << forward.z << std::endl;

	lightGo2->getTransform()->setPosition(glm::vec3(-10.0f, -10.0f, -10.0f));
	lightGo2->getTransform()->setEularAngle(glm::vec3(30.0f, 30.0f, 0.f));

	scene->addObject(go1);
	scene->addObject(go2);
	scene->addObject(go3);
	scene->addObject(cameraGo);
	scene->addObject(lightGo);
	//scene->addObject(lightGo2);
	//Graphics::drawMeshNow(cubeMesh, mat, cube1Transform->getModelMatrix() * glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f)));
}

void destroyScene()
{
	auto sceneManager = SceneManager::getInstance();
	for (auto scene : sceneManager->getAllScenes())
	{
		sceneManager->removeScene(scene);
	}
	pipeline->uninit();
	delete pipeline;
	pipeline = nullptr;
	Graphics::reset(nullptr);
}

void logicLoop()
{
	auto scenes = SceneManager::getInstance()->getAllScenes();
	gameMain.run(scenes);
}

void renderLoop()
{
	pipeline->render();
}

void gameLoop()
{
	logicLoop();
	renderLoop();
}

int main(int argc, char** argv)
{
	cout << fs::current_path().string() << endl;

	cout << SHADER_ROOT << "  " << fs::is_directory(SHADER_ROOT) << endl;
	auto window = Window::getInstance();
	window->init();
	window->create(1280, 960, "TestWindow");
	// 初始化gl3w库
	gl3wInit();
	// 初始化阶段，初始化VAO，VBO等
	createScene();
	window->attachToEventSystem();

	window->gameLoop(gameLoop);

	window->detachToEventSystem();
	destroyScene();

	window->destroy();
	window->uninit();

}