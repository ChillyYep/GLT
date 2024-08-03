#pragma once
enum class ComponentType {
	Invalid,
	Renderer,
	Light,
	Transform,
	Camera,
};

template<typename T>
struct component_traits
{
	static const ComponentType value = ComponentType::Invalid;
};

#define COMPONENT_CLASS(CLASSTYPE,CLASSTYPEENUM) ENUM_BINDING_CLASS(CLASSTYPE,Component,ComponentType,CLASSTYPEENUM,component_traits)

#define DERIVED_COMPONENT_CLASS(CLASSTYPE,BASECLASSTYPE,CLASSTYPEENUM) ENUM_BINDING_CLASS(CLASSTYPE,BASECLASSTYPE,ComponentType,CLASSTYPEENUM,component_traits)


class TypeCheck
{
public:
	TypeCheck(){}
	~TypeCheck(){}
	template<typename T>
	static constexpr bool IsComponentType()
	{
		return component_traits<T>::value != ComponentType::Invalid;
	}

	template<typename T>
	static constexpr bool IsComponentType(T src)
	{
		return component_traits<T>::value != ComponentType::Invalid;
	}
private:

};