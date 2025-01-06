#pragma once

#include <Engine.h>

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};


// some notes: 
// <1> they tends to explode into two directions
// <2> the maximum control outside(setting sizeBegin) is near 10.0f ~ 0.1f
class ParticleSystem
{
public:
	ParticleSystem(uint32_t maxParticles = 10000);

	void OnUpdate(Engine::Timestep ts);
	void OnRender(Engine::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);

	// for simplify same work in multiple layers
	void DefaultInit(ParticleProps& particleProps);
	void DefaultUpdate(Engine::Timestep ts, Engine::OrthographicCameraController& cameraControl, ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 0;
};