#pragma once

#include "Engine/Audio/AudioSource.h"

namespace Engine {

	class OpenALSource : public AudioSource
	{
	private:
		bool m_MuteState; // set gain 0 actually
		uint32_t m_SourceID;
		uint32_t m_BufferID = 114514; // default value stands for not init
	private:
	public:
		OpenALSource();
		virtual ~OpenALSource();

		virtual void Bind() const override;
		virtual void UnBind() const override; // for debug

		virtual void SetMuteState(bool muteState) override;
		virtual void SetGain(float gain) override;
		virtual void SetPitch(float pitch) override;
		virtual void SetLoopState(bool loopState) override;
		virtual void SetPosition(const glm::vec3& position) override;
		virtual void SetVelocity(const glm::vec3& velocity) override;

		virtual const void SetBuffer(uint32_t bufferID) override { m_BufferID = bufferID; }
		virtual const uint32_t GetBuffer() const override { return m_BufferID; }
		virtual const uint32_t GetSourceID() const override { return m_SourceID; };

	};

}

