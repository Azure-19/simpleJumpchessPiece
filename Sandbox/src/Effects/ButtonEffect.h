#pragma once
#include <Engine.h>

constexpr glm::vec4 whiteTintColor(1.0f),
	fadeTintColor = { 1.0f, 1.0f, 1.0f, 0.7f },
	goldTintColor = { 0.9f, 1.0f , 0.2f, 1.0f },
  orangeTintColor = { 1.0f, 0.98f, 0.95f, 1.0f },
     redTintColor = { 1.0f, 0.9f, 0.9f, 1.0f };
constexpr float dragEffectGain = 0.01f;

struct InputProps
{
	bool KeyShakeFreeFlag = true;
	bool MouseShakeFreeFlag = true;
	bool XboxShakeFreeFlag = true;
};

struct EffectProps
{
	bool Scale = true;
	float ScaleRate = 1.2f;
	glm::vec4 TintColor = whiteTintColor;
public:
	EffectProps() = default;
	EffectProps(bool scale = true, float scaleRate = 1.2f, const glm::vec4& tintColor = whiteTintColor)
		:Scale(scale), ScaleRate(scaleRate), TintColor(tintColor) {}
	EffectProps(const glm::vec4& TintColor)
		:TintColor(TintColor){}
};
// for chesses-[temp]-
struct PickProp
{
	bool IsPicked = false;
};

static InputProps s_InputProps;
const static EffectProps s_DefaultProps = EffectProps(true, 1.2f, whiteTintColor); // could be bad somehow but now it's a must

