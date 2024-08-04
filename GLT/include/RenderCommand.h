#pragma once
#include <Singleton.h>
#include <CommonDefine.h>
#include <ResourceIdentifier.h>

enum class RenderCommandType
{
	Invalid,
	Default,
	RequestRenderTexture,
	SetRenderTarget,
	Draw
};

class RenderCommandParam
{
public:
	RenderCommandParam() {}
	~RenderCommandParam() {}

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

RENDERCOMMANDPARAM_CLASS(RequestRenderTextureParam, RenderCommandType::RequestRenderTexture)
{
public:
	RequestRenderTextureParam() {}
	~RequestRenderTextureParam() {}

	int width;
	int height;
	TextureFormat textureFormat;
	TextureFilterMode filterMode;
private:
};

RENDERCOMMANDPARAM_CLASS(SetRenderTargetParam, RenderCommandType::SetRenderTarget)
{
public:
	SetRenderTargetParam() {}
	~SetRenderTargetParam() {}

	RenderTargetIdentifier* identifier;
private:
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
