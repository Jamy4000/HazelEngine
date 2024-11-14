#include "hzpch.h"
#include "Hazel/Core/LayerStack.h"

namespace Hazel
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);
		layer->OnAttach();
		m_LayersInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::find(m_Layers.begin() + m_LayersInsertIndex, m_Layers.end(), layer);
		if (it != m_Layers.begin() + m_LayersInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		const auto it = std::find(m_Layers.begin() + m_LayersInsertIndex, m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			layer->OnDetach();
		}
	}
}