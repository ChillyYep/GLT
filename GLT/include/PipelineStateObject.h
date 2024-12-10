#pragma once
#include <vector>
#include <map>
#include <GLMath.h>
#include <ResourceIdentifier.h>
#include <Mesh.h>
#include <ConstantBuffers.h>

struct PipelineStateObject
{
	unsigned int m_program = 0;
	glm::mat4 m_modelMatrix;
	RenderType m_renderType;
	MeshResourceIdentifier* m_meshIdentifier = nullptr;
	Material* m_material = nullptr;
	ConstantBufferSet* m_constantBufferSet = nullptr;
	// 部分纹理将会被多数物体引用，如阴影贴图，因此可将该类贴图设置为全局的，与来自材质的贴图做区分
	std::map<std::string, TextureResourceIdentifier*> m_globalTextureResources;

	std::vector<ShaderUniformProperty> m_uniforms;
	std::vector<TextureResourceIdentifier*> m_textureResources;
	std::vector<ShaderUniformBlockReference> m_uniformBlockRefs;
};