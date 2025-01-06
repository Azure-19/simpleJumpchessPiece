#include "egpch.h"
#include "AudioPlayer.h"

#include "AudioPlayer2D.h"

namespace Engine {

	void AudioPlayer::Init()
	{
		AudioCommand::Init();
		AudioPlayer2D::Init();
	}
}

#if PRIMITIVE_EXAMPLE

namespace Engine {

	// test case
	void AudioPlayer::InitializeOpenAL()
	{
		device = alcOpenDevice(nullptr); // open default device
		context = alcCreateContext(device, nullptr);
		alcMakeContextCurrent(context);
	}

	void AudioPlayer::CloseOpenAL()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	AudioPlayer::AudioPlayer()
	{
		InitializeOpenAL();
		alGenSources(1, &audioSource);
	}
	AudioPlayer::~AudioPlayer()
	{
		CloseOpenAL();
	}

	void AudioPlayer::Play(WaveAudio& wave, bool loopState, float posX, float posY, float posZ, float velX, float velY, float velZ)
	{
		audioSourcePos[0] = posX;
		audioSourcePos[1] = posY;
		audioSourcePos[2] = posZ;
		audioSourceVel[0] = velX;
		audioSourceVel[1] = velY;
		audioSourceVel[2] = velZ;
		alSourcei(audioSource, AL_BUFFER, wave.GetBuffer());
		alSourcef(audioSource, AL_PITCH, 1.0f);
		alSourcef(audioSource, AL_GAIN, 1.0f);
		alSourcefv(audioSource, AL_POSITION, audioSourcePos);
		alSourcefv(audioSource, AL_VELOCITY, audioSourceVel);
		alSourcei(audioSource, AL_LOOPING, static_cast<ALboolean>(loopState));
		alSourcePlay(audioSource);
	}

	void AudioPlayer::Stop()
	{
		alSourceStop(audioSource);
	}


}

#endif