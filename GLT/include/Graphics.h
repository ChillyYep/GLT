#pragma once
#include <Mesh.h>
#include <Material.h>
#include <Camera.h>
#include <ResourceManager.h>
#include <ShaderPropertyNames.h>
#include <SceneManager.h>
#include <SceneUtility.h>
#include <ShaderUtils.h>

class Graphics
{
public:
	Graphics() {}
	~Graphics() {}
	/// <summary>
	/// ��������Mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="camera"></param>
	/// <param name="modelMatrix"></param>
	static void drawMeshNow(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix);
	/// <summary>
	/// ��Mesh���뵽���ƶ���
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="camera"></param>
	/// <param name="modelMatrix"></param>
	static void drawMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix);
	/// <summary>
	/// �����Ѿ��������ڴ��Mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="camera"></param>
	/// <param name="modelMatrix"></param>
	static void drawRequestedMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix);
private:
};