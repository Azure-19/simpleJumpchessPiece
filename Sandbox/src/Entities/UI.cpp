#include "UI.h"

void GameUI::Init()
{
	m_Texture = Engine::Texture2D::Create("assets/textures/UI/GameUI.png");
	m_TintQuad = Engine::Texture2D::Create("assets/textures/Basic/shadedRoundedRectangle.png");
	m_LetterSheet = Engine::Texture2D::Create("assets/textures/Basic/Letters.png");
	// just make it simple
	s_LetterMap[9] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 0 }, { 30, 30 });
	s_LetterMap[8] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 1 }, { 30, 30 });
	s_LetterMap[7] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 2 }, { 30, 30 });
	s_LetterMap[6] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 3 }, { 30, 30 });
	s_LetterMap[5] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 4 }, { 30, 30 });
	s_LetterMap[4] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 5 }, { 30, 30 });
	s_LetterMap[3] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 6 }, { 30, 30 });
	s_LetterMap[2] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 7 }, { 30, 30 });
	s_LetterMap[1] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 8 }, { 30, 30 });
	s_LetterMap[0] = Engine::SubTexture2D::CreateFromCoord(m_LetterSheet, { 6, 9 }, { 30, 30 });
}

void GameUI::Submit(float timeLeft, uint32_t round, uint32_t teamCount, uint32_t activeTeam)
{
	m_Data.TimeLeft = timeLeft;
	m_Data.Round = round;
	m_Data.TeamCount = teamCount;
	m_Data.ActiveTeam = activeTeam;
}

