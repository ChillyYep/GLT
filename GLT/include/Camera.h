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
	ComponentType GetComponentType() override { return ComponentType::Camera; }

	inline void LookAt(std::shared_ptr<Transform> target)
	{
		GetTransform()->LookAt(target->GetPosition());
	}

	inline glm::mat4x4 GetViewMatrix() { ReComputeViewMatrix(); return m_viewMatrix; }

	inline glm::mat4x4 GetProjectMatrix() { ReComputeProjectMatrix(); return m_projectMatrix; }

	inline void SetCameraController(std::shared_ptr<CameraController> cameraController)
	{
		m_cameraController = cameraController;
		m_cameraController->Attach(GetTransform());
	}

	inline std::shared_ptr<Transform>& GetTransform()
	{
		return m_gameObjectPtr->GetTransform();
	}

	__GET_SET_PROPERTY__(Ortho, GLboolean, m_ortho)
		__GET_SET_PROPERTY__(FOV, GLfloat, m_fov)
		__GET_SET_PROPERTY__(Aspect, GLfloat, m_aspect)
		__GET_SET_PROPERTY__(NearFar, glm::vec2, m_nearFar)
		__GET_SET_PROPERTY__(Height, GLfloat, m_height)
		__GET_SET_PROPERTY__(ViewPort, glm::vec4, m_viewPort)
		__GET_SET_PROPERTY__(MainCameraFlag, bool, m_isMainCamera)
private:
	inline void ReComputeViewMatrix()
	{
		glm::vec3 left = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		auto rotation = GetTransform()->GetRotation();
		left = rotation * left;
		up = rotation * up;
		forward = rotation * forward;
		// 正交矩阵的转置矩阵就是其逆矩阵
		// 旋转+平移还原视图矩阵
		m_viewMatrix = glm::transpose(glm::mat4(glm::vec4(left, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0)));
		m_viewMatrix = glm::translate(m_viewMatrix, -GetTransform()->GetPosition());
	}

	inline void ReComputeProjectMatrix()
	{
		if (m_ortho)
		{
			GLfloat width = m_aspect * m_height;
			m_projectMatrix = glm::ortho(-width, width, -m_height, m_height, m_nearFar.x, m_nearFar.y);
		}
		else {
			m_projectMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearFar.x, m_nearFar.y);
		}
	}


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
