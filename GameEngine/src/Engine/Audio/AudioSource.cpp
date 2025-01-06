#include "egpch.h"
#include "AudioSource.h"

#include "Engine/Audio/AudioPlayer.h"
#include "Platform/OpenAL/OpenALSource.h"

namespace Engine {

	Ref<AudioSource> AudioSource::Create()
	{
		switch (AudioPlayer::GetAPI())
		{
		case AudioAPI::API::None:		EG_CORE_ASSERT(false, "RendererAPI::None isn't supported yet!"); return nullptr;
		case AudioAPI::API::OpenAL:		return CreateRef<OpenALSource>();
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	SourceList::~SourceList()
	{
		for (const auto& kv : m_AudioSources)
			kv.second->~AudioSource();
	}

	void SourceList::Add(const std::string& name, const Ref<AudioSource>& source)
	{
		EG_CORE_ASSERT(!Exists(name), "AudioSource [{0}] have already existed!", name);
		m_AudioSources[name] = source;
	}

	void SourceList::Add(const Ref<AudioSource>& source)
	{
	}

	Ref<AudioSource> SourceList::Get(const std::string& name)
	{
		EG_CORE_ASSERT(Exists(name), "AudioSource not found!");
		return m_AudioSources[name];
	}

	bool SourceList::Exists(const std::string& name) const
	{
		return m_AudioSources.find(name) != m_AudioSources.end();
	}

}