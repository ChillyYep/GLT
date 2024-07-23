#pragma once
enum ComponentType {
	ComponentType_None,
	ComponentType_Renderer,
	ComponentType_Light,
	ComponentType_Transform,
	ComponentType_Camera,
};

template<typename T>
struct component_traits
{
	static const ComponentType value = ComponentType::ComponentType_None;
};

class TypeCheck
{
public:
	TypeCheck(){}
	~TypeCheck(){}
	template<typename T>
	static constexpr bool IsComponentType()
	{
		return component_traits<T>::value != ComponentType::ComponentType_None;
	}

	template<typename T>
	static constexpr bool IsComponentType(T src)
	{
		return component_traits<T>::value != ComponentType::ComponentType_None;
	}
private:

};