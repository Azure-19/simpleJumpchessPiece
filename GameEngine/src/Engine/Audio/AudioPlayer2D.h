#pragma once

#include <glm/glm.hpp>

namespace Engine {

	typedef enum class SourceState{
		Initial,
		Playing,
		Paused,
		Stopped,
		Notfound,
	}State;
	// refactored and would quite different to renderer2D
	class AudioPlayer2D
	{
	public:
		static void Init();
		static void Shutdown();

		// must add/remove a buffer at the same time
		static void AddSource(const std::string& name, const std::string& filepath);
		static void ResetSourceBuffer(const std::string& name, const std::string& filepath);
		static void RemoveSource(const std::string& name);

		// gain should be positive. play a paused source is continue while a playing source is replay
		static void PlaySource(const std::string& name, float gain = 1.0f, float pitch = 1.0f, bool loopState = false, glm::vec2& position = glm::vec2(0.0f), glm::vec2& velocity = glm::vec2(0.0f));
		static void PauseSource(const std::string& name);
		static void StopSource(const std::string& name);
		// return the state of the source
		static SourceState CheckSource(const std::string& name);
		static void EditSource(const std::string& name, float gain = 1.0f, float pitch = 1.0f, bool loopState = false, glm::vec2& position = glm::vec2(0.0f), glm::vec2& velocity = glm::vec2(0.0f));

		static void SetGlobalAttribute(float globalGain, float effectGain, float musicGain);
		static void Update();

	};

}

