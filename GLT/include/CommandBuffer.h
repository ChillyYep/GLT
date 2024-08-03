#pragma once
#include <Enums.h>
#include <vector>
#include <Allocator.h>
#include <ResourceManager.h>

class RenderCommandParam
{
public:
	RenderCommandParam() {}
	~RenderCommandParam() {}

private:

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

struct RenderCommand
{
	RenderCommandType commandType;
	RenderCommandParam* param;
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
	T* CreateParam()
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
	void ReleaseParam(T*& param)
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
class CommandBuffer
{
public:
	CommandBuffer()
	{
	}
	~CommandBuffer() {}

	void RequestRenderTexture(int width, int height, TextureFormat textureFormat, TextureFilterMode filterMode)
	{
		RenderCommand command = RenderCommand();
		RequestRenderTextureParam* renderTextureParam = RenderCommandParamFactory::getInstance()->CreateParam<RequestRenderTextureParam>();
		renderTextureParam->width = width;
		renderTextureParam->height = height;
		renderTextureParam->textureFormat = textureFormat;
		renderTextureParam->filterMode = filterMode;
		command.commandType = RenderCommandType::RequestRenderTexture;
		command.param = renderTextureParam;
		m_cmdList.push_back(command);
	}

	void Release()
	{
		for (int i = 0;i < m_cmdList.size();++i)
		{
			RenderCommandParamFactory::getInstance()->ReleaseParam(m_cmdList[i].param);
		}
	}
private:
	static RenderCommandParamFactory s_renderCommandDict;
	std::string m_name;
	std::vector<RenderCommand> m_cmdList;
};
