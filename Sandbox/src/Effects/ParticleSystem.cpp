#include "ParticleSystem.h"

#include <random>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>


class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_PoolIndex(maxParticles - 1)
{
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Engine::Timestep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Engine::OrthographicCamera& camera)
{
	
	Engine::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		// Add temporary depth
		Engine::Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y, 0.2f }, { size * 0.1f, size * 0.1f }, particle.Rotation, color);

	}
	Engine::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

void ParticleSystem::DefaultInit(ParticleProps& particleProps)
{
	// particle init
	particleProps.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 230 / 255.0f, 1.0f };
	particleProps.ColorEnd = { 255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 0.4f };
	particleProps.SizeBegin = 0.1f, particleProps.SizeVariation = 0.3f, particleProps.SizeEnd = 0.0f;
	particleProps.LifeTime = 1.0f;
	particleProps.Velocity = { 0.0f, 0.0f };
	particleProps.VelocityVariation = { 3.0f, 1.0f };
	particleProps.Position = { 0.0f, 0.0f };
}

void ParticleSystem::DefaultUpdate(Engine::Timestep ts, Engine::OrthographicCameraController& cameraControl, ParticleProps& particleProps)
{
	// since the mouse will follow button coordinate, it simulates well
	if (Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonLeft))
	{
		auto [x, y] = Engine::Input::GetMousePosition();
		auto width = Engine::Application::Get().GetWindow().GetWidth();
		auto height = Engine::Application::Get().GetWindow().GetHeight();

		auto bounds = cameraControl.GetBounds();
		auto pos = cameraControl.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		particleProps.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			Emit(particleProps);
	}
	else if (Engine::Input::IsKeyPressed(Engine::KeyCode::Z))
	{
		auto [x, y] = Engine::Input::GetMousePosition();
		auto width = Engine::Application::Get().GetWindow().GetWidth();
		auto height = Engine::Application::Get().GetWindow().GetHeight();

		auto bounds = cameraControl.GetBounds();
		auto pos = cameraControl.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		particleProps.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			Emit(particleProps);
	}
	else if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonA))
	{
		auto [x, y] = Engine::Input::GetMousePosition();
		auto width = Engine::Application::Get().GetWindow().GetWidth();
		auto height = Engine::Application::Get().GetWindow().GetHeight();

		auto bounds = cameraControl.GetBounds();
		auto pos = cameraControl.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		particleProps.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			Emit(particleProps);
	}
	OnUpdate(ts);
	OnRender(cameraControl.GetCamera());
}
