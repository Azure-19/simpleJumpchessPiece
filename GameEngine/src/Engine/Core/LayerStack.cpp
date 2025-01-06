#include "egpch.h"
#include "LayerStack.h"

namespace Engine {

	LayerStack::LayerStack()
	{
	}
	//only lose them when destruct so application can distribute layer(owned by application also)
	LayerStack::~LayerStack()
	{
		// once replaced with unique ptr shouldn't double free
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}
	//pushed into the second half of the layerList(we want to render it at the last
	void LayerStack::PushOverlay(Layer* overlay)	
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

	void LayerStack::EnableLayer(unsigned int index)
	{
		m_Layers[index]->SetEnableState(1);
		m_Layers[index]->OnEnable();
	}

	void LayerStack::DisableLayer(unsigned int index)
	{
		m_Layers[index]->SetEnableState(0);
		m_Layers[index]->OnDisable();
	}

	void LayerStack::ShiftLayer(unsigned int onUseIndex,unsigned int targetIndex)
	{
		DisableLayer(onUseIndex);
		EnableLayer(targetIndex);
	}

	Layer* LayerStack::VisitLayer(unsigned int index) const
	{
		return m_Layers[index];
	}

}