// only support 0~9 for now
void GameUI::OnUpdate(Engine::Timestep ts)
{
#if (UI_PROPS_SET || 1)
	// render all stuffs here: teamColor and number    70 * 30 square could be nice
	constexpr uint32_t quadWidth = 70;
	constexpr uint32_t quadHeight = 30;

	constexpr uint32_t quadOffsetX = 180;
	constexpr uint32_t quadOffsetY = 133;
	constexpr uint32_t quadGap = 42; // for the second quad upward
	// the whole base UI position and size
	constexpr uint32_t boardWidth = 280;
	constexpr uint32_t boardHeight = 180;
	constexpr uint32_t xMinorCoord = 25;
	constexpr uint32_t yMinorCoord = 25;

	constexpr uint32_t timeOffsetX = 220;
	constexpr uint32_t timeOffsetY = 14;
	constexpr uint32_t timeWidth = 30;
	constexpr uint32_t timeHeight = 30;
	constexpr uint32_t timeGap = 20;

	constexpr uint32_t roundOffsetX = 220;
	constexpr uint32_t roundOffsetY = 54;
	constexpr uint32_t roundWidth = 30;
	constexpr uint32_t roundHeight = 30;

	constexpr glm::vec4 black  = { 0.1f, 0.1f, 0.1f, 1.0f };
	constexpr glm::vec4 yellow = { 1.0f, 1.0f, 0.5f, 1.0f };
	constexpr glm::vec4 red    = { 1.0f, 0.4f, 0.3f, 1.0f };
	constexpr glm::vec4 blue   = { 0.4f, 0.6f, 1.0f, 1.0f };
	constexpr glm::vec4 purple = { 0.9f, 0.1f, 1.0f, 1.0f };
	constexpr glm::vec4 green  = { 0.4f, 1.0f, 0.6f, 1.0f };
#endif
	// update Data(since timeLimit is useful in chessboard.cpp so there's no need move it to here

	// base
	Engine::Renderer2D::DrawCoord({ xMinorCoord, yMinorCoord + boardHeight }, 
		{ xMinorCoord + boardWidth , yMinorCoord }, m_Texture );
	// time
	Engine::Renderer2D::DrawCoord({ xMinorCoord + timeOffsetX, yMinorCoord + timeOffsetY + timeHeight },
		{ xMinorCoord + timeOffsetX + timeWidth , yMinorCoord + timeOffsetY }, s_LetterMap[static_cast<int>(m_Data.TimeLeft)],
		1.0f, m_Data.TimeLeft < 3 ? red : glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f) );

	// round
	if (m_Data.Round < 10)
		Engine::Renderer2D::DrawCoord({ xMinorCoord + roundOffsetX, yMinorCoord + roundOffsetY + roundHeight },
		{ xMinorCoord + roundOffsetX + roundWidth , yMinorCoord + roundOffsetY }, s_LetterMap[m_Data.Round]);
	else if (m_Data.Round < 100)
	{
		Engine::Renderer2D::DrawCoord({ xMinorCoord + roundOffsetX - timeGap, yMinorCoord + roundOffsetY + roundHeight },
			{ xMinorCoord + roundOffsetX + roundWidth - timeGap, yMinorCoord + roundOffsetY }, s_LetterMap[m_Data.Round / 10]);
		Engine::Renderer2D::DrawCoord({ xMinorCoord + roundOffsetX, yMinorCoord + roundOffsetY + roundHeight },
			{ xMinorCoord + roundOffsetX + roundWidth , yMinorCoord + roundOffsetY }, s_LetterMap[m_Data.Round % 10]);
	}
	else if (m_Data.Round < 1000 )
	{
		Engine::Renderer2D::DrawCoord({ xMinorCoord + roundOffsetX - timeGap * 2, yMinorCoord + roundOffsetY + roundHeight },
			{ xMinorCoord + roundOffsetX + roundWidth - timeGap * 2, yMinorCoord + roundOffsetY }, s_LetterMap[m_Data.Round / 100]);
		Engine::Renderer2D::DrawCoord({ xMinorCoord + roundOffsetX - timeGap, yMinorCoord + roundOffsetY + roundHeight },
			{ xMinorCoord + roundOffsetX + roundWidth - timeGap, yMinorCoord + roundOffsetY }, s_LetterMap[(m_Data.Round / 10) % 10]);
		Engine::Renderer2D::DrawCoord({ xMinorCoord + roundOffsetX, yMinorCoord + roundOffsetY + roundHeight },
			{ xMinorCoord + roundOffsetX + roundWidth , yMinorCoord + roundOffsetY }, s_LetterMap[m_Data.Round % 10]);
	}

	// to calculate may be better but this straight forward way isn't that bad
	switch (m_Data.TeamCount)
	{
	case 0:
		switch (m_Data.ActiveTeam)
		{
		case 0:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, black);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, yellow);
			break;
		case 1:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, yellow);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, black);
			break;
		default:
			EG_CORE_ASSERT(false, "Active team out of range!");
		}
		break;
	case 1:
		switch (m_Data.ActiveTeam)
		{
		case 0:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, black);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, yellow);
			break;
		case 1:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, yellow);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, red);
			break;
		case 2:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, red);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, black);
			break;
		default:
			EG_CORE_ASSERT(false, "Active team out of range!");
		}
		break;
	case 2:
		switch (m_Data.ActiveTeam)
		{
		case 0:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, black);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, yellow);
			break;
		case 1:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, yellow);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, red);
			break;
		case 2:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, red);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, blue);
			break;
		case 3:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, blue);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, black);
			break;
		default:
			EG_CORE_ASSERT(false, "Active team out of range!");
		}
		break;
	case 3:
		switch (m_Data.ActiveTeam)
		{
		case 0:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, black);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, yellow);
			break;
		case 1:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, yellow);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, red);
			break;
		case 2:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, red);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, blue);
			break;
		case 3:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, blue);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, purple);
			break;
		case 4:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, purple);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, black);
			break;
		default:
			EG_CORE_ASSERT(false, "Active team out of range!");
		}
		break;
	case 4:
		switch (m_Data.ActiveTeam)
		{
		case 0:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, black);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, yellow);
			break;
		case 1:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, yellow);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, red);
			break;
		case 2:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, red);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, blue);
			break;
		case 3:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, blue);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, purple);
			break;
		case 4:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, purple);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, green);
			break;
		case 5:
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight - quadGap },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY - quadGap },
				m_TintQuad, 1.0f, green);
			Engine::Renderer2D::DrawCoord({ xMinorCoord + quadOffsetX ,yMinorCoord + quadOffsetY + quadHeight },
				{ xMinorCoord + quadOffsetX + quadWidth ,yMinorCoord + quadOffsetY },
				m_TintQuad, 1.0f, black);
			break;
		default:
			break;
		}
		break;
	}


}


