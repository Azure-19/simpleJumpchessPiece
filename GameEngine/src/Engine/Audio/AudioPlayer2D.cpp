#include "egpch.h"
#include "AudioPlayer2D.h"

#include "Engine/Audio/AudioBuffer.h"
#include "Engine/Audio/AudioSource.h"
#include "Engine/Audio/AudioCommand.h"

namespace Engine {

	struct SourceProps
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		float Gain;
		float Pitch;
	};

	struct AudioPlayer2DData
	{
		Ref<BufferList> BufferList;
		Ref<SourceList> SourceList;
		
		float GlobalGain = 1.0f;
		float EffectGain = 1.0f;
		float MusicGain = 1.0f;
	};

	static AudioPlayer2DData s_Data;

//--------------------------------      Audio core function   -------------------------

	void AudioPlayer2D::Init()
	{
		EG_PROFILE_FUNCTION();
		s_Data.SourceList = CreateRef<SourceList>();
		s_Data.BufferList = CreateRef<BufferList>();
	}

	void AudioPlayer2D::Shutdown()
	{
		s_Data.BufferList->~BufferList();
		s_Data.SourceList->~SourceList();
	}

	void AudioPlayer2D::AddSource(const std::string& name, const std::string& filepath)
	{
		Ref<AudioSource> source = AudioSource::Create();
		if(!s_Data.SourceList->Exists(name))
			s_Data.SourceList->Add(name, source);

		ResetSourceBuffer(name, filepath);
	}

	void AudioPlayer2D::ResetSourceBuffer(const std::string& name, const std::string& filepath)
	{
		Ref<AudioBuffer> buffer = AudioBuffer::Create();
		if(!s_Data.SourceList->Exists(name))
			s_Data.BufferList->Add(name, buffer);

		buffer->LoadWave(filepath);
		s_Data.SourceList->Get(name)->SetBuffer(buffer->GetBufferID());
		s_Data.SourceList->Get(name)->Bind();
	}

	void AudioPlayer2D::RemoveSource(const std::string& name)
	{	// which destruct first doesn't matter
		if (s_Data.BufferList->Exists(name))
		{
			s_Data.BufferList->Get(name)->~AudioBuffer();
			s_Data.SourceList->Get(name)->~AudioSource();
		}
	}

	void AudioPlayer2D::PlaySource(const std::string& name, float gain, float pitch, bool loopState, glm::vec2& position, glm::vec2& velocity)
	{
		Ref<AudioSource>& source = s_Data.SourceList->Get(name);
		source->SetGain(gain);
		source->SetPitch(pitch);
		source->SetLoopState(loopState);
		source->SetPosition({ position.x, position.y, 0.0f });
		source->SetVelocity({ velocity.x, velocity.y, 0.0f });
		AudioCommand::PlaySource(source);

	}

	void AudioPlayer2D::PauseSource(const std::string& name)
	{
		AudioCommand::PauseSource(s_Data.SourceList->Get(name));
	}

	void AudioPlayer2D::StopSource(const std::string& name)
	{
		AudioCommand::StopSource(s_Data.SourceList->Get(name));
	}

	SourceState AudioPlayer2D::CheckSource(const std::string& name)
	{
		return s_Data.SourceList->Get(name) ? static_cast<SourceState>(AudioCommand::CheckSource(s_Data.SourceList->Get(name))) : SourceState::Notfound;
	}

	// gain should be positive
	void AudioPlayer2D::EditSource(const std::string& name, float gain, float pitch, bool loopState, glm::vec2& position, glm::vec2& velocity)
	{
		Ref<AudioSource>& source = s_Data.SourceList->Get(name);
		source->SetGain(gain);
		source->SetPitch(pitch);
		source->SetLoopState(loopState);
		source->SetPosition({ position.x, position.y, 0.0f });
		source->SetVelocity({ velocity.x, velocity.y, 0.0f });
	}

	void AudioPlayer2D::SetGlobalAttribute(float globalGain, float effectGain, float musicGain)
	{
		s_Data.GlobalGain = globalGain;
		s_Data.EffectGain = effectGain;
		s_Data.MusicGain = musicGain;

		Update();
	}

	void AudioPlayer2D::Update()
	{
		for (auto& it = s_Data.SourceList->begin(); it != s_Data.SourceList->end(); ++it)
		{
			if (it->first.find("Effect") != std::string::npos)
				it->second->SetGain(s_Data.GlobalGain * s_Data.EffectGain);
			else if (it->first.find("BGM") != std::string::npos)
				it->second->SetGain(s_Data.GlobalGain * s_Data.MusicGain);
		}
	}

}