class ButtonEffect
{
public:
	template<size_t N>
	constexpr static void RefreshButtonListD1(std::array<Engine::Ref<Engine::Button>, N>& buttonList, int32_t buttonChoice){ 
		for (auto it : buttonList)
		{
			it->InvalidateState();
		}
		Engine::Input::SetMousePosition(buttonList[buttonChoice]->GetCenterCoord().x, buttonList[buttonChoice]->GetCenterCoord().y);
		Engine::Input::SetCursorHiddenState(true);
		//EG_TRACE("buttonChoice: {0}.", buttonChoice);
		buttonList[buttonChoice]->SetKeyChosenState(true);
		buttonList[buttonChoice]->SetKeyFlipState(true);
	; }
	template<size_t RAW, size_t COLUMN>
	constexpr static void RefreshButtonListD2(std::array<std::array<Engine::Ref<Engine::Button>, RAW>, COLUMN>& buttonList, glm::ivec2& buttonChoice){
		for (int i = 0; i < COLUMN; i++)
			for (int j = 0; j < RAW; j++)
			{
				if (buttonList[i][j])
				{
					buttonList[i][j]->InvalidateState();
				}
			}
		Engine::Input::SetMousePosition(buttonList[buttonChoice.x][buttonChoice.y]->GetCenterCoord().x, buttonList[buttonChoice.x][buttonChoice.y]->GetCenterCoord().y);
		Engine::Input::SetCursorHiddenState(true);
		//EG_TRACE("buttonChoice: ({0}, {1})", buttonChoice.x, buttonChoice.y);
		buttonList[buttonChoice.x][buttonChoice.y]->SetKeyChosenState(true);
		buttonList[buttonChoice.x][buttonChoice.y]->SetKeyFlipState(true);
	}
	template<size_t RAW, size_t COLUMN>
	constexpr static void RefreshButtonListD2(std::array<std::array<Engine::Button*, RAW>, COLUMN>& buttonList, glm::ivec2& buttonChoice)
	{
		for (int i = 0; i < COLUMN; i++)
			for (int j = 0; j < RAW; j++)
			{
				if (buttonList[i][j])
				{
					buttonList[i][j]->InvalidateState();
				}
			}
		Engine::Input::SetMousePosition(buttonList[buttonChoice.x][buttonChoice.y]->GetCenterCoord().x, buttonList[buttonChoice.x][buttonChoice.y]->GetCenterCoord().y);
		Engine::Input::SetCursorHiddenState(true);
		//EG_TRACE("buttonChoice: ({0}, {1})", buttonChoice.x, buttonChoice.y);
		buttonList[buttonChoice.x][buttonChoice.y]->SetKeyChosenState(true);
		buttonList[buttonChoice.x][buttonChoice.y]->SetKeyFlipState(true);
	}

public:
	static void SetInputProps(InputProps inputProps);
	// for single column(not used)
	static void UpdateButtonListD1(Engine::Ref<Engine::Button>* buttonList, uint32_t length, int32_t& buttonChoice);
//------------------------------------------------------------------------------------------------------------------------------------------------------
	// lockList contains buttonIndexes that you don't want to forbid key movement after chosen. 
	// lock after left click(sometimes)/ z/ A  input. quit by right click/ x/ B
	template <size_t N, size_t M>
	static void UpdateButtonListD1(std::array<Engine::Ref<Engine::Button>, N>& buttonList, int32_t& buttonChoice, const std::array<int32_t, M>& lockList);
	// lockList contains buttonIndexes that you don't want to forbid key movement after chosen.
	template <size_t RAW, size_t COLUMN, size_t N>
	static void UpdateButtonListD2(std::array<std::array<Engine::Ref<Engine::Button>, RAW>, COLUMN>& buttonList, glm::ivec2& buttonChoice, const std::array<glm::ivec2, N>& lockList);
	// for chesses and board pos(raw pointers to prevent type problem)
	template <size_t RAW, size_t COLUMN, size_t N>
	static void UpdateButtonListD2(std::array<std::array<Engine::Button*, RAW>, COLUMN>& buttonList, glm::ivec2& buttonChoice, const std::array<glm::ivec2, N>& lockList);

//------------------------------------------------------------------------------------------------------------------------------------------------------
	template <typename Dimension, typename Tex>
	static void UpdateButton(Engine::Button& button, Tex& texture,  
		Dimension buttonIndex, Dimension buttonChoice, const std::string& audioSource, EffectProps props = s_DefaultProps);
	template <typename Dimension, typename Tex>
	static void UpdateButton(Engine::Button& button, Tex& texture,
		Dimension buttonIndex, Dimension buttonChoice, EffectProps props = s_DefaultProps);
	template <typename Dimension>
	static void UpdateChoseButton(Engine::ChoseButton& choseButton, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex,
		Dimension buttonIndex, Dimension buttonChoice, const std::string& audioSource = "", EffectProps props = s_DefaultProps);
	// linear for those capacity huge enough(greater than 40)
	template <typename Dimension>
	static void UpdateLinearSwitchButton(Engine::SwitchButton& button, Engine::Ref<Engine::SubTexture2D>& optionTex, Engine::Ref<Engine::SubTexture2D>& leftTex, Engine::Ref<Engine::SubTexture2D>& rightTex,
		Dimension buttonIndex, Dimension buttonChoice, Engine::Timestep ts, const std::string& audioSource = "", EffectProps props = s_DefaultProps);
	// toggle for those capacity is small(below 10), not support scale
	template <typename Dimension>
	static void UpdateToggleSwitchButton(Engine::SwitchButton& button, const Engine::Ref<Engine::Texture2D>& buttonTex, const Engine::Ref<Engine::Texture2D>& optionTex,
		Dimension buttonIndex, Dimension buttonChoice, const std::string& audioSource = "", EffectProps props = s_DefaultProps);

//------------------------------------------------------------------------------------------------------------------------------------------------------
	// keep for default parameter that template couldn't afford
	static void UpdateButton(Engine::Button& button, Engine::Ref<Engine::Texture2D>& texture,
		int32_t buttonIndex = 0, int32_t buttonChoice = -1, const std::string& audioSource = "", EffectProps props = s_DefaultProps);
	// update chessboardPosition as a button
	static void UpdateBoardPos(Engine::Button& button, Engine::Ref<Engine::Texture2D>& texture,
		glm::ivec2& buttonIndex, glm::ivec2& buttonChoice, const std::string& audioSource = "", EffectProps props = s_DefaultProps);
#if TEST_ENABLE || 1
	static void UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, 
		int32_t buttonIndex = 0, int32_t buttonChoice = -1, EffectProps props = s_DefaultProps);
	static void UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, const std::string& audioSource, 
		int32_t buttonIndex = 0, int32_t buttonChoice = -1, EffectProps props = s_DefaultProps);
