// NOT ON USE NOW
#if PRIMITIVE_EXAMPLE
#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <cstdint>

#include <AL/al.h>
#include <AL/alc.h>

namespace Engine {

	class WaveAudio
	{
	private:
		bool m_Loop = false;
		bool m_Loaded = false;

		uint32_t m_buffer;
		// could be better managed in a struct

		// RIFF chunk
		uint32_t chunkSize;
		char format[5] = { '\0' };

		// sub chunk -- format
		uint32_t formatChunkSize;
		uint16_t audioFormat;
		uint16_t numChannels;
		uint32_t sampleRate;
		uint32_t byteRate;
		uint16_t blockAlign;
		uint16_t bitsPerSample;

		// sub chunk -- data
		uint32_t dataChunkSize;
		void* data;

		int GetFileCursorMark(std::ifstream& ifstream, const std::string& mark);

	public:
		WaveAudio();
		WaveAudio(const std::string& filepath);

		~WaveAudio();

		void LoadWave(const std::string& filepath);
		uint32_t GetBuffer();

	};

}
#endif