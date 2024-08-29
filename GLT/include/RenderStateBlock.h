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
enum class CullMode
{
	Off,
	Front,
	Back
};
struct ColorState 
{
	CullMode m_cullMode;
};

struct DepthState
{
	bool m_writable;
	CompareFunction m_compareFunc;
};

struct StencilState
{
};

struct RenderStateBlock
{
	ColorState m_colorState;
	DepthState m_depthState;
	StencilState m_stencilState;
};
