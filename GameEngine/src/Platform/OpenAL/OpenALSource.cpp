#include "egpch.h"
#include "OpenALSource.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Engine {

	OpenALSource::OpenALSource()
	{
		alGenSources(1, &m_SourceID);
	}

	OpenALSource::~OpenALSource()
	{
		alDeleteSources(1, &m_SourceID);
	}

	void OpenALSource::Bind() const
	{
		alSourcei(m_SourceID, AL_BUFFER, m_BufferID);
	}

	void OpenALSource::UnBind() const
	{
		alSourcei(m_SourceID, AL_BUFFER, 0);
	}

	void OpenALSource::SetMuteState(bool muteState)
	{
		alSourcei(m_SourceID, AL_GAIN, muteState);
	}

	void OpenALSource::SetGain(float gain)
	{
		alSourcef(m_SourceID, AL_GAIN, gain);
	}

	void OpenALSource::SetPitch(float pitch)
	{
		alSourcef(m_SourceID, AL_PITCH, pitch);
	}

	void OpenALSource::SetLoopState(bool loopState)
	{
		alSourcei(m_SourceID, AL_LOOPING, static_cast<ALboolean>(loopState)); // just to tell it's al
	}

	void OpenALSource::SetPosition(const glm::vec3& position)
	{
		alSource3f(m_SourceID, AL_POSITION, position.x, position.y, position.z);
	}

	void OpenALSource::SetVelocity(const glm::vec3& velocity)
	{
		alSource3f(m_SourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}
}