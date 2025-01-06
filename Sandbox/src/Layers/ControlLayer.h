#pragma once
// showing how to control in game
#include <Engine.h>

// just keep it simple for now, no framebuffer everything is primitive
class ControlLayer : public Engine::Layer
{
private:
	Engine::OrthographicCameraController m_CameraController;
	Engine::Ref<Engine::Texture2D> m_BackgroundTexture,
		m_QuitButtonTex;

	std::array<Engine::Ref<Engine::Button>, 1> m_ButtonList = { nullptr };
	Engine::LayerButton* m_QuitButton;
	int32_t m_ButtonChoice = -1;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
public:
	ControlLayer();
	virtual ~ControlLayer() = default;
	// behave as a constructor++ and the other destructor++
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnEvent(Engine::Event& e) override;
};