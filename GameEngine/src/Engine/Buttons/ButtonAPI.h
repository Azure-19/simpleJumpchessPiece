#pragma once

#include "Engine/Buttons/Button.h"

#include "Engine/Core/Application.h"

namespace Engine {

	class LayerButton : public Button
	{
	private:
		std::array<unsigned int, 2> m_LayerIndex = {0};
		// shift a layer or just display a overlay(e.g. PAUSE, not implemented yet)
		bool m_State = true;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	public:
		inline void SetLayer(unsigned int nowLayer, unsigned int targetLayer) { m_LayerIndex[0] = nowLayer; m_LayerIndex[1] = targetLayer; }
		inline void ShiftLayer() { Application::Get().ShiftLayer(m_LayerIndex[0], m_LayerIndex[1]); }

		// true for push
		LayerButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, bool state = true, bool enable = true);
		virtual ~LayerButton();

		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(Timestep ts = Timestep(0.01667f)) override;

	};

	class SwitchButton : public Button
	{
	private:
		// set as up to 4 now
		float m_IndexCopy = 0.0f;
		unsigned int m_Index = 0;
		unsigned int m_Capacity = 0;
		unsigned int m_OptionRadius = 0;
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	public:
		inline void SetIndex(unsigned int index) { m_Index = index; }
		inline unsigned int GetIndex() { return m_Index; }
		// for simplify render func
		std::array<glm::vec2, 2> GetOptionCoord() const;
		inline void SetOptRadius(unsigned int optionRadius) { m_OptionRadius = optionRadius; }

		SwitchButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, unsigned int capacity, unsigned int defaultIndex = 0, bool enable = true);
		virtual ~SwitchButton();

		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(Timestep ts = Timestep(0.01667f)) override;

	};

	// reference: openGLShader
	// *** -TODO: improve it after fileSystem built***
	class FileButton : public Button
	{
	private:
		std::string m_Filepath;
		bool m_WriteState = true; // not used
	private:
		// origin version
		void WriteFile(std::string& content);
		std::string ReadFile();

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	public:
		inline std::string& GetFilepath() { return m_Filepath; }
		inline void SetFilepath(const std::string& filepath) { m_Filepath = filepath; }
		inline void SetWriteState(bool state) { m_WriteState = state; }

		FileButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, const std::string& filepath, bool writeState = true, bool enable = true);
		//virtual ~FileButton();

		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(Timestep ts = Timestep(0.01667f)) override;
	};

	class ChoseButton : public Button
	{
	private:
		bool m_Choice;

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	public:
		inline void SetChoice(bool choice) { m_Choice = choice; }
		inline void InvertChoice() { m_Choice = !m_Choice; }
		inline bool GetChoice() const { return m_Choice; }

		ChoseButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, bool choice = true, bool enable = true);

		virtual void OnEvent(Event& e) override;
		virtual void OnUpdate(Timestep ts) override;
	};
}