#include "egpch.h"
#if PRIMITIVE_EXAMPLE

#include "AudioLoader.h"

namespace Engine {


	int WaveAudio::GetFileCursorMark(std::ifstream& ifstream, const std::string& mark)
	{
		int length = mark.length();
		char* data = new char[length + 1];
		
		if (data)
			data[length] = '\0';
		else
			EG_CORE_ASSERT(false, "failed to malloc memory!");

		int i = 0;
		while (!ifstream.eof())
		{
			ifstream.seekg(i++, std::ios::beg);
			ifstream.read(data, sizeof(char) * length);
			if (mark.compare(data) == 0)
			{
				delete[] data;
				return i;
			}
		}
		EG_CORE_ASSERT(false, "failed to locate mark");
	}

	WaveAudio::WaveAudio()
	{
	}

	WaveAudio::WaveAudio(const std::string& filepath)
	{
		LoadWave(filepath);
	}

	WaveAudio::~WaveAudio()
	{
		alDeleteBuffers(1, &m_buffer);
	}

	void WaveAudio::LoadWave(const std::string& filepath)
	{
		int mark;
		std::ifstream fileStream(filepath, std::ios::in | std::ios::binary);
		if (!fileStream)
			EG_CORE_ASSERT(false, "Failed to open file: {0}", filepath);

		mark = GetFileCursorMark(fileStream, "RIFF") - 1;
		fileStream.seekg(mark + 4, std::ios::beg);
		fileStream.read((char*)&chunkSize, 4);
		fileStream.seekg(mark + 8, std::ios::beg);
		fileStream.read((char*)&format, 4);

		if (std::string(format).compare("WAVE") != 0)
			EG_CORE_ASSERT(false, "{0} is not a .wav file!", filepath);

		mark = GetFileCursorMark(fileStream, "fmt") - 1;
		fileStream.seekg(mark + 4, std::ios::beg);
		fileStream.read((char*)&formatChunkSize, 4);
		fileStream.seekg(mark + 8, std::ios::beg);
		fileStream.read((char*)&audioFormat, 2);
		fileStream.seekg(mark + 10, std::ios::beg);
		fileStream.read((char*)&numChannels, 2);
		fileStream.seekg(mark + 12, std::ios::beg);
		fileStream.read((char*)&sampleRate, 4);
		fileStream.seekg(mark + 16, std::ios::beg);
		fileStream.read((char*)&byteRate, 4);
		fileStream.seekg(mark + 20, std::ios::beg);
		fileStream.read((char*)&blockAlign, 2);
		fileStream.seekg(mark + 22, std::ios::beg);
		fileStream.read((char*)&bitsPerSample, 2);
		fileStream.seekg(mark + 24, std::ios::beg);

		mark = GetFileCursorMark(fileStream, "data") - 1;
		fileStream.seekg(mark + 4, std::ios::beg);
		fileStream.read((char*)&dataChunkSize, 4) ;
		fileStream.seekg(mark + 8, std::ios::beg);
		data = new unsigned char[dataChunkSize];
		fileStream.read((char*)data, dataChunkSize);

		// load data to OpenAL buffer
		alGenBuffers(1, &m_buffer);
		if (bitsPerSample == 16)
		{
			if (numChannels == 1)
				alBufferData(m_buffer, AL_FORMAT_MONO16, data, dataChunkSize, sampleRate);
			else if (numChannels == 2)
				alBufferData(m_buffer, AL_FORMAT_STEREO16, data, dataChunkSize, sampleRate);
			else
				EG_CORE_ASSERT(false, "Engine do not support more than 2 channels!");
		}
		else if (bitsPerSample == 8)
		{
			if (numChannels == 1)
				alBufferData(m_buffer, AL_FORMAT_MONO8, data, dataChunkSize, sampleRate);
			else if (numChannels == 2)
				alBufferData(m_buffer, AL_FORMAT_STEREO8, data, dataChunkSize, sampleRate);
			else
				EG_CORE_ASSERT(false, "Engine do not support more than 2 channels!");
		}
		else
			;

		// release data
		delete[] data;

		fileStream.close();
		m_Loaded = true;
	}

	uint32_t WaveAudio::GetBuffer()
	{
		if (m_Loaded)
			return m_buffer;
		else
			EG_CORE_ASSERT(false, "Buffer haven't loaded yet!");
	}

}

#endif