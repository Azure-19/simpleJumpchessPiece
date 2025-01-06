#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if MAP_PARTICLE_ENABLE
static const uint32_t s_MapWidth = 24;
static const char* m_MapTiles =				// bad in visual but somehow usefuls
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWDDDDDWWWWWWWWWWWW"
"WWWDDDDDDDDDDDDWWWWWWWWW"
"WWWDDDDDDDDDDWDDDDDWWWWW"
"WWDDDDDDDDDWWWWDDDDDWWWW"
"WWWDDDDDDWWWDWWWDDDDWWWW"
"WWWWDDDDDDWWWWWWDDDDWWWW"
"WWWWWDDDDDDDWWDDDDDWWWWW"
"WWWWWWWWWWDDWWDDDDWWWWWW"
"WWWWWWWWDWWWWDDWWWWWWWWW"
"WWWWWWWWWDDDWDDDDWWWWWWW"
"WWWWWWWWWDDDDDDDDDWWWWWW"
"WWWWWWWWWWDDDDDDWWWWWWWW"
"WWWWWWWWWWDDDDWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW"
;
#endif

void Sandbox2D::OnAttach()
{
	EG_PROFILE_FUNCTION();

	// framebuffer init
	Engine::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Engine::Framebuffer::Create(fbSpec);

	// texture init works
	m_Texture = (Engine::Texture2D::Create("assets/textures/forTest/testTexture03.png"));
#if MAP_PARTICLE_EXAMPLE_CODE
	m_SpriteSheet = (Engine::Texture2D::Create("assets/textures/forTest/RPGpack_sheet_2X.png"));

	m_TextureStairs = Engine::SubTexture2D::CreateFormCoord(m_SpriteSheet, { 7, 6 }, { 128, 128 });
	m_TextureTree = Engine::SubTexture2D::CreateFormCoord(m_SpriteSheet, { 2, 1 }, { 128, 128 }, {1, 2});
	// map works below
	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(m_MapTiles) / m_MapWidth;

	s_TextureMap['D'] = Engine::SubTexture2D::CreateFormCoord(m_SpriteSheet, {  6, 11 }, { 128, 128 });
	s_TextureMap['W'] = Engine::SubTexture2D::CreateFormCoord(m_SpriteSheet, { 11, 11 }, { 128, 128 });
	// particle init
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.01f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
#endif

#if AUDIO_EXAMPLE_CODE
	//m_BackgroundMusic.LoadWave("assets/sounds/th06_14.wav");
	//m_AudioPlayer.Play(m_BackgroundMusic, true, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	// a more modern version
	Engine::AudioPlayer2D::SetGlobalAttribute(1.0f, true);
	Engine::AudioPlayer2D::AddSource("menuBGM", "assets/sounds/th06_14.wav");
	Engine::AudioPlayer2D::PlaySource("menuBGM", 0.1f, 1.0f, true);
	Engine::AudioPlayer2D::PauseSource("menuBGM");
	Engine::AudioPlayer2D::RemoveSource("menuBGM");
#endif

	// scene init
	m_ActiveScene = Engine::CreateRef<Engine::Scene>();
	// entity init
	auto square = m_ActiveScene->CreateEntity("Square Entity");
	square.AddComponent<Engine::SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.2f, 1.0f });

	m_SquareEntity = square;

	m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
	m_CameraEntity.AddComponent<Engine::CameraComponent>();

}

void Sandbox2D::OnDetach()
{
	EG_PROFILE_FUNCTION();
}

void Sandbox2D::OnEnable()
{
	// -[WARNING]- It would disable Sandbox2D
	exit(1); // for game. 1 for exit by button
}

