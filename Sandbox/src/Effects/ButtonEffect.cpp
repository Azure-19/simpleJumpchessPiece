#include "ButtonEffect.h"

#include "Entities/SettingController.h"

void ButtonEffect::SetInputProps(InputProps inputProps)
{
	s_InputProps.KeyShakeFreeFlag = inputProps.KeyShakeFreeFlag;
	s_InputProps.MouseShakeFreeFlag = inputProps.MouseShakeFreeFlag;
	s_InputProps.XboxShakeFreeFlag = inputProps.XboxShakeFreeFlag;
}

void ButtonEffect::UpdateButtonListD1(Engine::Ref<Engine::Button>* buttonList, uint32_t length, int32_t& buttonChoice)
{
	if (buttonChoice > -1 ? !buttonList[buttonChoice]->GetKeyChosenState() : false)
		buttonChoice = -1;
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::Up))
	{
		if (s_InputProps.KeyShakeFreeFlag)
		{
			if (buttonChoice > 0)
				buttonChoice--;
			else
				buttonChoice = length - 1;
			if (buttonChoice > -1) // set chosen state
			{
				for (int i = 0; i < length; i++)
					buttonList[i]->SetKeyChosenState(false);
				buttonList[buttonChoice]->SetKeyChosenState(true);
				buttonList[buttonChoice]->SetKeyFlipState(true);
			}
			s_InputProps.KeyShakeFreeFlag = false;
			//EG_TRACE("buttonChoice: {0}.", buttonChoice);
		}
	}
	else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Up))
	{
		if (Engine::Input::IsKeyPressed(Engine::KeyCode::Down))
		{
			if (s_InputProps.KeyShakeFreeFlag)
			{
				if (buttonChoice < length - 1)
					buttonChoice++;
				else
					buttonChoice = 0;
				if (buttonChoice > -1) // setting chosen state
				{
					for (int i = 0; i < length; i++)
						buttonList[i]->SetKeyChosenState(false);
					buttonList[buttonChoice]->SetKeyChosenState(true);
					buttonList[buttonChoice]->SetKeyFlipState(true);
				}
				s_InputProps.KeyShakeFreeFlag = false;
				//EG_TRACE("buttonChoice: {0}.", buttonChoice);
			}
		}
		else if (Engine::Input::IsKeyReleased(Engine::KeyCode::Down))
			s_InputProps.KeyShakeFreeFlag = true;
	}
	// Xbox here, maybe LS included(carefully consider)
}

void ButtonEffect::UpdateButton(Engine::Button& button, Engine::Ref<Engine::Texture2D>& texture, int32_t buttonIndex, int32_t buttonChoice, const std::string& audioSource, EffectProps props)
{
	if (buttonIndex == buttonChoice && button.GetKeyFlipState())
	{
		Engine::AudioPlayer2D::PlaySource(audioSource,
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
		button.SetKeyFlipState(false);
		EG_TRACE("Sound effect played. Index: {0}", buttonIndex);
	}
	else if (button.IsMouseChosen() && button.GetMouseFlipState())
		Engine::AudioPlayer2D::PlaySource(audioSource,
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
	if (buttonIndex != buttonChoice)
		button.SetKeyFlipState(false);
	UpdateButton(button, texture, buttonIndex, buttonChoice, props);
}

void ButtonEffect::UpdateBoardPos(Engine::Button& button, Engine::Ref<Engine::Texture2D>& texture, glm::ivec2& buttonIndex, glm::ivec2& buttonChoice, const std::string& audioSource, EffectProps props)
{
	if (buttonIndex == buttonChoice && button.GetKeyFlipState())
	{
		Engine::AudioPlayer2D::PlaySource(audioSource,
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
		button.SetKeyFlipState(false);
	}
	if (buttonIndex != buttonChoice)
		button.SetKeyFlipState(false);

	if (button.IsMouseChosen() || buttonIndex == buttonChoice)
	{
		Engine::Renderer2D::DrawButton(button, texture);
	}
	else
		button.SetKeyFlipState(false);
}

void ButtonEffect::UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& subtexture, const std::string& audioSource, EffectProps props)
{
	if (button.IsMouseChosen() && button.GetMouseFlipState())
		Engine::AudioPlayer2D::PlaySource(audioSource,
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());

	if (button.IsMouseChosen())
	{
		if(props.Scale)
			Engine::Renderer2D::DrawButton(button, subtexture, 1.0f, glm::vec4(1.0f), props.ScaleRate);
		else
			Engine::Renderer2D::DrawButton(button, subtexture);
	}
	else
		Engine::Renderer2D::DrawButton(button, subtexture);
}

#if TEST_ENABLE || 1
void ButtonEffect::UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, int32_t buttonIndex, int32_t buttonChoice, EffectProps props)
{
	if (button.IsMouseChosen() || buttonIndex == buttonChoice)
	{
		if (props.Scale)
			Engine::Renderer2D::DrawButton(button, chosenTex, 1.0f, glm::vec4(1.0f), props.ScaleRate);
		else
			Engine::Renderer2D::DrawButton(button, chosenTex);
	}
	else
	{
		Engine::Renderer2D::DrawButton(button, normalTex);
		button.SetKeyFlipState(false);
	}
}

void ButtonEffect::UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, const std::string& audioSource, int32_t buttonIndex, int32_t buttonChoice, EffectProps props)
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
	if (buttonIndex != buttonChoice)
		button.SetKeyFlipState(false);
	UpdateButton(button, chosenTex, normalTex, buttonIndex, buttonChoice, props);
}
#endif

// used by chesses
void ButtonEffect::UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, PickProp pickProp, EffectProps props)
{
	if (pickProp.IsPicked)
	{
		Engine::Renderer2D::DrawButton(button, chosenTex);
	}
	else if (button.IsMouseChosen())
	{
		if(props.Scale)
			Engine::Renderer2D::DrawButton(button, normalTex, 1.0f, glm::vec4(1.0f), props.ScaleRate);
		else
			Engine::Renderer2D::DrawButton(button, normalTex, 1.0f, fadeTintColor);
	}
	else
		Engine::Renderer2D::DrawButton(button, normalTex);
}
// used by chesses
void ButtonEffect::UpdateButton(Engine::Button& button, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, const std::string& audioSource, PickProp pickProp, EffectProps props)
{
	if (button.IsMouseChosen() && button.GetMouseFlipState())
		Engine::AudioPlayer2D::PlaySource(audioSource, 
			GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());

	if (button.IsMouseChosen() && Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonLeft))
	{
		if (s_InputProps.MouseShakeFreeFlag)
		{
			Engine::AudioPlayer2D::PlaySource(audioSource,
				GameSetting::GetGlobalGain() * GameSetting::GetGlobalEffectGain());
			s_InputProps.MouseShakeFreeFlag = false;
		}
	}
	if (Engine::Input::IsMouseButtonReleased(Engine::MouseCode::ButtonLeft))
		s_InputProps.MouseShakeFreeFlag = true;

	UpdateButton(button, chosenTex, normalTex, pickProp, props);
}

void ButtonEffect::UpdateChoseButton(Engine::ChoseButton& choseButton, Engine::Ref<Engine::SubTexture2D>& chosenTex, Engine::Ref<Engine::SubTexture2D>& normalTex, const std::string& audioSource)
{
	UpdateButton(choseButton, choseButton.GetChoice() ? chosenTex : normalTex, audioSource);
}




