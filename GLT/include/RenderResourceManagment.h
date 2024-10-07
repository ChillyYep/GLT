#pragma once
#include <RenderEventSystem.h>
#include <map>
#include <unordered_map>
#include <ResourceIdentifier.h>
#include <DeviceBase.h>

class RenderResourceManagement :public Singleton<RenderResourceManagement>, public RenderEventListener
{
public:
	inline void setup(DeviceBase* device) { m_device = device; }

	inline void regist()
	{
		RenderEventSystem::getInstance()->addListener(RenderEventId::RequestResources, this);
		RenderEventSystem::getInstance()->addListener(RenderEventId::DestroyResources, this);
		RenderEventSystem::getInstance()->addListener(RenderEventId::UpdateResources, this);
	}

	inline void unregist()
	{
		RenderEventSystem::getInstance()->removeListener(RenderEventId::RequestResources, this);
		RenderEventSystem::getInstance()->removeListener(RenderEventId::DestroyResources, this);
		RenderEventSystem::getInstance()->removeListener(RenderEventId::UpdateResources, this);
	}

	void onNotify(RenderEvent& renderEvent) override;

	ResourceIdentifier* getResourceIdentifier(ResourceType resourceType, unsigned int instanceId);

private:
	void generateResourceIdentifier(ResourceType resourceType, std::vector<Object*>& objects);

	void destroyResourceIdentifier(ResourceType resourceType, std::vector<Object*>& objects);

	void updateResouceIdentifier(ResourceType resourceType, std::vector<Object*>& objects);

	inline std::vector<unsigned int> getInstanceIds(std::vector<Object*>& objects)
	{
		std::vector<unsigned int> instanceIds(objects.size());
		for (size_t i = 0; i < objects.size(); ++i)
		{
			instanceIds[i] = objects[i]->getInstanceId();
		}
		return instanceIds;
	}

	template<typename Identifier>
	inline std::vector<Identifier> getResourceIdentifier(std::vector<unsigned int>& instanceIds, std::unordered_map<unsigned int, Identifier> identifierMap)
	{
		std::vector<Identifier> identifiers = std::vector<Identifier>(instanceIds.size());
		for (size_t i = 0; i < instanceIds.size(); ++i)
		{
			auto instanceId = instanceIds[i];
			auto& resourceIdentifier = identifierMap[instanceId];
			identifiers[i] = resourceIdentifier;
		}
		return identifiers;
	}

	template<typename T, typename Identifier>
	inline std::vector<T*> filterNewResources(std::vector<Object*>& objects, std::unordered_map<unsigned int, Identifier>& identifiers)
	{
		std::vector<T*> resourcePtrs;
		for (int i = 0; i < objects.size(); ++i)
		{
			if (identifiers.find(objects[i]->getInstanceId()) != identifiers.end())
			{
				continue;
			}
			resourcePtrs.push_back(static_cast<T*>(objects[i]));
		}
		return resourcePtrs;
	}

	std::unordered_map<unsigned int, MeshResourceIdentifier> m_meshResources;
	std::unordered_map<unsigned int, TextureResourceIdentifier> m_textureResources;
	std::unordered_map<unsigned int, SamplerResouceIdentifier> m_samplerResources;
	std::unordered_map<unsigned int, RenderTargetIdentifier> m_renderTargetResources;
	std::unordered_map<unsigned int, RenderBufferIdentifier> m_renderBufferResources;

	DeviceBase* m_device;
};