void Sandbox2D::OnUpdate(Engine::Timestep ts)
{
	EG_PROFILE_FUNCTION();

	// Update camera(used to use)
	m_CameraController.OnUpdate(ts);
#if RESIZE_EXAMPLE_CODE___CANNOT_BE_USE
	// Resize
	if (Engine::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		EG_INFO("Set size to ({0}, {1})", m_ViewportSize.x, m_ViewportSize.y);
	}
#endif
	// test
	if (m_SquareEntity)
	{
		auto& squareColor = m_SquareEntity.GetComponent<Engine::SpriteRendererComponent>().Color;
		squareColor.r += 0.001f;
	}

	// Render
	Engine::Renderer2D::ResetStats();
	{
		EG_PROFILE_SCOPE("Render Prep");
		m_Framebuffer->Bind();
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
		Engine::RenderCommand::Clear();
	}
	{
		EG_PROFILE_SCOPE("Render Call");

#if (RENDERER_EXAMPLE_CODE || 0)
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			Engine::Renderer2D::DrawRotatedQuad({ 1.4f, 0.0f }, { 0.5f, 0.5f }, glm::radians(45.0f), { 0.4f,0.6f,0.8f,1.0f });
			Engine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, {0.4f,0.6f,0.8f,1.0f});
			Engine::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.7f, 1.0f }, m_SquareColor);
			Engine::Renderer2D::DrawQuad({ -0.5f,-0.5f, -0.1f }, { 5.0f, 5.0f }, m_Texture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
			Engine::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture, 1.0f, glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
		}
		Engine::Renderer2D::EndScene();

		static float rotation = 0.0f;
		// shake free(for loading)
		if (ts < 1.0f)
			rotation += 50.0f * 1 / 100.0f;

		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Engine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Engine::Renderer2D::EndScene();
#endif

#if MAP_PARTICLE_EXAMPLE_CODE
		// Particle render
		if (Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonLeft))
		{
			auto [x, y] = Engine::Input::GetMousePosition();
			auto width = Engine::Application::Get().GetWindow().GetWidth();
			auto height = Engine::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		// Map render
		Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		{
			for (uint32_t y = 0; y < m_MapHeight; y++)
			{
				for (uint32_t x = 0; x < m_MapWidth; x++)
				{
					char tileType = m_MapTiles[x + y * m_MapWidth];
					Engine::Ref<Engine::SubTexture2D> texture;
					if(s_TextureMap.find(tileType) != s_TextureMap.end())
						texture = s_TextureMap[tileType];
					else
						texture = m_TextureStairs;

					Engine::Renderer2D::DrawQuad({ x, m_MapHeight - y - 1.0f, 0.0f }, { 1.0f, 1.0f }, texture);
				}
			}



			Engine::Renderer2D::DrawQuad({ 1.0f,1.0f }, { 1.0f, 2.0f }, m_TextureTree);
			Engine::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f, 1.0f }, m_TextureStairs);
		}
		Engine::Renderer2D::EndScene();

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif

#if (ENTT_EXAMPLE_CODE || 1)

		// Update scene
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
#endif

#if gamepadInputTestCase
		if (Engine::Input::IsXboxButtonPressed(Engine::XboxCode::ButtonA))
			EG_INFO("XBOX 'A' Detected!");
		if (Engine::Input::GetXboxRTPosition() > 0)
			EG_INFO("XBOX 'RT' Detected, value: {0}", Engine::Input::GetXboxRTPosition());
		if (Engine::Input::GetXboxLSPosition().first > -10 &&
			glm::pow(Engine::Input::GetXboxLSPosition().first, 2) *
			glm::pow(Engine::Input::GetXboxLSPosition().second, 2) >= 10e-5)
			EG_INFO("XBOX 'LS' Detected, value: ({0}, {1})",
				Engine::Input::GetXboxLSPosition().first,
				Engine::Input::GetXboxLSPosition().second);
#endif

	}

}

void Sandbox2D::OnImGuiRender()
{
	EG_PROFILE_FUNCTION();

#if (IMGUI_EXAMPLE_CODE||0)
	ImGui::Begin("Settings");

	// flat color example
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	// stat example
	auto stats = Engine::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	// entity example
	if (m_SquareEntity)
	{
		ImGui::Separator();
		auto& tag = m_SquareEntity.GetComponent<Engine::TagComponent>().Tag;
		ImGui::Text("%s", tag.c_str());

		auto& squareColor = m_SquareEntity.GetComponent<Engine::SpriteRendererComponent>().Color;
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor)); // change color -> crash
		ImGui::Separator();
	}
	ImGui::End();

	// viewport
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
	{
		m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		// AspectRatio may actually changed here
		m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
	}
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

#endif

}

void Sandbox2D::OnEvent(Engine::Event& e)
{
	m_CameraController.OnEvent(e);
}
