#include<iostream>
#include "Common.h"
#include <GLFW/glfw3.h>
#include <CameraOperation.h>
#include <Window.h>
#include <stb_image.h>
#include <RenderTexture.h>
#include <SceneUtility.h>
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
RenderPipeline pipeline;
GameMain gameMain;

void CreateScene()
{
	glm::ivec2 screenSize = Window::GetInstance().GetSize();

	pipeline.Init();
	auto scene = shared_ptr<Scene>(new Scene());
	scene->SetActived(true);
	scene->SetMainScene(true);
	SceneManager::GetInstance().AddScene(scene);
	// 材质
	auto shader = shared_ptr<Shader>(new Shader("cube_tex"));

	auto mat = shared_ptr<Material>(new Material(shader));
	auto tex = shared_ptr<Texture2D>(new Texture2D());
	// 需要引入一个从文件加载纹理的库
	tex->Load("Resources/wall.jpg");
	tex->SetInternalFormat(GL_RGB8);
	tex->SetLevels(4);
	tex->SetPerChannelSize(GL_UNSIGNED_BYTE);
	tex->SetWrapModeS(TextureWrapMode::TextureWrapMode_Repeat);
	tex->SetWrapModeT(TextureWrapMode::TextureWrapMode_Repeat);
	tex->SetTextureFilter(TextureFilter::TextureFilter_Linear_Mipmap_Linear);

	mat->SetProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(tex)));
	// 网格
	auto cubeMesh = PrimitiveUtils::CreateCube();

	auto go1 = SceneUtility::CreateMeshGameObject(cubeMesh, mat);
	auto cube1Transform = go1->GetComponent<Transform>();
	cube1Transform->SetPosition(glm::vec3(0.0f));
	cube1Transform->SetScale(glm::vec3(1.0f));
	cube1Transform->SetEularAngle(glm::vec3(0.0f));

	auto go2 = SceneUtility::CreateMeshGameObject(cubeMesh, mat);
	auto cube2Transform = go2->GetComponent<Transform>();
	cube2Transform->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	cube2Transform->SetScale(glm::vec3(1.0f));
	cube2Transform->SetEularAngle(glm::vec3(0.0f));

	auto cameraGo = SceneUtility::CreateEmptyGameObject();
	auto cameraTransform = cameraGo->GetTransform();
	cameraTransform->SetPosition(glm::vec3(10.0f, 2.0f, 10.0f));
	cameraTransform->SetScale(glm::vec3(1.0f));
	cameraTransform->SetEularAngle(glm::vec3(0.0f));

	auto camera = cameraGo->AddComponent<Camera>();
	camera->SetViewPort(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	camera->SetOrtho(false);
	camera->SetAspect((GLfloat)screenSize.x / (GLfloat)screenSize.y);
	camera->SetFOV(45.0f);
	camera->SetNearFar(glm::vec2(1.0f, 1000.0f));
	camera->SetHeight(4.0f);
	camera->SetMainCameraFlag(true);
	cameraTransform->LookAt(cube1Transform->GetPosition());
	camera->SetCameraController(shared_ptr<CameraController>(new CameraController()));

	auto lightGo = SceneUtility::CreateEmptyGameObject();
	auto lightGo2 = SceneUtility::CreateEmptyGameObject();

	auto lightComp = lightGo->AddComponent<DirectionalLight>();
	auto lightComp2 = lightGo2->AddComponent<PointLight>();
	lightComp->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightComp2->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));
	//lightComp2->SetAngle(90.0f);

	lightGo->GetTransform()->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	lightGo->GetTransform()->SetEularAngle(glm::vec3(30.0f, 30.0f, 0.f));

	lightGo2->GetTransform()->SetPosition(glm::vec3(-10.0f, -10.0f, -10.0f));
	lightGo2->GetTransform()->SetEularAngle(glm::vec3(30.0f, 30.0f, 0.f));

	scene->AddObject(go1);
	scene->AddObject(go2);
	scene->AddObject(cameraGo);
	scene->AddObject(lightGo);
	scene->AddObject(lightGo2);
	Graphics::DrawMeshNow(cubeMesh, mat, camera, cube1Transform->GetMatrix() * glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f)));
}

void DestroyScene()
{
	auto& sceneManager = SceneManager::GetInstance();
	for (auto scene : sceneManager.GetAllScenes())
	{
		sceneManager.RemoveScene(scene);
	}
	pipeline.UnInit();
}

void logicLoop()
{
	auto scenes = SceneManager::GetInstance().GetAllScenes();
	gameMain.Run(scenes);
}

void renderLoop()
{
	auto activedRT = RenderTexture::GetActiveRenderTexture();
	if (activedRT != nullptr)
	{
		activedRT->Active();
		activedRT->ClearColor();
		pipeline.Render();
		activedRT->BlitToWindow();
	}
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
	auto& window = Window::GetInstance();
	window.Init();
	window.Create(1280, 960, "TestWindow");
	// 初始化gl3w库
	gl3wInit();
	// 初始化阶段，初始化VAO，VBO等
	CreateScene();
	window.AttachToEventSystem();

	auto windowSize = window.GetSize();
	RenderTexture renderTexture = RenderTexture(windowSize.x, windowSize.y, GL_RGBA, RenderTextureDepthStencilType::RenderTextureDepthStencilType_Depth16,
		RenderTextureDepthStencilType::RenderTextureDepthStencilType_None);
	renderTexture.Create();
	renderTexture.Active();

	window.GameLoop(gameLoop);

	window.DetachToEventSystem();
	DestroyScene();

	window.Destroy();
	window.UnInit();

}