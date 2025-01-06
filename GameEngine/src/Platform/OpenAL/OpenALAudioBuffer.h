#pragma once

#include "Engine/Audio/AudioBuffer.h"


namespace Engine {

	struct BufferProps {
		// RIFF chunk
		uint32_t ChunkSize;
		char Format[5] = { '\0' };

		// sub chunk -- format
		uint32_t FormatChunkSize;
		uint16_t AudioFormat;
		uint16_t NumChannels;
		uint32_t SampleRate;
		uint32_t ByteRate;
		uint16_t BlockAlign;
		uint16_t BitsPerSample;

		// sub chunk -- data
		uint32_t DataChunkSize;
		void* Data; // would be clear per call
	};

	class OpenALAudioBuffer : public AudioBuffer
	{
	private:
		uint32_t m_BufferID;
		bool m_Loaded = false;
		BufferProps m_BufferProps;
	private:
		int GetFileCursorMark(std::ifstream& ifstream, const std::string& mark);
	public:
		OpenALAudioBuffer();
		virtual ~OpenALAudioBuffer() override;

		void LoadWave(const std::string& filepath) override;

		virtual uint32_t GetBufferID() const override { return m_BufferID; }
	};

}

