#pragma once

#include "Engine/Audio/AudioAPI.h"

struct ALCdevice;
struct ALCcontext;

namespace Engine {
	
	typedef enum class ALSourceState {

	}State;

	class OpenALAudioAPI : public AudioAPI
	{
	private:
		ALCdevice* m_Device;
		ALCcontext* m_Context;
	public:
		virtual void Init() override;

		virtual void Close() override;

		virtual void PlaySource(const Ref<AudioSource>& source) override;
		virtual void PauseSource(const Ref<AudioSource>& source) override;
		virtual void StopSource(const Ref<AudioSource>& source) override;
		virtual uint32_t CheckSource(const Ref<AudioSource>& source) override;
		virtual void RewindSource(const Ref<AudioSource>& source) override;

	};
	
}

