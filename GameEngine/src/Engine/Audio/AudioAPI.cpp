#include "egpch.h"
#include "AudioAPI.h"

#include "Platform/OpenAL/OpenALAudioAPI.h"

namespace Engine {

	AudioAPI::API AudioAPI::s_API = AudioAPI::API::OpenAL;

	Scope<AudioAPI> AudioAPI::Create()
	{
		switch (s_API)
		{
		case Engine::AudioAPI::API::None:	EG_CORE_ASSERT(false, "AudioAPI::None is currently not supported!"); return nullptr;
		case Engine::AudioAPI::API::OpenAL:	return CreateScope<OpenALAudioAPI>();
		}
		EG_CORE_ASSERT(false, "Unknown AudioAPI!");
		return nullptr;
	}
}
