#pragma once
#include <CommonDefine.h>
#include <GLMath.h>
#include <Transform.h>
#include <Component.h>
#include <GameObject.h>
#include <Light.h>
#include <CameraOperation.h>

COMPONENT_CLASS(Camera, ComponentType::Camera)
{
public:
	Camera() :m_isMainCamera(false), m_ortho(false), m_aspect(0.0f), m_fov(0.0f),
		m_height(0.0f), m_nearFar(glm::zero<glm::vec2>()), m_cameraController(nullptr), m_viewPort(glm::zero<glm::vec4>()),
		m_viewMatrix(glm::zero<glm::mat4>()), m_projectMatrix(glm::zero<glm::mat4>())
	{}
	~Camera() {}
	ComponentType getComponentType() override { return ComponentType::Camera; }

	inline void lookAt(std::shared_ptr<Transform> target)
	{
		static_cast<GameObject*>(m_gameObjectPtr)->getTransform()->lookAt(target->getPosition());
	}

	static glm::mat4x4 computeViewMatrix(glm::quat rotation, glm::vec3 position);

	static glm::mat4x4 computeOrthoProjectionMatrix(float aspect, float height, float nearPlane, float farPlane);

	static glm::mat4x4 computePerspectiveProjectionMatrix(float aspect, float fov, float nearPlane, float farPlane);

	inline glm::mat4x4 getViewMatrix() { recomputeViewMatrix(); return m_viewMatrix; }

	inline glm::mat4x4 getProjectMatrix() { recomputeProjectMatrix(); return m_projectMatrix; }

	inline glm::vec4 getProjectParams() { return glm::vec4(1.0f, m_nearFar.x, m_nearFar.y, 1 / m_nearFar.y); }

	inline void setCameraController(std::shared_ptr<CameraController> cameraController)
	{
		m_cameraController = cameraController;
		auto transform = static_cast<GameObject*>(m_gameObjectPtr)->getTransform();
		m_cameraController->attach(transform);
	}

	__GET_SET_PROPERTY__(Ortho, GLboolean, m_ortho)
		__GET_SET_PROPERTY__(FOV, GLfloat, m_fov)
		__GET_SET_PROPERTY__(Aspect, GLfloat, m_aspect)
		__GET_SET_PROPERTY__(NearFar, glm::vec2, m_nearFar)
		__GET_SET_PROPERTY__(Height, GLfloat, m_height)
		__GET_SET_PROPERTY__(ViewPort, glm::vec4, m_viewPort)
		__GET_SET_PROPERTY__(MainCameraFlag, bool, m_isMainCamera)
private:
	void recomputeViewMatrix();

	void recomputeProjectMatrix();

	bool m_isMainCamera;

	GLboolean m_ortho;

	GLfloat m_fov;

	GLfloat m_aspect;

	GLfloat m_height;

	glm::vec2 m_nearFar;

	glm::vec4 m_viewPort;

	glm::mat4x4 m_viewMatrix;

	glm::mat4x4 m_projectMatrix;

	std::shared_ptr<CameraController> m_cameraController;
};
