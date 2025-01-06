#pragma once

#include "Engine/Audio/AudioAPI.h"

namespace Engine {

	class AudioCommand
	{
	private:
		static Scope<AudioAPI> s_AudioAPI; // single instance
	public:
		inline static void Init()
		{
			s_AudioAPI->Init();
		}
		inline static void Close()
		{
			s_AudioAPI->Close();
		}
		inline static void PlaySource(const Ref<AudioSource>& source)
		{
			s_AudioAPI->PlaySource(source);
		}
		inline static void PauseSource(const Ref<AudioSource>& source)
		{
			s_AudioAPI->PauseSource(source);
		}
		inline static void StopSource(const Ref<AudioSource>& source)
		{
			s_AudioAPI->StopSource(source);
		}
		inline static uint32_t CheckSource(const Ref<AudioSource>& source)
		{
			return s_AudioAPI->CheckSource(source);
		}
		inline static void RewindSource(const Ref<AudioSource>& source)
		{
			s_AudioAPI->RewindSource(source);
		}
	};
}

