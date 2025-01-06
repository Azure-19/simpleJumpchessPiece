#include "egpch.h"
#include "Layer.h"

namespace Engine {

	Layer::Layer(const std::string& debugName, bool state)
		: m_DebugName(debugName), m_Enabled(state)
	{
	}

	Layer::~Layer()
	{
	}

}