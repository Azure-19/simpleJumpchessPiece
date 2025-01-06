#include "egpch.h"
#include "AudioCommand.h"

namespace Engine {

	Scope<AudioAPI> AudioCommand::s_AudioAPI = AudioAPI::Create();

}