#endif
	// no list management below
	static void UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& subtexture, const std::string& audioSource, EffectProps props = s_DefaultProps);
	// chesses
	static void UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, PickProp pickProp, EffectProps props = s_DefaultProps);
	static void UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, const std::string& audioSource, PickProp pickProp, EffectProps props = s_DefaultProps);
	static void UpdateChoseButton(Engine::ChoseButton& choseButton, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, const std::string& audioSource);
};

template<size_t N, size_t M>
inline void ButtonEffect::UpdateButtonListD1(std::array<Engine::Ref<Engine::Button>, N>& buttonList, int32_t& buttonChoice, const std::array<int32_t, M>& lockList)
{
	if (lockList[0] == -1 || !std::any_of(lockList.begin(), lockList.end(), [&](int32_t lockIndex){
		return lockIndex == buttonChoice &&
			buttonList[lockIndex]->GetKeyPickState(); }))
	{
		if (buttonChoice > -1 ? !buttonList[buttonChoice]->GetKeyChosenState()&&
			!buttonList[buttonChoice]->GetKeyPickState() :
			false)
			buttonChoice = -1;
		if (Engine::Input::IsKeyPressed(Engine::KeyCode::Up) ||
			Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonUp) ||
			Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftY, Engine::XboxCode::UP))
		{
			if (s_InputProps.KeyShakeFreeFlag)
			{
				if (buttonChoice > 0)
					buttonChoice--;
				else
					buttonChoice = N - 1;
				if (buttonChoice > -1) // set chosen state
				{
					RefreshButtonListD1(buttonList, buttonChoice);
				}
				s_InputProps.KeyShakeFreeFlag = false;
			}
		}
		else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Up)&&
			Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonUp) &&
			Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftY, Engine::XboxCode::UP))
		{
			if (Engine::Input::IsKeyPressed(Engine::KeyCode::Down) ||
				Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonDown) ||
				Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftY, Engine::XboxCode::DOWN))
			{
				if (s_InputProps.KeyShakeFreeFlag)
				{
					if (buttonChoice < N - 1)
						buttonChoice++;
					else
						buttonChoice = 0;
					if (buttonChoice > -1) // setting chosen state
					{
						RefreshButtonListD1(buttonList, buttonChoice);
					}
					s_InputProps.KeyShakeFreeFlag = false;
				}
			}
			else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Down) &&
				Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonDown) &&
				Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftY, Engine::XboxCode::DOWN))
				s_InputProps.KeyShakeFreeFlag = true;
		}
	}
	// Xbox here, maybe LS included(carefully consider)
}

