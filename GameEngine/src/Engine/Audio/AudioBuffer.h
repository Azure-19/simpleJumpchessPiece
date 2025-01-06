#pragma once

namespace Engine {

	class AudioBuffer
	{
	public:
		virtual ~AudioBuffer() = default;
		
		virtual void LoadWave(const std::string& filepath) = 0;
		virtual uint32_t GetBufferID() const = 0;
		
		static Ref<AudioBuffer> Create();
	};

	class BufferList
	{
	private:
		std::unordered_map<std::string, Ref<AudioBuffer>> m_AudioBuffers;
	public:
		~BufferList();
		void Add(const std::string& name, const Ref<AudioBuffer>& buffer);

		Ref<AudioBuffer> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	};
}

