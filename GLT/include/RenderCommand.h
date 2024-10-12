#pragma once
#include <Singleton.h>
#include <CommonDefine.h>
#include <ResourceIdentifier.h>
#include <Light.h>
#include <Material.h>
#include <Mesh.h>
#include <Renderer.h>

enum class RenderCommandType
{
	Invalid,
	Default,
	SetRenderTarget,
	SetViewPort,
	ClearColor,
	DrawMesh,
	DrawRenderer,
	SetProjectionMatrix,
	SetViewMatrix,
	SetShadowMap
};

class RenderCommandParam
{
public:
	RenderCommandParam() {}
	virtual ~RenderCommandParam() {}

private:

};

struct RenderCommand
{
	RenderCommandType commandType;
	RenderCommandParam* param;
};


template<typename T>
struct rendercommandparam_traits
{
	static const RenderCommandType value = RenderCommandType::Invalid;
};

template<>
struct rendercommandparam_traits<RenderCommandParam>
{
	static const RenderCommandType value = RenderCommandType::Default;
};

#define RENDERCOMMANDPARAM_CLASS(COMMANDPARAMCLASS,RENDERCOMMANDENUM) ENUM_BINDING_CLASS(COMMANDPARAMCLASS, RenderCommandParam, RenderCommandType, RENDERCOMMANDENUM, rendercommandparam_traits)

RENDERCOMMANDPARAM_CLASS(SetRenderTargetParam, RenderCommandType::SetRenderTarget)
{
public:
	SetRenderTargetParam() {}
	~SetRenderTargetParam() {}

	RenderTargetIdentifier* identifier;
private:
};

RENDERCOMMANDPARAM_CLASS(ClearColorRenderParam, RenderCommandType::ClearColor)
{
public:
	ClearColorRenderParam() {}
	~ClearColorRenderParam() {}

	float r, g, b, a;
};
RENDERCOMMANDPARAM_CLASS(DrawMeshParam, RenderCommandType::DrawMesh)
{
public:
	DrawMeshParam() {}
	~DrawMeshParam() {}

	Mesh* m_meshPtr;
	Material* m_materialPtr;
	glm::mat4 m_modelMatrix;
};

RENDERCOMMANDPARAM_CLASS(DrawRendererParam, RenderCommandType::DrawRenderer)
{
public:
	DrawRendererParam() {}
	~DrawRendererParam() {}

	Renderer* m_rendererPtr;
};


RENDERCOMMANDPARAM_CLASS(SetViewPortParam, RenderCommandType::SetViewPort)
{
public:
	SetViewPortParam() {}
	~SetViewPortParam() {}

	int m_x, m_y;
	int m_width, m_height;
};

RENDERCOMMANDPARAM_CLASS(SetViewMatrixParam, RenderCommandType::SetViewMatrix)
{
public:
	SetViewMatrixParam() {}
	~SetViewMatrixParam() {}

	glm::mat4 m_viewMatrix;
};

RENDERCOMMANDPARAM_CLASS(SetProjectionMatrixParam, RenderCommandType::SetProjectionMatrix)
{
public:
	SetProjectionMatrixParam() {}
	~SetProjectionMatrixParam() {}

	glm::mat4 m_projectionMatrix;
};
RENDERCOMMANDPARAM_CLASS(SetShadowMapParam, RenderCommandType::SetShadowMap)
{
public:
	SetShadowMapParam() {}
	~SetShadowMapParam() {}
	ShadowMapType m_shadowType;
	Texture2D* m_shadowMap;
};

class RenderCommandParamFactory :public Singleton<RenderCommandParamFactory>
{
public:
	RenderCommandParamFactory() {}
	~RenderCommandParamFactory() {}

	/// <summary>
	/// 创建命令参数
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* createParam()
	{
		static_assert(rendercommandparam_traits<T>::value != RenderCommandType::Invalid);
		return new T();
	}
	/// <summary>
	/// 释放命令参数
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="param"></param>
	template<typename T>
	void releaseParam(T*& param)
	{
		static_assert(rendercommandparam_traits<T>::value != RenderCommandType::Invalid);
		if (param != nullptr)
		{
			delete param;
			param = nullptr;
		}
	}

private:
	//std::vector<RenderCommandParam*> m_params;
};