template<size_t RAW, size_t COLUMN, size_t N>
inline void ButtonEffect::UpdateButtonListD2(std::array<std::array<Engine::Ref<Engine::Button>, RAW>, COLUMN>& buttonList, glm::ivec2& buttonChoice, const std::array<glm::ivec2, N>& lockList)
{
	if (lockList[0] == glm::ivec2(-1) || !std::any_of(lockList.begin(), lockList.end(), [&](const glm::ivec2 lockIndex) {
		return lockIndex == buttonChoice &&
			buttonList[buttonChoice.x][buttonChoice.y]->GetKeyPickState(); }))
	{
		if (buttonChoice.x > -1 && buttonChoice.y > -1 ?
			!buttonList[buttonChoice.x][buttonChoice.y]->GetKeyChosenState() &&
			!buttonList[buttonChoice.x][buttonChoice.y]->GetKeyPickState() :
			false)
				buttonChoice = { -1, -1 };
		if (Engine::Input::IsKeyPressed(Engine::KeyCode::Up) ||
			Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonUp) ||
			Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftY, Engine::XboxCode::UP))
		{
			if (s_InputProps.KeyShakeFreeFlag)
			{
				do {
					if (buttonChoice.y > 0)
					{
						buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						buttonChoice.y--;
					}
					else
					{
						buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						buttonChoice.y = RAW - 1;
					}
				} while (!buttonList[buttonChoice.x][buttonChoice.y]); // skip nullptr

				if (buttonChoice.y > -1 && buttonChoice.x > -1)
				{
					RefreshButtonListD2(buttonList, buttonChoice);
				}
				s_InputProps.KeyShakeFreeFlag = false;
			}
		}
		else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Up) &&
			Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonUp) &&
			Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftY, Engine::XboxCode::UP))
		{
			if (Engine::Input::IsKeyPressed(Engine::KeyCode::Down) ||
				Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonDown) ||
				Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftY, Engine::XboxCode::DOWN))
			{
				if (s_InputProps.KeyShakeFreeFlag)
				{
					do {
						if (buttonChoice.y < RAW - 1)
						{
							buttonChoice.y++;
							buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						}
						else
						{
							buttonChoice.y = 0;
							buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						}
					} while (!buttonList[buttonChoice.x][buttonChoice.y]);

					if (buttonChoice.y > -1 && buttonChoice.x > -1) // setting chosen state
					{
						RefreshButtonListD2(buttonList, buttonChoice);
					}
					s_InputProps.KeyShakeFreeFlag = false;
				}
			}
			else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Down) &&
				Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonDown) &&
				Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftY, Engine::XboxCode::DOWN))
			{
				if (Engine::Input::IsKeyPressed(Engine::KeyCode::Left) ||
					Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonLeft) ||
					Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftX, Engine::XboxCode::LEFT))
				{
					if (s_InputProps.KeyShakeFreeFlag)
					{
						do {
							if (buttonChoice.x > 0)
							{
								buttonChoice.x--;
								buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
							}
							else
							{
								buttonChoice.x = COLUMN - 1;
								buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
							}
						} while (!buttonList[buttonChoice.x][buttonChoice.y]);

						if (buttonChoice.y > -1 && buttonChoice.x > -1)
						{
							RefreshButtonListD2(buttonList, buttonChoice);
						}
						s_InputProps.KeyShakeFreeFlag = false;
					}
				}
				else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Left) &&
					Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonLeft) &&
					Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftX, Engine::XboxCode::LEFT))
				{
					if (Engine::Input::IsKeyPressed(Engine::KeyCode::Right) ||
						Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonRight) ||
						Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftX, Engine::XboxCode::RIGHT))
					{
						if (s_InputProps.KeyShakeFreeFlag)
						{
							do {
								if (buttonChoice.x < COLUMN - 1)
								{
									buttonChoice.x++;
									buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
								}
								else
								{
									buttonChoice.x = 0;
									buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
								}
							} while (!buttonList[buttonChoice.x][buttonChoice.y]);

							if (buttonChoice.y > -1 && buttonChoice.x > -1) // setting chosen state
							{
								RefreshButtonListD2(buttonList, buttonChoice);
							}
							s_InputProps.KeyShakeFreeFlag = false;
						}
					}
					else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Right) &&
						Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonRight) &&
						Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftX, Engine::XboxCode::RIGHT))
						s_InputProps.KeyShakeFreeFlag = true;
				}
			}

		}
	}

}