//-----------------------------------------------------------------------------------------------------------


static InputUI s_InputUI;

bool InputUI::CheckGamepadInputState()
{
	return Engine::Input::IsGamepadInput();
}

void InputUI::Init()
{
	s_InputUI.m_NormalMouseTex = Engine::Texture2D::Create("assets/textures/UI/inputUI_Mouse.png");
	s_InputUI.m_NormalKeyTex = Engine::Texture2D::Create("assets/textures/UI/inputUI_Key.png");
	s_InputUI.m_NormalGamepadTex = Engine::Texture2D::Create("assets/textures/UI/inputUI_Gamepad.png");
	s_InputUI.m_GamingMouseTex = Engine::Texture2D::Create("assets/textures/UI/gameInputUI_Mouse.png");
	s_InputUI.m_GamingKeyTex = Engine::Texture2D::Create("assets/textures/UI/gameInputUI_Key.png");
	s_InputUI.m_GamingGamepadTex = Engine::Texture2D::Create("assets/textures/UI/gameInputUI_Gamepad.png");
}

void InputUI::Submit(bool scene, InputDeviceType inputDeviceType)
{
	s_InputUI.m_Scene = scene;
	if (static_cast<uint32_t>(inputDeviceType))
		s_InputUI.m_DeviceType = inputDeviceType;
	// else .....Deal with unknown type here
}

void InputUI::OnUpdate(Engine::Timestep ts)
{
	if (CheckGamepadInputState())
		s_InputUI.m_DeviceType = InputDeviceType::Gamepad;

	switch (s_InputUI.m_DeviceType)
	{
	case InputDeviceType::Mouse:
		if(s_InputUI.m_Scene)
			Engine::Renderer2D::DrawCoord({ 1060.0f, 260.0f }, { 1260.0f, 20.0f }, s_InputUI.m_GamingMouseTex);
		else
			Engine::Renderer2D::DrawCoord({ 1060.0f, 260.0f }, { 1260.0f, 20.0f }, s_InputUI.m_NormalMouseTex);
		break;
	case InputDeviceType::Keyboard:
		if (s_InputUI.m_Scene)
			Engine::Renderer2D::DrawCoord({ 1060.0f, 260.0f }, { 1260.0f, 20.0f }, s_InputUI.m_GamingKeyTex);
		else
			Engine::Renderer2D::DrawCoord({ 1060.0f, 260.0f }, { 1260.0f, 20.0f }, s_InputUI.m_NormalKeyTex);
		break;
	case InputDeviceType::Gamepad:
		if (s_InputUI.m_Scene)
			Engine::Renderer2D::DrawCoord({ 1060.0f, 260.0f }, { 1260.0f, 20.0f }, s_InputUI.m_GamingGamepadTex);
		else
			Engine::Renderer2D::DrawCoord({ 1060.0f, 260.0f }, { 1260.0f, 20.0f }, s_InputUI.m_NormalGamepadTex);
		break;
	}
}

void InputUI::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategoryMouse))
	{
		s_InputUI.m_DeviceType = InputDeviceType::Mouse;
	}
	else if (e.IsInCategory(Engine::EventCategoryKeyboard))
	{
		s_InputUI.m_DeviceType = InputDeviceType::Keyboard;
	}
}
