#include "egpch.h"
#include "AudioBuffer.h"

#include "Engine/Audio/AudioPlayer.h"

#include "Platform/OpenAL/OpenALAudioBuffer.h"

namespace Engine {

	Ref<AudioBuffer> Engine::AudioBuffer::Create()
	{
		switch (AudioPlayer::GetAPI())
		{
		case AudioAPI::API::None:		EG_CORE_ASSERT(false, "AudioPlayerAPI::None isn't supported yet!"); return nullptr;
		case AudioAPI::API::OpenAL:		return CreateRef<OpenALAudioBuffer>();
		}

		EG_CORE_ASSERT(false, "Unknown AudioPlayerAPI!");
		return nullptr;
	}

	BufferList::~BufferList()
	{
		for (const auto& kv : m_AudioBuffers)
		{
			if(kv.second)
			kv.second->~AudioBuffer();
		}
	}

	void BufferList::Add(const std::string& name, const Ref<AudioBuffer>& buffer)
	{
		EG_CORE_ASSERT(!Exists(name), "AudioBuffer [{0}] have already existed!", name);
		m_AudioBuffers[name] = buffer;
	}

	Ref<AudioBuffer> BufferList::Get(const std::string& name)
	{
		EG_CORE_ASSERT(Exists(name), "AudioSource not found!");
		return m_AudioBuffers[name];
	}
	bool BufferList::Exists(const std::string& name) const
	{
		return m_AudioBuffers.find(name) != m_AudioBuffers.end();
	}

}