template<size_t RAW, size_t COLUMN, size_t N>
inline void ButtonEffect::UpdateButtonListD2(std::array<std::array<Engine::Button*, RAW>, COLUMN>& buttonList, glm::ivec2& buttonChoice, const std::array<glm::ivec2, N>& lockList)
{
	if (lockList[0] == glm::ivec2(-1) || !std::any_of(lockList.begin(), lockList.end(), [&](const glm::ivec2 lockIndex) {
		return lockIndex == buttonChoice &&
			buttonList[buttonChoice.x][buttonChoice.y]->GetKeyPickState(); }))
	{
		if (buttonChoice.x > -1 && buttonChoice.y > -1 ?
			!buttonList[buttonChoice.x][buttonChoice.y]->GetKeyChosenState() &&
			!buttonList[buttonChoice.x][buttonChoice.y]->GetKeyPickState() :
			false)
				buttonChoice = { -1, -1 };
		if (Engine::Input::IsKeyPressed(Engine::KeyCode::Up) ||
			Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonUp) ||
			Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftY, Engine::XboxCode::UP))
		{
			if (s_InputProps.KeyShakeFreeFlag)
			{
				do {
					if (buttonChoice.y > 0)
					{
						buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						buttonChoice.y--;
					}
					else
					{
						buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						buttonChoice.y = RAW - 1;
					}
				} while (!buttonList[buttonChoice.x][buttonChoice.y]); // skip nullptr

				if (buttonChoice.y > -1 && buttonChoice.x > -1)
				{
					RefreshButtonListD2(buttonList, buttonChoice);
				}
				s_InputProps.KeyShakeFreeFlag = false;
			}
		}
		else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Up) &&
			Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonUp) &&
			Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftY, Engine::XboxCode::UP))
		{
			if (Engine::Input::IsKeyPressed(Engine::KeyCode::Down) ||
				Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonDown) ||
				Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftY, Engine::XboxCode::DOWN))
			{
				if (s_InputProps.KeyShakeFreeFlag)
				{
					do {
						if (buttonChoice.y < RAW - 1)
						{
							buttonChoice.y++;
							buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						}
						else
						{
							buttonChoice.y = 0;
							buttonChoice.x = buttonChoice.x < 0 ? 0 : buttonChoice.x;
						}
					} while (!buttonList[buttonChoice.x][buttonChoice.y]);

					if (buttonChoice.y > -1 && buttonChoice.x > -1) // setting chosen state
					{
						RefreshButtonListD2(buttonList, buttonChoice);
					}
					s_InputProps.KeyShakeFreeFlag = false;
				}
			}
			else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Down) &&
				Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonDown) &&
				Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftY, Engine::XboxCode::DOWN))
			{
				if (Engine::Input::IsKeyPressed(Engine::KeyCode::Left) ||
					Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonLeft) ||
					Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftX, Engine::XboxCode::LEFT))
				{
					if (s_InputProps.KeyShakeFreeFlag)
					{
						do {
							if (buttonChoice.x > 0)
							{
								buttonChoice.x--;
								buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
							}
							else
							{
								buttonChoice.x = COLUMN - 1;
								buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
							}
						} while (!buttonList[buttonChoice.x][buttonChoice.y]);

						if (buttonChoice.y > -1 && buttonChoice.x > -1)
						{
							RefreshButtonListD2(buttonList, buttonChoice);
						}
						s_InputProps.KeyShakeFreeFlag = false;
					}
				}
				else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Left) &&
					Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonLeft) &&
					Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftX, Engine::XboxCode::LEFT))
				{
					if (Engine::Input::IsKeyPressed(Engine::KeyCode::Right) ||
						Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonRight) ||
						Engine::Input::IsGamepadAxisPulled(Engine::XboxCode::AxisLeftX, Engine::XboxCode::RIGHT))
					{
						if (s_InputProps.KeyShakeFreeFlag)
						{
							do {
								if (buttonChoice.x < COLUMN - 1)
								{
									buttonChoice.x++;
									buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
								}
								else
								{
									buttonChoice.x = 0;
									buttonChoice.y = buttonChoice.y < 0 ? 0 : buttonChoice.y;
								}
							} while (!buttonList[buttonChoice.x][buttonChoice.y]);

							if (buttonChoice.y > -1 && buttonChoice.x > -1) // setting chosen state
							{
								RefreshButtonListD2(buttonList, buttonChoice);
							}
							s_InputProps.KeyShakeFreeFlag = false;
						}
					}
					else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Right) &&
						Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonRight) &&
						Engine::Input::IsGamepadAxisReset(Engine::XboxCode::AxisLeftX, Engine::XboxCode::RIGHT))
							s_InputProps.KeyShakeFreeFlag = true;
				}
			}

		}
	}

}

