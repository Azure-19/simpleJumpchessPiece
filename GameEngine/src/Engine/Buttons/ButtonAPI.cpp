#include "egpch.h"
#include "ButtonAPI.h"

#include "Engine/Core/Input.h"

#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"


#include <fstream>


namespace Engine {

    bool LayerButton::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (this->IsMousePressed())
        {
            if(m_State)
            {
                ShiftLayer();
                return true;
            }
            else
                ;// for overlay/popWindows
        }
        return false;
    }

    bool LayerButton::OnMouseMoved(MouseMovedEvent& e)
    {
        if (!m_MouseState.Chosen)
            m_KeyState.Reset();
        return false;
    }

    bool LayerButton::OnKeyPressed(KeyPressedEvent& e)
    {
        if (Input::IsKeyPressed(KeyCode::Z) && m_KeyState.Chosen)
        {
            if (m_State)
            {
                Application::Get().ShiftLayer(m_LayerIndex[0], m_LayerIndex[1]);
                m_KeyState.Reset();
                return true;
            }
            else
                ;// for overlay/popWindows
        }
        if (Input::IsKeyPressed(KeyCode::X) && m_KeyState.Chosen)
        {
            m_KeyState.Chosen = false;
            return true;
        }
        return false;
    }

    LayerButton::LayerButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, bool state, bool enable)
        :Button(leftLowerCoord, rightHigherCoord, enable),m_State(state)
    {
    }

    LayerButton::~LayerButton()
    {
    }

    void LayerButton::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(LayerButton::OnMouseButtonPressed));
        dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(LayerButton::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(EG_BIND_EVENT_FN(LayerButton::OnMouseMoved));
    }

    void LayerButton::OnUpdate(Timestep ts)
    {
        if (Input::IsGamepadButtonPressed(XboxCode::ButtonA) && m_KeyState.Chosen)
        {
            if (m_GamepadState.ShakeFreeFlag)
            {
                if (m_State)
                {
                    Application::Get().ShiftLayer(m_LayerIndex[0], m_LayerIndex[1]);
                    m_GamepadState.ShakeFreeFlag = false;
                    m_KeyState.Reset();
                }
                else
                    ;// for overlay/popWindows
            }
        }
        else if (Input::IsGamepadButtonReleased(XboxCode::ButtonA))
        {

            if (Input::IsGamepadButtonPressed(XboxCode::ButtonB) && m_KeyState.Chosen)
            {
                m_KeyState.Chosen = false;
            }
            else if (Input::IsGamepadButtonReleased(XboxCode::ButtonB))
                m_GamepadState.ShakeFreeFlag = true;
        }
    }


