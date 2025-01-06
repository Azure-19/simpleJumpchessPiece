#include "egpch.h"
#include "OpenALAudioAPI.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Engine {

	void OpenALAudioAPI::Init()
	{
		EG_PROFILE_FUNCTION();
		m_Device = alcOpenDevice(nullptr); // open default device
		m_Context = alcCreateContext(m_Device, nullptr);
		alcMakeContextCurrent(m_Context);
		EG_CORE_ASSERT(m_Device && m_Context, "Device or context create failure!");
		// other openAL settings here(if there is)
	}

	void OpenALAudioAPI::Close()
	{
		EG_PROFILE_FUNCTION();
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_Context);
		alcCloseDevice(m_Device);
	}

	void OpenALAudioAPI::PlaySource(const Ref<AudioSource>& source)
	{
		alSourcePlay(source->GetSourceID());
	}

	void OpenALAudioAPI::PauseSource(const Ref<AudioSource>& source)
	{
		alSourcePause(source->GetSourceID());
	}

	void OpenALAudioAPI::StopSource(const Ref<AudioSource>& source)
	{
		alSourceStop(source->GetSourceID());
	}

	uint32_t OpenALAudioAPI::CheckSource(const Ref<AudioSource>& source)
	{
		ALint state;
		alGetSourcei(source->GetSourceID(), AL_SOURCE_STATE, &state);
		switch (state)
		{
		case AL_INITIAL:
			return 0;
		case AL_PLAYING:
			return 1;
		case AL_PAUSED:
			return 2;
		case AL_STOPPED:
			return 3;
		};
	}

	void OpenALAudioAPI::RewindSource(const Ref<AudioSource>& source)
	{
		alSourceRewind(source->GetSourceID());
	}
	
}