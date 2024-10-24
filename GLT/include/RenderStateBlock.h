#pragma once
enum class CompareFunction
{
	//
	// ժҪ:
	//     Depth or stencil test is disabled.
	Disabled,
	//
	// ժҪ:
	//     Never pass depth or stencil test.
	Never,
	//
	// ժҪ:
	//     Pass depth or stencil test when new value is less than old one.
	Less,
	//
	// ժҪ:
	//     Pass depth or stencil test when values are equal.
	Equal,
	//
	// ժҪ:
	//     Pass depth or stencil test when new value is less or equal than old one.
	LessEqual,
	//
	// ժҪ:
	//     Pass depth or stencil test when new value is greater than old one.
	Greater,
	//
	// ժҪ:
	//     Pass depth or stencil test when values are different.
	NotEqual,
	//
	// ժҪ:
	//     Pass depth or stencil test when new value is greater or equal than old one.
	GreaterEqual,
	//
	// ժҪ:
	//     Always pass depth or stencil test.
	Always
};
template<typename T>
struct RenderStateParam {
	void setValue(T newValue)
	{
		if (newValue != m_value)
		{
			m_dirty = true;
		}
		m_value = newValue;
	}
	T getValue() { return m_value; }

	bool isDirty() { return m_dirty; }

	void clearDirty() { m_dirty = false; }

	bool operator==(const RenderStateParam<T> right) const
	{
		return m_value == right.m_value;
	}
	RenderStateParam operator=(T value)
	{
		setValue(value);
		return *this;
	}
private:
	T m_value;
	bool m_dirty = true;
};

enum class CullMode
{
	Off,
	Front,
	Back
};
enum class BlendMode {
	One,
	Zero,
	DstColor,
	SrcColor,
	DstAlpha,
	SrcAlpha,
	OneMinusDscColor,
	OneMinusSrcColor,
	OneMinuesDstAlpha,
	OneMinuesSrcAlpha,
	SrcAlphaSaturate,
};
struct ColorState
{
	RenderStateParam<CullMode> m_cullMode;
	RenderStateParam<bool> m_blendModeEnabled;
	RenderStateParam<BlendMode> m_srcBlendMode;
	RenderStateParam<BlendMode> m_dstBlendMode;
	RenderStateParam<glm::bvec4> m_rgbaWritable;

	void copyTo(ColorState& colorState)
	{
		colorState.m_cullMode = m_cullMode.getValue();
		colorState.m_blendModeEnabled = m_blendModeEnabled.getValue();
		colorState.m_srcBlendMode = m_srcBlendMode.getValue();
		colorState.m_dstBlendMode = m_dstBlendMode.getValue();
		colorState.m_rgbaWritable = m_rgbaWritable.getValue();
	}
	bool isSame(const ColorState& other)
	{
		return m_cullMode == other.m_cullMode &&
			m_blendModeEnabled == other.m_blendModeEnabled &&
			m_srcBlendMode == other.m_srcBlendMode &&
			m_dstBlendMode == other.m_dstBlendMode &&
			m_rgbaWritable == other.m_rgbaWritable;
	}
};

struct DepthState
{
	RenderStateParam<glm::vec2> m_depthRange;
	RenderStateParam<bool> m_writable;
	RenderStateParam<CompareFunction> m_compareFunc;
	bool isSame(const DepthState& other)
	{
		return m_depthRange == other.m_depthRange &&
			m_writable == other.m_writable &&
			m_compareFunc == other.m_compareFunc;
	}
	void copyTo(DepthState& depthState)
	{
		depthState.m_depthRange = m_depthRange.getValue();
		depthState.m_writable = m_writable.getValue();
		depthState.m_compareFunc = m_compareFunc.getValue();
	}
};

struct StencilState
{
	bool isSame(const StencilState& otherRenderStateBlock)
	{
		return true;
	}
	void copyTo(StencilState& stencilState)
	{
	}
};

struct RenderStateBlock
{
	ColorState m_colorState;
	DepthState m_depthState;
	StencilState m_stencilState;

	void copyTo(RenderStateBlock& renderStateBlock)
	{
		m_colorState.copyTo(renderStateBlock.m_colorState);
		m_depthState.copyTo(renderStateBlock.m_depthState);
		m_stencilState.copyTo(renderStateBlock.m_stencilState);
	}

	bool isSame(const RenderStateBlock& other)
	{
		return m_colorState.isSame(other.m_colorState) &&
			m_depthState.isSame(other.m_depthState) &&
			m_stencilState.isSame(other.m_stencilState);
	}
};
