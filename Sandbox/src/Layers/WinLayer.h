#pragma once

#include <Engine.h>

// just keep it simple for now, no framebuffer everything is primitive
class WinLayer : public Engine::Layer
{
private:
	uint32_t m_Winner = 0;

	Engine::OrthographicCameraController m_CameraController;
	Engine::Ref<Engine::Texture2D> m_BackgroundTexture,
		m_QuitButtonTex, m_WinUITex, m_TintQuad;

	std::array<Engine::Ref<Engine::Button>, 1> m_ButtonList = { nullptr };
	Engine::LayerButton* m_QuitButton;
	int32_t m_ButtonChoice = 0; // auto chose when winning
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
public:
	WinLayer();
	virtual ~WinLayer() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnEvent(Engine::Event& e) override;
};

