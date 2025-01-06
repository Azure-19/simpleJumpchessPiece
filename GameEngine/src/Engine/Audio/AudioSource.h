#pragma once

#include <glm/glm.hpp>

namespace Engine {

	class AudioSource
	{
	public:
		virtual ~AudioSource() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetMuteState(bool muteState) = 0;
		virtual void SetGain(float gain) = 0;
		virtual void SetPitch(float pitch) = 0;
		virtual void SetLoopState(bool loopstate) = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual void SetVelocity(const glm::vec3& velocity) = 0;

		virtual const void SetBuffer(uint32_t bufferID) = 0;
		virtual const uint32_t GetBuffer() const = 0;
		virtual const uint32_t GetSourceID() const = 0;

		static Ref<AudioSource> Create();
	};
	// not useful for single source
	class SourceList
	{
	private:
		std::unordered_map<std::string, Ref<AudioSource>> m_AudioSources;
	public:
		~SourceList();
		void Add(const std::string& name, const Ref<AudioSource>& source);
		// this reload will be unsupported soon
		void Add(const Ref<AudioSource>& source);

		Ref<AudioSource> Get(const std::string& name);

		bool Exists(const std::string& name) const;

		std::unordered_map<std::string, Ref<AudioSource>>::iterator begin() { return m_AudioSources.begin(); }
		std::unordered_map<std::string, Ref<AudioSource>>::iterator end() { return m_AudioSources.end(); }

	};

}