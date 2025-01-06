#pragma once

#include "Engine/Audio/AudioCommand.h"

namespace Engine {

	class AudioPlayer 
	{
	private:
	public:
		static void Init();

		inline static AudioAPI::API GetAPI() { return AudioAPI::GetAPI(); }
	};
}

#if PRIMITIVE_EXAMPLE
#include "AudioLoader.h"

namespace Engine {

	// test case
	class AudioPlayer
	{
	private:
		ALCdevice* device = nullptr;
		ALCcontext* context = nullptr;
		ALuint audioSource;
		ALfloat audioSourcePos[3];
		ALfloat audioSourceVel[3];

		void InitializeOpenAL();

		void CloseOpenAL();

	public:
		AudioPlayer();
		~AudioPlayer();


		void Play(WaveAudio& wave, bool loopState, float posX, float posY, float posZ, float velX, float velY, float velZ);

		void Stop();

	};
}
#endif