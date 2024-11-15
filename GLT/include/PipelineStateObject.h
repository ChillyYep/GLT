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
	int m_texUnit = 0;
	MeshResourceIdentifier* m_meshIdentifier = nullptr;
	Material* m_material = nullptr;
	ConstantBufferSet* m_constantBufferSet = nullptr;

	std::map<std::string, TextureResourceIdentifier*> m_globalTextureResources;

	std::vector<ShaderUniformProperty> m_uniforms;
	std::vector<TextureResourceIdentifier*> m_textureResources;
	std::vector<ShaderUniformBlockReference> m_uniformBlockRefs;
};