#pragma once

#include <Engine.h>

#include "Effects/ParticleSystem.h"

// index 0, considered as a exception
class Sandbox2D : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;

	// TEMPORARY before a 2D Renderer
	//Engine::Ref<Engine::VertexArray> m_SquareVA;
	//Engine::Ref<Engine::Shader> m_FlatColorShader;
	// ^now we have a simple 2D Renderer, we don't need to define them any more
	Engine::Ref<Engine::Texture2D> m_Texture;
#if MAP_PARTICLE_EXAMPLE_CODE
	// subTextures
	Engine::Ref<Engine::Texture2D> m_SpriteSheet;
	Engine::Ref<Engine::SubTexture2D> m_TextureStairs,
		m_TextureSlabStone, m_TextureTree;


	// config simple particle system 
	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
	// a simple way to implement tiled map --- to bind char to a subTexture
	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Engine::Ref<Engine::SubTexture2D>> s_TextureMap;
#endif

#if PRIMITIVE_AUDIO_EXAMPLE_CODE
	//Engine::AudioPlayer m_AudioPlayer;
	//Engine::WaveAudio m_BackgroundMusic;
#endif

	Engine::Ref<Engine::Scene> m_ActiveScene;
	Engine::Entity m_SquareEntity;
	Engine::Entity m_CameraEntity;

	glm::vec4 m_SquareColor = { 0.9f, 0.3f, 0.8f, 1.0f };
	Engine::Ref<Engine::Framebuffer> m_Framebuffer;

	// viewports
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& e) override;
};

