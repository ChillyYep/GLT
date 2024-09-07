#pragma once
#include <Object.h>
#include <type_traits>
#include <Transform.h>

class GameObject :public ComponentOwner
{
public:
	GameObject() {}
	~GameObject() {}

	inline std::shared_ptr<Transform> getTransform() { return getComponent<Transform>(); }

	void destroy() override;
private:
};