//-------------------------------------    Switch Button    --------------------------------------------

    // -TODO: Prevent HARD encoding
    constexpr float optionRadius = 25;
    constexpr float texOffset = 25;
    constexpr float timeBase = 0.01667f;
    constexpr float slowDownRate = 0.4f; // smaller -> slower
    constexpr float myTrigger = 0.1f;

    // old school. useful when do not want to update it per frame
    bool SwitchButton::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (this->IsMousePressed())
        {
            // to choose index
            float stride = (m_ScreenCoord[1].x - m_ScreenCoord[0].x) / m_Capacity;
                for (uint32_t i = 0; i < m_Capacity; i++)
                {
                    if ((Input::GetMouseX() > m_ScreenCoord[0].x + stride * i) && (Input::GetMouseX() < m_ScreenCoord[0].x + stride * (i + 1)))
                    {
                        m_Index = i;
                        return true;
                    }
                }
        }
        return false;
    }

    bool SwitchButton::OnKeyPressed(KeyPressedEvent& e)
    {
        if (m_KeyState.Pick && (Input::IsKeyPressed(KeyCode::X) || Input::IsKeyPressed(KeyCode::Z)))
        {
            m_KeyState.Pick = false; // press z or x means you finish change
            return true;
        }
        else if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::X))
        {
            m_KeyState.Chosen = false; // quit chosen state
            return true;
        }
        if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::Z))
        {
            m_KeyState.Pick = true;
            m_KeyState.Flip = true;
            return true;
        }
        if(m_KeyState.Pick)
        {
            if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::Left) && m_Index > 0)
            {
                m_Index--;
                return true;
            }
            if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::Right) && m_Index < m_Capacity - 1)
            {
                m_Index++;
                return true;
            }
        }
        return false;
    }

    std::array<glm::vec2, 2> SwitchButton::GetOptionCoord() const
    {
        if (m_Capacity && !m_OptionRadius)
        {
            glm::vec2 CentreCoord = { m_ScreenCoord[0].x + m_Index * (m_ScreenCoord[1].x - m_ScreenCoord[0].x) / m_Capacity + texOffset, (m_ScreenCoord[0].y + m_ScreenCoord[1].y) / 2.0f };
            glm::vec2 leftLowerCoord = { CentreCoord.x - optionRadius, CentreCoord.y + optionRadius };
            glm::vec2 rightHigherCoord = { CentreCoord.x + optionRadius, CentreCoord.y - optionRadius };
            return { leftLowerCoord, rightHigherCoord };
        }
        else if (m_Capacity && m_OptionRadius)
        {
            glm::vec2 CentreCoord = { m_ScreenCoord[0].x + m_Index * (m_ScreenCoord[1].x - m_ScreenCoord[0].x) / m_Capacity + m_OptionRadius, (m_ScreenCoord[0].y + m_ScreenCoord[1].y) / 2.0f };
            glm::vec2 leftLowerCoord = { CentreCoord.x - m_OptionRadius, CentreCoord.y + m_OptionRadius };
            glm::vec2 rightHigherCoord = { CentreCoord.x + m_OptionRadius, CentreCoord.y - m_OptionRadius };
            return { leftLowerCoord, rightHigherCoord };
        }
        EG_ASSERT(false, "0 capacity is invalid!");
    }

    SwitchButton::SwitchButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, unsigned int capacity, unsigned int defaultIndex, bool enable)
        :Button(leftLowerCoord, rightHigherCoord, enable), m_Capacity(capacity), m_Index(defaultIndex)
    {
    }

    SwitchButton::~SwitchButton()
    {
    }

    void SwitchButton::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(SwitchButton::OnMouseButtonPressed));
        dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(SwitchButton::OnKeyPressed));

    }
    // for drag
    void SwitchButton::OnUpdate(Timestep ts)
    {
        if (this->IsMousePressed())
        {
            float stride = (m_ScreenCoord[1].x - m_ScreenCoord[0].x) / m_Capacity;
            for (uint32_t i = 0; i < m_Capacity; i++)
            {
                if ((Input::GetMouseX() > m_ScreenCoord[0].x + stride * i) && (Input::GetMouseX() < m_ScreenCoord[0].x + stride * (i + 1)))
                {
                    m_Index = i;
                }
            }
        }


        if (m_KeyState.Pick && (Input::IsGamepadButtonPressed(XboxCode::ButtonA)) || Input::IsGamepadButtonPressed(XboxCode::ButtonB))
        {
            if (m_GamepadState.ShakeFreeFlag)
            {
                m_KeyState.Pick = false; // press A or B means you finish change
                m_GamepadState.ShakeFreeFlag = false;
            }
        }
        else if (m_KeyState.Chosen && Input::IsGamepadButtonPressed(XboxCode::ButtonA))
        {
            if (m_GamepadState.ShakeFreeFlag)
            {
                m_KeyState.Pick = true;
                m_KeyState.Flip = true;
                m_GamepadState.ShakeFreeFlag = false;
            }
        }
        else if (Input::IsGamepadButtonReleased(XboxCode::ButtonA))
        {
            if(m_KeyState.Chosen && Input::IsGamepadButtonPressed(XboxCode::ButtonB))
            {
                if (m_GamepadState.ShakeFreeFlag)
                {
                    m_KeyState.Pick = false;
                    m_KeyState.Flip = true;
                    m_GamepadState.ShakeFreeFlag = false;
                }
            }
            else if(Input::IsGamepadButtonReleased(XboxCode::ButtonB))
                m_GamepadState.ShakeFreeFlag = true;
        }
        if(m_Capacity == 5)
            EG_TRACE("{0}", m_IndexCopy);
        // change index needn't set shake free, Use LS to control speed
        if (m_KeyState.Pick)
        {
            if (Input::IsGamepadButtonPressed(XboxCode::ButtonLeft) && m_Index > 0)
            {
                m_Index--;
                m_IndexCopy = m_Index;
            }
            else if (Input::IsGamepadButtonPressed(XboxCode::ButtonRight) && m_Index < m_Capacity - 1)
            {
                m_Index++;
                m_IndexCopy = m_Index;
            }
            if (Input::GetGamepadLSPosition().first < -myTrigger && Input::GetGamepadLSPosition().first != -10)
            {
                if(m_IndexCopy - abs(Input::GetGamepadLSPosition().first) * slowDownRate * ts / timeBase >= 0)
                m_IndexCopy -= abs(Input::GetGamepadLSPosition().first) * slowDownRate * ts / timeBase;
                if (m_Index - m_IndexCopy > 0.7)
                    m_Index = round(m_IndexCopy);
            }
            else if (Input::GetGamepadLSPosition().first > myTrigger)
            {
                if (m_IndexCopy + abs(Input::GetGamepadLSPosition().first) * slowDownRate * ts / timeBase <= m_Capacity - 1)
                    m_IndexCopy += Input::GetGamepadLSPosition().first * slowDownRate * ts / timeBase;
                if (m_IndexCopy - m_Index > 0.7)
                    m_Index = round(m_IndexCopy);
            }
        }
    }


