#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace Engine {

	class Layer  // method implemented in subclasses
	{
	private:
		bool m_Enabled = false;
	protected:
		std::string m_DebugName;		// should't used in a dist so called debugname
	public:
		Layer(const std::string& name = "Layer", bool state = false);
		virtual ~Layer();

		inline void SetEnableState(bool state) { m_Enabled = state; }
		inline bool GetEnableState() const { return m_Enabled; }

		virtual void OnAttach() {}					// layer push into the layerstack
		virtual void OnDetach() {}					// layer detach out the layerstack

		// -TODO: strengthen the two api below
		virtual void OnEnable() {}
		virtual void OnDisable() {}

		virtual void OnUpdate(Timestep ts) {}		// update , called per frame
		virtual void OnImGuiRender() {}				// to render sth
		virtual void OnEvent(Event& event) {}		// receive an layer event

		inline const std::string& GetName() const { return m_DebugName; }

	};

}