template<typename Dimension, typename Tex>
inline void ButtonEffect::UpdateButton(Engine::Button& button, Tex& texture, Dimension buttonIndex, Dimension buttonChoice, const std::string& audioSource, EffectProps props)
{
	if (buttonIndex == buttonChoice && button.GetKeyFlipState())
	{
		Engine::AudioPlayer2D::PlaySource(audioSource,
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
		button.SetKeyFlipState(false);
		//EG_TRACE("Sound effect played. Index: {0}", buttonIndex);
	}
	else if (button.IsMouseChosen() && button.GetMouseFlipState())
		Engine::AudioPlayer2D::PlaySource(audioSource,
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
	if (buttonIndex != buttonChoice)
		button.SetKeyFlipState(false);
	UpdateButton(button, texture, buttonIndex, buttonChoice, props);
}

template<typename Dimension, typename Tex>
inline void ButtonEffect::UpdateButton(Engine::Button& button, Tex& texture, Dimension buttonIndex, Dimension buttonChoice, EffectProps props)
{
	if (button.IsMouseChosen() || buttonIndex == buttonChoice)
	{
		if (props.Scale)
			Engine::Renderer2D::DrawButton(button, texture, 1.0f, props.TintColor, props.ScaleRate);
		else
			Engine::Renderer2D::DrawButton(button, texture, 1.0f, props.TintColor);
	}
	else
	{
		Engine::Renderer2D::DrawButton(button, texture, 1.0f, fadeTintColor);
		button.SetKeyFlipState(false);
	}
}

template<typename Dimension>
inline void ButtonEffect::UpdateChoseButton(Engine::ChoseButton& choseButton, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, Dimension buttonIndex, Dimension buttonChoice, const std::string& audioSource, EffectProps props)
{
	UpdateButton(choseButton, choseButton.GetChoice() ? chosenTex : normalTex, buttonIndex, buttonChoice, audioSource, props);
}

template<typename Dimension>
inline void ButtonEffect::UpdateLinearSwitchButton(Engine::SwitchButton& button, Engine::Ref<Engine::SubTexture2D>& optionTex, Engine::Ref<Engine::SubTexture2D>& leftTex, Engine::Ref<Engine::SubTexture2D>& rightTex, Dimension buttonIndex, Dimension buttonChoice, Engine::Timestep ts, const std::string& audioSource, EffectProps props)
{
	if (audioSource != "")
	{
		if (buttonIndex == buttonChoice && button.GetKeyFlipState())
		{
			Engine::AudioPlayer2D::PlaySource(audioSource,
				GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
			button.SetKeyFlipState(false);
		}
		else if (button.IsMouseChosen() && button.GetMouseFlipState())
			Engine::AudioPlayer2D::PlaySource(audioSource,
				GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
	}

	if (buttonIndex != buttonChoice)
		button.SetKeyFlipState(false);

	if (button.IsMouseChosen() || (buttonIndex == buttonChoice && button.GetKeyPickState()))
		Engine::Renderer2D::DrawSwitchButton(button, optionTex, leftTex, rightTex, 1.0f, props.TintColor);
	else if (buttonIndex == buttonChoice)
		Engine::Renderer2D::DrawSwitchButton(button, optionTex, leftTex, rightTex, 1.0f, props.TintColor * fadeTintColor);
	else
	{
		Engine::Renderer2D::DrawSwitchButton(button, optionTex, leftTex, rightTex, 1.0f, fadeTintColor * fadeTintColor);
		button.SetKeyFlipState(false);
	}

	button.OnUpdate(ts);
}

template<typename Dimension>
inline void ButtonEffect::UpdateToggleSwitchButton(Engine::SwitchButton& button, const Engine::Ref<Engine::Texture2D>& buttonTex, const Engine::Ref<Engine::Texture2D>& optionTex, Dimension buttonIndex, Dimension buttonChoice, const std::string& audioSource, EffectProps props)
{
	if (audioSource != "")
	{
		if (buttonIndex == buttonChoice && button.GetKeyFlipState())
		{
			Engine::AudioPlayer2D::PlaySource(audioSource,
				GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
			button.SetKeyFlipState(false);
		}
		else if (button.IsMouseChosen() && button.GetMouseFlipState())
			Engine::AudioPlayer2D::PlaySource(audioSource,
				GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
		if (button.IsMousePressed() && audioSource != "" && button.GetMouseFlipState())
			Engine::AudioPlayer2D::PlaySource(audioSource,
				GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
	}
	if (buttonIndex != buttonChoice)
		button.SetKeyFlipState(false);


	if (button.IsMouseChosen() || (buttonIndex == buttonChoice && button.GetKeyPickState()))
		Engine::Renderer2D::DrawSwitchButton(button, buttonTex, optionTex, 1.0f, props.TintColor);
	else if (buttonIndex == buttonChoice)
		Engine::Renderer2D::DrawSwitchButton(button, buttonTex, optionTex, 1.0f, props.TintColor * fadeTintColor);
	else
	{
		Engine::Renderer2D::DrawSwitchButton(button, buttonTex, optionTex, 1.0f, fadeTintColor * fadeTintColor);
		button.SetKeyFlipState(false);
	}
}
