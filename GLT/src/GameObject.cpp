#include "GameObject.h"
void GameObject::destroy()
{
	ComponentOwner::destroy();
	// 子节点销毁
	auto transform = getTransform();
	if (transform != nullptr)
	{
		auto children = transform->getChildren();
		for (const auto& child : children)
		{
			child->getGameObject()->destroy();
		}
		transform->setParent(nullptr);
	}
}
