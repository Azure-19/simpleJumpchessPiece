#pragma once

#include "Scene.h"

#include <entt.hpp>

namespace Engine {

	class Entity
	{
	private:
		// shouldn't init by 0 since the very first entity takes the handle '0'.
		// COULD BE  -entt::null-  but i will receive some problems so just treat 114514 as a maximum entity
		entt::entity m_EntityHandle{ 114514 };
		// a weak pointer is more ideal
		Scene* m_Scene = nullptr;

		// -TODO: DELETE IT LATER
		friend class Scene;
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EG_CORE_ASSERT(!HasComponent<T>(), "Entity already has the component.");
			// std::forward means not unwrapping arguments and sending them along to emplace func
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			EG_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have the component.");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);	
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComonent()
		{
			EG_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have the component.");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return static_cast<uint32_t>(m_EntityHandle) != 114514; }
	};
}