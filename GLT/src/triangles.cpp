#include<iostream>
#include "Common.h"
#include <Graphics.h>
#include <CameraOperation.h>
#include <stb_image.h>
#include <gamemain.h>
using namespace std;

const GLuint Triangles = 0;
const GLuint NumVAOs = 1;
const GLuint ArrayBuffer = 0;
const GLuint NumBuffers = 1;
const GLuint vPosition = 0;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;
RenderPipeline* pipeline;
GameMain gameMain;

void CreateScene()
{
	glm::ivec2 screenSize = Window::getInstance()->getSize();
	pipeline = new RenderPipeline();
	pipeline->init();
	Graphics::reset(pipeline);
	auto scene = shared_ptr<Scene>(new Scene());
	scene->SetActived(true);
	scene->SetMainScene(true);
	SceneManager::getInstance()->addScene(scene);
	// ����
	auto shader = shared_ptr<Shader>(new Shader("cube_tex"));

	auto mat = shared_ptr<Material>(new Material(shader));
	auto tex = shared_ptr<Texture2D>(new Texture2D());
	// ��Ҫ����һ�����ļ���������Ŀ�
	tex->load("Resources/wall.jpg");
	tex->SetInternalFormat(TextureInternalFormat::RGB8);
	tex->SetLevels(4);
	tex->SetPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
	tex->SetWrapModeS(TextureWrapMode::Repeat);
	tex->SetWrapModeT(TextureWrapMode::Repeat);
	tex->SetTextureFilter(TextureFilterMode::Linear_Mipmap_Linear);

	mat->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(tex)));
	// ����
	auto cubeMesh = PrimitiveUtils::createCube();

	auto go1 = SceneUtility::CreateMeshGameObject(cubeMesh, mat);
	auto cube1Transform = go1->getComponent<Transform>();
	cube1Transform->SetPosition(glm::vec3(0.0f));
	cube1Transform->SetScale(glm::vec3(1.0f));
	cube1Transform->SetEularAngle(glm::vec3(0.0f));

	auto go2 = SceneUtility::CreateMeshGameObject(cubeMesh, mat);
	auto cube2Transform = go2->getComponent<Transform>();
	cube2Transform->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	cube2Transform->SetScale(glm::vec3(1.0f));
	cube2Transform->SetEularAngle(glm::vec3(0.0f));

	auto cameraGo = SceneUtility::CreateEmptyGameObject();
	auto cameraTransform = cameraGo->getTransform();
	cameraTransform->SetPosition(glm::vec3(10.0f, 2.0f, 10.0f));
	cameraTransform->SetScale(glm::vec3(1.0f));
	cameraTransform->SetEularAngle(glm::vec3(0.0f));

	auto camera = cameraGo->addComponent<Camera>();
	camera->SetViewPort(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	camera->SetOrtho(false);
	camera->SetAspect((GLfloat)screenSize.x / (GLfloat)screenSize.y);
	camera->SetFOV(45.0f);
	camera->SetNearFar(glm::vec2(1.0f, 1000.0f));
	camera->SetHeight(4.0f);
	camera->SetMainCameraFlag(true);
	cameraTransform->LookAt(cube1Transform->GetPosition());
	camera->setCameraController(shared_ptr<CameraController>(new CameraController()));

	auto lightGo = SceneUtility::CreateEmptyGameObject();
	auto lightGo2 = SceneUtility::CreateEmptyGameObject();

	auto lightComp = lightGo->addComponent<Light>();
	lightComp->SetLightType(LightType::DirectionalLight);
	auto lightComp2 = lightGo2->addComponent<Light>();
	lightComp2->SetLightType(LightType::PointLight);
	lightComp->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightComp2->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));
	//lightComp2->SetAngle(90.0f);

	lightGo->getTransform()->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	lightGo->getTransform()->SetEularAngle(glm::vec3(30.0f, 30.0f, 0.f));

	lightGo2->getTransform()->SetPosition(glm::vec3(-10.0f, -10.0f, -10.0f));
	lightGo2->getTransform()->SetEularAngle(glm::vec3(30.0f, 30.0f, 0.f));

	scene->addObject(go1);
	scene->addObject(go2);
	scene->addObject(cameraGo);
	scene->addObject(lightGo);
	scene->addObject(lightGo2);
	Graphics::drawMeshNow(cubeMesh, mat, cube1Transform->GetMatrix() * glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f)));
}

void DestroyScene()
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
	// ��ʼ��gl3w��
	gl3wInit();
	// ��ʼ���׶Σ���ʼ��VAO��VBO��
	CreateScene();
	window->attachToEventSystem();

	auto windowSize = window->getSize();
	RenderTexture* renderTexture = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGB8, RenderTextureDepthStencilType::Depth16,
		RenderTextureDepthStencilType::None);
	renderTexture->create(false);

	window->gameLoop(gameLoop);

	window->detachToEventSystem();
	DestroyScene();

	window->destroy();
	renderTexture->release(false);
	delete renderTexture;
	renderTexture = nullptr;
	window->uninit();

}