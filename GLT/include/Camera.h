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
		getTransform()->lookAt(target->getPosition());
	}

	inline void isInFrustum()
	{

	}

	inline glm::mat4x4 getViewMatrix() { recomputeViewMatrix(); return m_viewMatrix; }

	inline glm::mat4x4 getProjectMatrix() { recomputeProjectMatrix(); return m_projectMatrix; }

	inline void setCameraController(std::shared_ptr<CameraController> cameraController)
	{
		m_cameraController = cameraController;
		m_cameraController->attach(getTransform());
	}

	inline std::shared_ptr<Transform>& getTransform()
	{
		return m_gameObjectPtr->getTransform();
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
