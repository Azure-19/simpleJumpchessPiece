#include "egpch.h"
#include "RenderCommand.h"

namespace Engine {

	// would be dynamically set in the future
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}