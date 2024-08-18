#pragma once
#include <Mesh.h>
#include <Material.h>
#include <Camera.h>
#include <ResourceManager.h>
#include <ShaderPropertyNames.h>
#include <SceneManager.h>
#include <SceneUtility.h>
#include <ShaderUtils.h>
#include <RenderPipeline.h>

class Graphics
{
public:
	Graphics() {}
	~Graphics() {}
	static void reset(RenderPipeline* pipeline)
	{
		m_pipeline = pipeline;
	}
	/// <summary>
	/// ��������Mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="modelMatrix"></param>
	static void drawMeshNow(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4x4& modelMatrix);
	/// <summary>
	/// ��Mesh���뵽���ƶ���
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="modelMatrix"></param>
	static void drawMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4x4& modelMatrix);
	/// <summary>
	/// �����Ѿ��������ڴ��Mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="material"></param>
	/// <param name="modelMatrix"></param>
	static void drawRequestedMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4x4& modelMatrix);
private:
	static RenderPipeline* m_pipeline;
};