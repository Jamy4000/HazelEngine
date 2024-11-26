#include "hzpch.h"
#include "Hazel/Scene/Scene.h"

#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Scene/Component.h"

namespace Hazel
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
    }

    void Scene::OnUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto& [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, spriteRenderer);
        }
    }
}
