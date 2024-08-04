#pragma once
#include <Mesh.h>
#include <Material.h>
#include <Camera.h>
#include <ResourceManager.h>
#include <ShaderPropertyNames.h>
#include <SceneManager.h>
#include <RenderPipeline.h>
#include <SceneUtility.h>

class Graphics
{
public:
	friend class RenderPipeline;
	Graphics() {}
	~Graphics() {}
	/// <summary>
	/// 立即绘制Mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="camera"></param>
	/// <param name="modelMatrix"></param>
	static void drawMeshNow(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix);
	/// <summary>
	/// 将Mesh加入到绘制队列
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="camera"></param>
	/// <param name="modelMatrix"></param>
	static void drawMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix);
private:
	/// <summary>
	/// 绘制已经申请了内存的Mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="camera"></param>
	/// <param name="modelMatrix"></param>
	static void drawRequestedMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix);
};