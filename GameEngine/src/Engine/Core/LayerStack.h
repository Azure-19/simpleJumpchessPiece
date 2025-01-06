#pragma once

#include "Engine/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Engine {

	class LayerStack
	{
	private:
		std::vector<Layer*> m_Layers;				 //WHY USE VECTOR: iterator needed every frame and need to reverse iterated
		unsigned int m_LayerInsertIndex = 0;		 //a raw pointer just because it's a system that's easy to handle
		// change iterator to an uint since sometimes
		// when whole cache move it is disabled	

	public:
		LayerStack();
		~LayerStack();

		inline unsigned int GetIndex() { return m_LayerInsertIndex; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);


		void EnableLayer(unsigned int index);
		void DisableLayer(unsigned int index);
		void ShiftLayer(unsigned int onUseIndex, unsigned int target);
		Layer* VisitLayer(unsigned int index) const;			// a dangerous func. -TODO: figure out a way fixing it

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

		inline Layer* operator[](unsigned int index)//a reload that may be friendly for use
		{
			return m_Layers[index];
		}
	};

}