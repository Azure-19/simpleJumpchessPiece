#include "egpch.h"
#include "OpenALAudioBuffer.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Engine {

	int OpenALAudioBuffer::GetFileCursorMark(std::ifstream& ifstream, const std::string& mark)
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

	OpenALAudioBuffer::OpenALAudioBuffer()
	{
		alGenBuffers(1, &m_BufferID);
	}

	void OpenALAudioBuffer::LoadWave(const std::string& filepath)
	{
		// read from file
		int mark;
		std::ifstream fileStream(filepath, std::ios::in | std::ios::binary);
		if (!fileStream)
			EG_CORE_ASSERT(false, "Failed to open file: {0}", filepath);

		mark = GetFileCursorMark(fileStream, "RIFF") - 1;
		fileStream.seekg(mark + 4, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.ChunkSize, 4);
		fileStream.seekg(mark + 8, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.Format, 4);

		if (std::string(m_BufferProps.Format).compare("WAVE") != 0)
			EG_CORE_ASSERT(false, "{0} is not a .wav file!", filepath);

		mark = GetFileCursorMark(fileStream, "fmt") - 1;
		fileStream.seekg(mark + 4, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.FormatChunkSize, 4);
		fileStream.seekg(mark + 8, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.AudioFormat, 2);
		fileStream.seekg(mark + 10, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.NumChannels, 2);
		fileStream.seekg(mark + 12, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.SampleRate, 4);
		fileStream.seekg(mark + 16, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.ByteRate, 4);
		fileStream.seekg(mark + 20, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.BlockAlign, 2);
		fileStream.seekg(mark + 22, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.BitsPerSample, 2);
		fileStream.seekg(mark + 24, std::ios::beg);

		mark = GetFileCursorMark(fileStream, "data") - 1;
		fileStream.seekg(mark + 4, std::ios::beg);
		fileStream.read((char*)&m_BufferProps.DataChunkSize, 4);
		fileStream.seekg(mark + 8, std::ios::beg);
		m_BufferProps.Data = new unsigned char[m_BufferProps.DataChunkSize];
		fileStream.read((char*)m_BufferProps.Data, m_BufferProps.DataChunkSize);

		// load data to OpenAL buffer
		if (m_BufferProps.BitsPerSample == 16)
		{
			if (m_BufferProps.NumChannels == 1)
				alBufferData(m_BufferID, AL_FORMAT_MONO16, m_BufferProps.Data, m_BufferProps.DataChunkSize, m_BufferProps.SampleRate);
			else if (m_BufferProps.NumChannels == 2)
				alBufferData(m_BufferID, AL_FORMAT_STEREO16, m_BufferProps.Data, m_BufferProps.DataChunkSize, m_BufferProps.SampleRate);
			else
				EG_CORE_ASSERT(false, "Engine do not support more than 2 channels!");
		}
		else if (m_BufferProps.BitsPerSample == 8)
		{
			if (m_BufferProps.NumChannels == 1)
				alBufferData(m_BufferID, AL_FORMAT_MONO8, m_BufferProps.Data, m_BufferProps.DataChunkSize, m_BufferProps.SampleRate);
			else if (m_BufferProps.NumChannels == 2)
				alBufferData(m_BufferID, AL_FORMAT_STEREO8, m_BufferProps.Data, m_BufferProps.DataChunkSize, m_BufferProps.SampleRate);
			else
				EG_CORE_ASSERT(false, "Engine do not support more than 2 channels!");
		}
		else
			;

		// release data
		delete[] m_BufferProps.Data;

		fileStream.close();
		m_Loaded = true;
	}


	OpenALAudioBuffer::~OpenALAudioBuffer()
	{
		alDeleteBuffers(1, &m_BufferID);
	}

}