#pragma once

#include "Engine/Audio/AudioSource.h"

namespace Engine{
	// learn from rendererAPI
	class AudioAPI
	{
	public:
		enum class API
		{
			None = 0, OpenAL = 1
		};
	private:
		static API s_API;

	public:
		virtual void Init() = 0;
		// is there a function clear?
		virtual void Close() = 0;

		virtual void PlaySource(const Ref<AudioSource>& source) = 0;
		virtual void PauseSource(const Ref<AudioSource>& source) = 0;
		virtual void StopSource(const Ref<AudioSource>& source) = 0;
		virtual uint32_t CheckSource(const Ref<AudioSource>& source) = 0;
		virtual void RewindSource(const Ref<AudioSource>& source) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<AudioAPI> Create();
	};
}