//-------------------------------------    File Button    --------------------------------------------

    void FileButton::WriteFile(std::string& content)
    {
        EG_PROFILE_FUNCTION();

        std::ofstream out(m_Filepath, std::ios::out | std::ios::binary);
        if (out)
        {
            out.write(&content[0], content.size());
            if (!out)
            {
                EG_CORE_ERROR("Could not write file '{0}'", m_Filepath);
            }
            out.close();
        }
        else
        {
            EG_CORE_ERROR("Could not open file '{0}'", m_Filepath);
        }
    }

    std::string FileButton::ReadFile()
    {
        EG_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(m_Filepath, std::ios::in | std::ios::binary);	// read by binary originally, ',' and '|' both okay
        if (in)
        {
            in.seekg(0, std::ios::end);									// start from index 0 to end
            result.resize(in.tellg());									// adapt to file's size
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            EG_CORE_ERROR("Could not open file '{0}'", m_Filepath);
        }
        return result;
    }
    // useless for now
    bool FileButton::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        EG_CORE_ASSERT(!m_Filepath.empty(), "Filepath should be init first!");
        return false;
    }

    bool FileButton::OnKeyPressed(KeyPressedEvent& e)
    {
        if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::X))
        {
            m_KeyState.Chosen = false;
            return true;
        }
        if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::Z))
        {
            m_Trigger = !m_Trigger;
            return true;
        }
        return false;
    }

    FileButton::FileButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, const std::string& filepath, bool writeState, bool enable)
        : Button(leftLowerCoord, rightHigherCoord, enable), m_Filepath(filepath), m_WriteState(writeState)
    {
    }

    void FileButton::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(FileButton::OnKeyPressed));
    }

    void FileButton::OnUpdate(Timestep ts)
    {
        if (Input::IsGamepadButtonPressed(XboxCode::ButtonA) && m_KeyState.Chosen)
        {
            if (m_GamepadState.ShakeFreeFlag)
            {
                m_Trigger = !m_Trigger;
                m_GamepadState.ShakeFreeFlag = false;
            }
        }
        else if (Input::IsGamepadButtonReleased(XboxCode::ButtonA))
        {

            if (Input::IsGamepadButtonPressed(XboxCode::ButtonB) && m_KeyState.Chosen)
            {
                m_KeyState.Chosen = false;
            }
            else if (Input::IsGamepadButtonReleased(XboxCode::ButtonB))
                m_GamepadState.ShakeFreeFlag = true;
        }
    }

//-------------------------------   Chose Button    -------------------------------------

    bool ChoseButton::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if(this->IsMousePressed())
        {
            m_Choice = !m_Choice;
            return true;
        }
        return false;
    }

    bool ChoseButton::OnKeyPressed(KeyPressedEvent& e)
    {
        if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::Z))
        {
            m_Choice = !m_Choice;
            return true;
        }
        if (m_KeyState.Chosen && Input::IsKeyPressed(KeyCode::X))
        {
            m_KeyState.Chosen = false;
            return true;
        }
        return false;
    }

    ChoseButton::ChoseButton(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, bool choice, bool enable)
        : Button(leftLowerCoord, rightHigherCoord, enable), m_Choice(choice)
    {
    }

    void ChoseButton::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(ChoseButton::OnMouseButtonPressed));
        dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(ChoseButton::OnKeyPressed));
    }

    void ChoseButton::OnUpdate(Timestep ts)
    {
        if (Input::IsGamepadButtonPressed(XboxCode::ButtonA) && m_KeyState.Chosen)
        {
            if (m_GamepadState.ShakeFreeFlag)
            {
                m_Choice = !m_Choice;
                m_GamepadState.ShakeFreeFlag = false;
            }
        }
        else if (Input::IsGamepadButtonReleased(XboxCode::ButtonA))
        {

            if (Input::IsGamepadButtonPressed(XboxCode::ButtonB) && m_KeyState.Chosen)
            {
                m_KeyState.Chosen = false;
            }
            else if (Input::IsGamepadButtonReleased(XboxCode::ButtonB))
                m_GamepadState.ShakeFreeFlag = true;
        }
    }

}
