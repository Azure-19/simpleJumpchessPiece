#include "Chessboard.h"

//#include <windows.h>			what included?
//#include "Layers/GameLayer.h" ??????????????

#include "Effects/ButtonEffect.h"
#include "AI.h"

// the two coordinates don't share origin
constexpr auto xOffset = 71.8874;
// y direction is different
constexpr auto yOffset = 32;

constexpr auto reversedP_00 = 0.0211226;
constexpr auto reversedP_01 = -0.0121951;
constexpr auto reversedP_11 = 0.0243902;
// by pixel, 20 is by blocks
constexpr auto chessRadius = 20;

constexpr std::array<glm::ivec2, 1> m_LockList = { glm::ivec2(-1) };

void Chessboard::UpdateGamepadEvent(Engine::Timestep ts)
{
	if (m_BoardChoice != glm::ivec2(-1))
		m_BoardList[m_BoardChoice.x][m_BoardChoice.y]->OnUpdate(ts);

	if (Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonA))
	{
		if (m_BoardChoice == glm::ivec2(-1))
			m_BoardChoice = glm::ivec2(12, 16);
		if (m_InputProps.GamepadShakeFreeFlag)
		{
			// m_BoardChoice is the position choosing(in another coordinate)
			std::array<int, 2> boardChoice = { m_BoardChoice.x, 16 - m_BoardChoice.y };

			if (m_ChessChosen == nullptr)
			{
				m_AdjacentFlag = true;
				m_ChessChosen = JudgeTeamRound(m_BoardPos[boardChoice[0]][boardChoice[1]].get()->GetChess());
				if (m_ChessChosen)
					m_InputProps.GamepadShakeFreeFlag = false;
			}
			else
			{
				if (m_ChessChosen == m_BoardPos[boardChoice[0]][boardChoice[1]].get()->GetChess())
				{
					// cancel chose operation here
					if (m_TeamMoved)
						Skip();
					m_ChessChosen = nullptr;
					m_InputProps.GamepadShakeFreeFlag = false;
				}
				// checking movement
				if (IsValidMovement(m_ChessChosen, boardChoice))
				{
					m_TeamMoved = true;
					// checking superSkip
					bool chessSkipValid = IsChessSkipValid(m_ChessChosen, boardChoice);
					m_AdjacentFlag = chessSkipValid ? false : true;
					m_InputProps.GamepadShakeFreeFlag = false;

					// turn into move event
					if (OnChessMoved(m_ChessChosen, boardChoice))
					{
						//EG_INFO("Have moved a Chess from ({0}, {1})", castPosition[0], castPosition[1]);
						if (IsWinning())
						{
							m_Props.Winner = IsWinning();
							Skip();
						}
						if (!chessSkipValid)
							Skip();
					}
				}
			}
		}
	}
	else if (Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonA))
		m_InputProps.GamepadShakeFreeFlag = true;
}

//-----------------------------------------------------------------------------------------------------------
// casting mouseCoord to boardPosition
std::array<int, 2> Chessboard::FuzzilyCast(glm::vec2& mouseCoord)
{
	std::array<int, 2>& castPosition = ScreenCoordToBoard(mouseCoord);			// take advantage of round， take& since return a var
	glm::vec2& target_0 = BoardCoordToScreen(castPosition[0], castPosition[1]);		// access nearby targets
	glm::vec2& target_1 = BoardCoordToScreen(castPosition[0], castPosition[1] + 1);
	glm::vec2& target_2 = BoardCoordToScreen(castPosition[0], castPosition[1] - 1);

	if (glm::dot(mouseCoord - target_0, mouseCoord - target_0) < chessRadius * chessRadius)
		return castPosition;
	if (glm::dot(mouseCoord - target_1, mouseCoord - target_1) < chessRadius * chessRadius)
		return { castPosition[0], castPosition[1] + 1 };
	if (glm::dot(mouseCoord - target_2, mouseCoord - target_2) < chessRadius * chessRadius)
		return { castPosition[0], castPosition[1] - 1 };

	return {0, 0};
}

Chess* Chessboard::JudgeTeamRound(Chess* chess)
{
	if (!chess)
		return nullptr;
	else if (m_Props.ActiveTeam == static_cast<int>(chess->GetColor()) && chess->GetEnableState())
		return chess;

	return nullptr;
}
// to check if position on chessboard
bool Chessboard::IsValidBoardPosition(std::array<int, 2>& position) const
{
	if (!(position[0]||position[1]))
		return false;
	// just simply divided to 2 part, not best algo
	if (position[1] > -1 && position[1] < 9)
	{
		switch (position[1])
		{
		case 0: if (position[0] == 12)					    return true;			return false;
		case 1: if (position[0] > 10 &&  position[0] < 13)  return true;			return false;
		case 2: if (position[0] > 9  &&  position[0] < 13)  return true;			return false;
		case 3: if (position[0] > 8  &&  position[0] < 13)  return true;			return false;
		case 4: if (position[0] > 3  &&  position[0] < 17)  return true;			return false;
		case 5: if (position[0] > 3  &&  position[0] < 16)  return true;			return false;
		case 6: if (position[0] > 3  &&  position[0] < 15)  return true;			return false;
		case 7: if (position[0] > 3  &&  position[0] < 14)  return true;			return false;
		case 8: if (position[0] > 3  &&  position[0] < 13)  return true;			return false;
		}
	}
	else if (position[1] > 8 && position[1] < 17)
	{
		switch (position[1])
		{
		case 9:  if (position[0] > 2  &&  position[0] < 13)  return true;			return false;
		case 10: if (position[0] > 1  &&  position[0] < 13)  return true;			return false;
		case 11: if (position[0] > 0  &&  position[0] < 13)  return true;			return false;
		case 12: if (position[0] > -1 &&  position[0] < 13)  return true;			return false;
		case 13: if (position[0] > 3  &&  position[0] < 8 )  return true;			return false;
		case 14: if (position[0] > 3  &&  position[0] < 7 )  return true;			return false;
		case 15: if (position[0] > 3  &&  position[0] < 6 )  return true;			return false;
		case 16: if (position[0] == 4)					     return true;			return false;
		}
	}
	else
		return false;
}

bool Chessboard::IsChessAdjacent(Chess* chess, std::array<int, 2> boardPosition)const//判断是否相邻
{
	std::array<glm::vec2, 2> chessCoord = chess->GetCoord();
	glm::vec2 chessOriginCoord;
	chessOriginCoord.x = chessCoord[0].x + chessRadius;
	chessOriginCoord.y = chessCoord[0].y - chessRadius;
	std::array<int, 2>& castPosition = ScreenCoordToBoard(chessOriginCoord);
	if (m_AdjacentFlag)
	{
		if (IsValidBoardPosition(boardPosition))
		{

			if (m_BoardPos[boardPosition[0]][boardPosition[1]] != nullptr)
			{


				if (castPosition[0] + castPosition[1] == boardPosition[0] + boardPosition[1] && abs(castPosition[0] - boardPosition[0]) == 1 && chess != nullptr)
				{
					if (!m_BoardPos[boardPosition[0]][boardPosition[1]].get()->GetChess())
						return true;
				}
				else if (boardPosition[0] == castPosition[0] && abs(boardPosition[1] - castPosition[1]) == 1 && chess != nullptr)
				{
					if (!m_BoardPos[boardPosition[0]][boardPosition[1]].get()->GetChess())
						return true;
				}
				else if (boardPosition[1] == castPosition[1] && abs(boardPosition[0] - castPosition[0]) == 1 && chess != nullptr)
				{
					if (!m_BoardPos[boardPosition[0]][boardPosition[1]].get()->GetChess())
						return true;
				}
			}
		}
	}
	return false;
}

bool Chessboard::IsChessSkipValid(Chess* chess, std::array<int, 2> boardPosition)const
{
	std::array<glm::vec2, 2> chessCoord = chess->GetCoord();
	glm::vec2 chessCentreCoord;
	chessCentreCoord.x = chessCoord[0].x + chessRadius;
	chessCentreCoord.y = chessCoord[0].y - chessRadius;
	std::array<int, 2>& castPosition = ScreenCoordToBoard(chessCentreCoord);
	std::array<int, 2> tempCoord;
	if (IsValidBoardPosition(boardPosition))
	{
		if (m_BoardPos[boardPosition[0]][boardPosition[1]] != nullptr)
		{
			Chess* targetBoardPos = m_BoardPos[boardPosition[0]][boardPosition[1]].get()->GetChess();
			if (chess != nullptr)
			{
				if (castPosition[0] == boardPosition[0] && abs(castPosition[1] - boardPosition[1]) == 2 && targetBoardPos == nullptr)//情况1:x相等
				{
					if (castPosition[1] - boardPosition[1] == 2)
					{
						tempCoord[0] = castPosition[0];
						tempCoord[1] = boardPosition[1] + 1;
						Chess* SaveInfoChess = m_BoardPos[tempCoord[0]][tempCoord[1]].get()->GetChess();//SaveInfoChess是所隔的棋子
						if (SaveInfoChess != nullptr)
							return true;
					}
					else
					{
						tempCoord[0] = castPosition[0];
						tempCoord[1] = boardPosition[1] - 1;
						Chess* SaveInfoChess = m_BoardPos[tempCoord[0]][tempCoord[1]].get()->GetChess();
						if (SaveInfoChess != nullptr)
							return true;
					}
				}
				else if (castPosition[1] == boardPosition[1] && abs(castPosition[0] - boardPosition[0]) == 2 && targetBoardPos == nullptr)//情况2:y相等
				{
					if (castPosition[0] - boardPosition[0] == 2)
					{
						tempCoord[0] = castPosition[0] - 1;
						tempCoord[1] = boardPosition[1];
						Chess* SaveInfoChess = m_BoardPos[tempCoord[0]][tempCoord[1]].get()->GetChess();
						if (SaveInfoChess != nullptr)
							return true;

					}
					else
					{
						tempCoord[0] = castPosition[0] + 1;
						tempCoord[1] = boardPosition[1];
						Chess* SaveInfoChess = m_BoardPos[tempCoord[0]][tempCoord[1]].get()->GetChess();
						if (SaveInfoChess != nullptr)
							return true;
					}
				}
				else if (castPosition[0] + castPosition[1] == boardPosition[0] + boardPosition[1] && abs(castPosition[0] - boardPosition[0]) == 2 && targetBoardPos == nullptr)//情况三:x+y=c
				{
					if (castPosition[0] - boardPosition[0] == 2)
					{
						tempCoord[0] = castPosition[0] - 1;
						tempCoord[1] = boardPosition[1] - 1;
						Chess* SaveInfoChess = m_BoardPos[tempCoord[0]][tempCoord[1]].get()->GetChess();
						if (SaveInfoChess != nullptr)
							return true;
					}
					else
					{
						tempCoord[0] = castPosition[0] + 1;
						tempCoord[1] = boardPosition[1] + 1;
						Chess* SaveInfoChess = m_BoardPos[tempCoord[0]][tempCoord[1]].get()->GetChess();
						if (SaveInfoChess != nullptr)
							return true;
					}
				}
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------
// AI relative operations
std::array<int, 2> Chessboard::BoardCoord(int x, int y) const
{
	std::array<int, 2> BoardPos;
	BoardPos[0] = x;
	BoardPos[1] = y;
	return BoardPos;
}

bool Chessboard::SkipValid(Chess* chess) const
{
	if (chess)
	{
		std::array<glm::vec2, 2> chessCoords = chess->GetCoord();
		glm::vec2 chessOriginCoord;
		chessOriginCoord.x = chessCoords[0].x + chessRadius;
		chessOriginCoord.y = chessCoords[0].y - chessRadius;
		std::array<int, 2>& castPos = ScreenCoordToBoard(chessOriginCoord);
		if (IsValidBoardPosition(castPos))
		{
			if (chess)
			{
				// check chess distribution in 6 directions: if theres a chess alongside and empty through the direction
				if (PosHasChess(castPos[0], castPos[1] + 1) && !PosHasChess(castPos[0], castPos[1] + 2))
				{
					return true;
				}
				if (PosHasChess(castPos[0], castPos[1] - 1) && !PosHasChess(castPos[0], castPos[1] - 2))
				{
					return true;
				}
				if (PosHasChess(castPos[0] + 1, castPos[1]) && !PosHasChess(castPos[0] + 2, castPos[1]))
				{
					return true;
				}
				if (PosHasChess(castPos[0] - 1, castPos[1]) && !PosHasChess(castPos[0] - 2, castPos[1]))
				{
					return true;
				}
				if (PosHasChess(castPos[0] - 1, castPos[1] + 1) && !PosHasChess(castPos[0] - 2, castPos[1] + 2))
				{
					return true;
				}
				if (PosHasChess(castPos[0] + 1, castPos[1] - 1) && !PosHasChess(castPos[0] + 2, castPos[1] - 2))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Chessboard::PosHasChess(int x, int y) const
{
	if (x >= 0 && x < 17 && y >= 0 && y < 17)
	{
		if (IsValidBoardPosition(BoardCoord(x, y)))
		{
			Chess* chessPos = m_BoardPos[x][y].get()->GetChess();
			if (chessPos != nullptr) return true;
		}
	}
	return false;
}

ChessColor Chessboard::GetOpponentColor(ChessColor color)
{
	switch (color) {
	case ChessColor::BLACK: return ChessColor::YELLOW;
	case ChessColor::YELLOW: return ChessColor::BLACK;
	case ChessColor::RED: return ChessColor::BLUE;
	case ChessColor::BLUE: return ChessColor::RED;
	case ChessColor::PURPLE: return ChessColor::GREEN;
	case ChessColor::GREEN: return ChessColor::PURPLE;
	}
}

bool Chessboard::AIMakeMove(int activeTeam)
{
	EG_WARN("AIMove attempted!");

	m_AI->setChessboard(this);
	RefreshCopyBoard();
	if (activeTeam == 0)
	{
		return false;
	}
	else
	{
		ChessColor activeTeamColor{};
		ChessColor opposiveTeamColor{};
		switch (activeTeam)
		{
		case 1:
			activeTeamColor = ChessColor::YELLOW;
			opposiveTeamColor = GetOpponentColor(activeTeamColor);
			break;
		case 2:
			activeTeamColor = ChessColor::RED;
			opposiveTeamColor = GetOpponentColor(activeTeamColor);
			break;
		case 3:
			activeTeamColor = ChessColor::BLUE;
			opposiveTeamColor = GetOpponentColor(activeTeamColor);
			break;
		case 4:
			activeTeamColor = ChessColor::PURPLE;
			opposiveTeamColor = GetOpponentColor(activeTeamColor);
			break;
		case 5:
			activeTeamColor = ChessColor::GREEN;
			opposiveTeamColor = GetOpponentColor(activeTeamColor);
			break;
		}

		std::array<int, 5> bestMovement = m_AI->AI_FindDeeply(m_CopyBoard, activeTeamColor, opposiveTeamColor, 4, 3);
		if (bestMovement[0] != -1 && IsValidBoardPosition(BoardCoord(bestMovement[0], bestMovement[1]))) {  // 检查是否找到了有效的移动且索引在范围内
			//---------------------------------------------------------------------
			//---------------------------------------------------------------------
			Chess* chess = m_BoardPos[bestMovement[0]][bestMovement[1]].get()->GetChess();
			if (chess)
			{
				if (IsValidBoardPosition(BoardCoord(bestMovement[2], bestMovement[3])))
				{
					std::array<glm::vec2, 2> chessCoords = chess->GetCoord();

					Chess* targetBoardPos = m_BoardPos[bestMovement[2]][bestMovement[3]].get()->GetChess();
					if (targetBoardPos == nullptr)
					{
						// reserve the access to previous position
						glm::vec2 chessOriginCoord;
						chessOriginCoord.x = chessCoords[0].x + chessRadius;
						chessOriginCoord.y = chessCoords[0].y - chessRadius;
						std::array<int, 2>& castPosition = ScreenCoordToBoard(chessOriginCoord);

						chessCoords[0][0] = 82 * tan30 * bestMovement[2] + 41 * tan30 * bestMovement[3] + xOffset - chessRadius;
						chessCoords[0][1] = 720 - (0 + 41 * bestMovement[3] + yOffset) + chessRadius;
						chessCoords[1][0] = 82 * tan30 * bestMovement[2] + 41 * tan30 * bestMovement[3] + xOffset + chessRadius;
						chessCoords[1][1] = 720 - (0 + 41 * bestMovement[3] + yOffset) - chessRadius;
						chess->SetScreenCoord(chessCoords);
						// update the chess movement on our board
						m_BoardPos[bestMovement[2]][bestMovement[3]]->SetChess(chess);
						// remove chess pointer at the previous position
						m_BoardPos[castPosition[0]][castPosition[1]]->SetChess(nullptr);
						RefreshCopyBoard();
					}
				}
			}
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
// core move function
bool Chessboard::OnChessMoved(Chess* chess, std::array<int, 2> boardPosition)
{
	if (IsValidMovement(chess, boardPosition))
	{
		std::array<glm::vec2, 2> chessCoords = chess->GetCoord();

		Chess* targetBoardPos = m_BoardPos[boardPosition[0]][boardPosition[1]].get()->GetChess();
		if (targetBoardPos == nullptr)
		{
			// reserve the access to previous position
			glm::vec2 chessOriginCoord;
			chessOriginCoord.x = chessCoords[0].x + chessRadius;
			chessOriginCoord.y = chessCoords[0].y - chessRadius;
			std::array<int, 2>& castPosition = ScreenCoordToBoard(chessOriginCoord);

			chessCoords[0][0] = 82 * tan30 * boardPosition[0] + 41 * tan30 * boardPosition[1] + xOffset - chessRadius;
			chessCoords[0][1] = 720 - (0 + 41 * boardPosition[1] + yOffset) + chessRadius;
			chessCoords[1][0] = 82 * tan30 * boardPosition[0] + 41 * tan30 * boardPosition[1] + xOffset + chessRadius;
			chessCoords[1][1] = 720 - (0 + 41 * boardPosition[1] + yOffset) - chessRadius;
			chess->SetScreenCoord(chessCoords);
			// update the chess movement on our board
			m_BoardPos[boardPosition[0]][boardPosition[1]]->SetChess(chess);
			// remove chess pointer at the previous position
			m_BoardPos[castPosition[0]][castPosition[1]]->SetChess(nullptr);
			return true;
		}
	}
	return false;
}
// core judging func
bool Chessboard::IsValidMovement(Chess* chess, std::array<int, 2> boardPosition) const
{
	if (chess == nullptr)
		return false;
	else
	{
		if (IsChessAdjacent(chess, boardPosition) || IsChessSkipValid(chess, boardPosition))//
		{
			Chess* targetChess = m_BoardPos[boardPosition[0]][boardPosition[1]].get()->GetChess();
			if (targetChess == nullptr)
				return true;
		}
		return false;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
// specially for KeyEvent
bool Chessboard::OnKeyPressed(Engine::KeyPressedEvent& e)
{
	if (m_InputProps.ControlState)
	{
		if (Engine::Input::IsKeyPressed(Engine::KeyCode::Z))
		{
			if (m_BoardChoice == glm::ivec2(-1))
				m_BoardChoice = glm::ivec2(12, 16);
			// m_BoardChoice is the position choosing(in another coordinate)
			std::array<int, 2> boardChoice = { m_BoardChoice.x, 16 - m_BoardChoice.y };

			if (m_ChessChosen == nullptr)
			{
				m_AdjacentFlag = true;
				m_ChessChosen = JudgeTeamRound(m_BoardPos[boardChoice[0]][boardChoice[1]].get()->GetChess());
				if (m_ChessChosen)
				{
					return true;
				}
				return false;
			}
			else
			{
				if (m_ChessChosen == m_BoardPos[boardChoice[0]][boardChoice[1]].get()->GetChess())
				{
					// cancel chose operation here
					if (m_TeamMoved)
						Skip();
					m_ChessChosen = nullptr;
					return true;
				}
				// checking movement
				if (IsValidMovement(m_ChessChosen, boardChoice))
				{
					m_TeamMoved = true;
					// checking superSkip
					bool chessSkipValid = IsChessSkipValid(m_ChessChosen, boardChoice);
					m_AdjacentFlag = chessSkipValid ? false : true;
					// turn into move event
					if (OnChessMoved(m_ChessChosen, boardChoice))
					{
						//EG_INFO("Have moved a Chess from ({0}, {1})", castPosition[0], castPosition[1]);
						if (IsWinning())
						{
							m_Props.Winner = IsWinning();
							Skip();
							return true;
						}
						if (!chessSkipValid)
							Skip();
						return true;
					}
				}
			}
		}
	}
	return false;
}
// mouseButton Event
bool Chessboard::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
{
	if (Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonLeft) ||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::ButtonRight))
	{
		glm::vec2 mouseCoord = { Engine::Input::GetMouseX(), Engine::Input::GetMouseY() };
		std::array<int, 2>& castPosition = FuzzilyCast(mouseCoord);
		// start dealing with event
		if (IsValidBoardPosition(castPosition))
		{
			//EG_WARN("MousePressed, castPosition:({0}, {1})", castPosition[0], castPosition[1]);
			if (m_ChessChosen == nullptr)
			{
				m_AdjacentFlag = true;
				m_ChessChosen = JudgeTeamRound(m_BoardPos[castPosition[0]][castPosition[1]].get()->GetChess());
				if (m_ChessChosen)
				{
					//EG_INFO("Have chosen a Chess at ({0}, {1})", castPosition[0], castPosition[1]);
					return true;
				}
				//EG_TRACE("Invalid chess chosen at ({0}, {1})", castPosition[0], castPosition[1]);
				return false;
			}
			else
			{
				if (m_ChessChosen == m_BoardPos[castPosition[0]][castPosition[1]].get()->GetChess())
				{
					// cancel chose operation here
					if (m_TeamMoved)
						Skip();
					m_ChessChosen = nullptr;
					return true;
				}
				// checking movement
				if (IsValidMovement(m_ChessChosen, castPosition))
				{
					m_TeamMoved = true;
					// checking superSkip
					bool chessSkipValid = IsChessSkipValid(m_ChessChosen, castPosition);
					m_AdjacentFlag = chessSkipValid ? false : true;
					// turn into move event
					if (OnChessMoved(m_ChessChosen, castPosition))
					{
						//EG_INFO("Have moved a Chess from ({0}, {1})", castPosition[0], castPosition[1]);
						if(IsWinning())
						{
							m_Props.Winner = IsWinning();
							Skip();
							return true;
						}
						if (!chessSkipValid)
							Skip();
						return true;
					}
				}
			}
		}
	}
	return false;
}

glm::ivec2 Chessboard::GetChooseCoord(uint8_t index)
{
	glm::ivec2 quickChooseCoord;
	for (int i = 0; i < 17; i++)
	{
		quickChooseCoord[0] = i;
		for (int j = 0; j < 17; j++)
		{
			quickChooseCoord[1] = j;
			if (m_BoardPos[i][j] && m_BoardPos[i][j]->GetChess() == m_Chesses[m_Props.ActiveTeam][m_InputProps.QuickChoice].get())
				return { quickChooseCoord.x, 16 - quickChooseCoord.y };
		}
	}
}

void Chessboard::QuickChoose()
{
	if (Engine::Input::IsKeyPressed(Engine::KeyCode::V) ||
		Engine::Input::IsGamepadButtonPressed(Engine::XboxCode::ButtonLB)||
		Engine::Input::IsMouseButtonPressed(Engine::MouseCode::Button4))
	{
		if (m_InputProps.ShakeFreeFlag)
		{
			m_InputProps.ControlState = true;
			m_BoardChoice = GetChooseCoord(m_InputProps.QuickChoice);
			m_InputProps.QuickChoice = m_InputProps.QuickChoice > 8 ? 0 : m_InputProps.QuickChoice + 1;

			ButtonEffect::RefreshButtonListD2(m_BoardList, m_BoardChoice);
			m_InputProps.ShakeFreeFlag = false;
		}
	}
	else if (Engine::Input::IsKeyReleased(Engine::KeyCode::V)&&
		Engine::Input::IsGamepadButtonReleased(Engine::XboxCode::ButtonLB)&&
		Engine::Input::IsMouseButtonReleased(Engine::MouseCode::Button4))
			m_InputProps.ShakeFreeFlag = true;
}

//--------------------------------------------------------------------------------------------------------------------
// coordinate transform function
glm::vec2 Chessboard::BoardCoordToScreen(int xCoord, int yCoord) const
{
	glm::vec2 screenCoord;

	screenCoord.x = 82 * tan30 * xCoord + 41 * tan30 * yCoord + xOffset;
	screenCoord.y = 720 - (0 + 41 * yCoord + yOffset) ;

	return screenCoord;
}

std::array<int, 2> Chessboard::ScreenCoordToBoard(glm::vec2& screenCoord) const
{
	std::array<int, 2> boardCoord;

	boardCoord[0] = static_cast<int>(std::round(reversedP_00 * (screenCoord.x - xOffset) + reversedP_01 * (720 - yOffset - screenCoord.y)));
	boardCoord[1] = static_cast<int>(std::round(0 + reversedP_11 * (720 - yOffset - screenCoord.y)));

	return boardCoord;
}
//--------------------------------------------------------------------------------------------------------------------
// core logic updating chessboard(rendering and other logical functions)
void Chessboard::Init(const TeamCount& teamCount, bool gameMode, bool loadMode, std::string& filepath, const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord)
{
	Invalidate();
	m_Props.GameMode = gameMode; // update once
	// Textures init
	m_Texture = Engine::Texture2D::Create("assets/textures/Backgrounds/chessboard.png");
	m_PickCursorTex = Engine::Texture2D::Create("assets/textures/Buttons/pickPosition.png");
	m_Coord[0] = leftLowerCoord;
	m_Coord[1] = rightHigherCoord;
	// subTextures for chesses
	{
		m_ChessTextures = Engine::Texture2D::Create("assets/textures/Buttons/newChessSprite.png");
		m_BlackChessTex			= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 0, 1 }, { 30, 30 });
		m_BlackChessChosenTex	= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 0, 0 }, { 30, 30 });
		m_YellowChessTex		= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 1, 1 }, { 30, 30 });
		m_YellowChessChosenTex	= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 1, 0 }, { 30, 30 });
		m_RedChessTex			= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 2, 1 }, { 30, 30 });
		m_RedChessChosenTex		= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 2, 0 }, { 30, 30 });
		m_BlueChessTex			= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 3, 1 }, { 30, 30 });
		m_BlueChessChosenTex	= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 3, 0 }, { 30, 30 });
		m_PurpleChessTex		= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 4, 1 }, { 30, 30 });
		m_PurpleChessChosenTex	= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 4, 0 }, { 30, 30 });
		m_GreenChessTex			= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 5, 1 }, { 30, 30 });
		m_GreenChessChosenTex	= Engine::SubTexture2D::CreateFromCoord(m_ChessTextures, { 5, 0 }, { 30, 30 });
	}

	m_UI.Init();

	// chess.coord.y is in [ 32.0f, 720 - 32.0f ] for size,  their width: 328*sqrt(3)

	if (loadMode)
		Load(filepath);
	else
	{
		{	// reset statistic data
			m_Props.TeamCount = teamCount;
			m_Props.TimeLimit = 9.999f;
			m_Props.ActiveTeam = 0;
			m_Props.Round = 0;
		}
		// using make unique guarantees chesses' destruct
		for (int i = 0; i < static_cast<int>(teamCount) + 2; i++)
		{
			int j = 0;
			switch (i)
			{
			case 0:
			{
				for (int y = 0; y < 4; y++)
				{
					for (int x = 12 - y; x < 13; x++)
					{
						m_Chesses[i][j] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(i), false);
						m_BoardPos[x][y]->SetChess(m_Chesses[i][j].get());
						j++;
					}
				}
				EG_ASSERT(j == 10, "Exception: chessArray haven't init completely");
				break;
			}
			case 1:
			{
				for (int y = 13; y < 17; y++)
				{
					for (int x = 20 - y; x > 3; x--)
					{
						m_Chesses[i][j] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(i), false);
						m_BoardPos[x][y]->SetChess(m_Chesses[i][j].get());
						j++;
					}
				}
				break;
			}
			case 2:
			{
				for (int y = 4; y < 8; y++)
				{
					for (int x = 20 - y; x > 12; x--)
					{
						m_Chesses[i][j] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(i), false);
						m_BoardPos[x][y]->SetChess(m_Chesses[i][j].get());
						j++;
					}
				}
				break;
			}
			case 3:
			{
				for (int y = 9; y < 13; y++)
				{
					for (int x = 12 - y; x < 4; x++)
					{
						m_Chesses[i][j] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(i), false);
						m_BoardPos[x][y]->SetChess(m_Chesses[i][j].get());
						j++;
					}
				}
				break;
			}
			case 4:
			{
				for (int y = 9; y < 13; y++)
				{
					for (int x = 21 - y; x < 13; x++)
					{
						m_Chesses[i][j] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(i), false);
						m_BoardPos[x][y]->SetChess(m_Chesses[i][j].get());
						j++;
					}
				}
				break;
			}
			case 5:
			{
				for (int y = 4; y < 8; y++)
				{
					for (int x = 11 - y; x > 3; x--)
					{
						m_Chesses[i][j] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(i), false);
						m_BoardPos[x][y]->SetChess(m_Chesses[i][j].get());
						j++;
					}
				}
				break;
			}
			default:
				// add some error callback here or limit it in modeSelectLayer
				EG_ASSERT(false, "Invalid player count!");
				break;
			}
		}
	}
	RefreshChessState();
}

uint32_t Chessboard::IsWinning()
{
	// using 0 as a boolean by giving all winners a offset 1
	constexpr uint32_t offset = 1;
	switch (m_Props.ActiveTeam)
	{
	case 0:
	{
		for (int y = 13; y < 17; y++)
		{
			for (int x = 20 - y; x > 3; x--)
			{
				Chess* checkPoint = m_BoardPos[x][y]->GetChess();
				if (!checkPoint)
					return false;
				if (checkPoint->GetColor() != ChessColor::BLACK)
					return false;
			}
		}
		return static_cast<uint32_t>(ChessColor::BLACK) + offset;
	}
	case 1:
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 12 - y; x < 13; x++)
			{
				Chess* checkPoint = m_BoardPos[x][y]->GetChess();
				if (!checkPoint)
					return false;
				if (checkPoint->GetColor() != ChessColor::YELLOW)
					return false;
			}
		}
		return static_cast<uint32_t>(ChessColor::YELLOW) + offset;
	}
	case 2:
	{
		for (int y = 9; y < 13; y++)
		{
			for (int x = 12 - y; x < 4; x++)
			{
				Chess* checkPoint = m_BoardPos[x][y]->GetChess();
				if (!checkPoint)
					return false;
				if (checkPoint->GetColor() != ChessColor::RED)
					return false;
			}
		}
		return static_cast<uint32_t>(ChessColor::RED) + offset;
	}
	case 3:
	{
		for (int y = 4; y < 8; y++)
		{
			for (int x = 20 - y; x > 12; x--)
			{
				Chess* checkPoint = m_BoardPos[x][y]->GetChess();
				if (!checkPoint)
					return false;
				if (checkPoint->GetColor() != ChessColor::BLUE)
					return false;
			}
		}
		return static_cast<uint32_t>(ChessColor::BLUE) + offset;
	}
	case 4:
	{
		for (int y = 4; y < 8; y++)
		{
			for (int x = 11 - y; x > 3; x--)
			{
				Chess* checkPoint = m_BoardPos[x][y]->GetChess();
				if (!checkPoint)
					return false;
				if (checkPoint->GetColor() != ChessColor::PURPLE)
					return false;
			}
		}
		return static_cast<uint32_t>(ChessColor::PURPLE) + offset;
	}
	case 5:
	{
		for (int y = 9; y < 13; y++)
		{
			for (int x = 21 - y; x < 13; x++)
			{
				Chess* checkPoint = m_BoardPos[x][y]->GetChess();
				if (!checkPoint)
					return false;
				if (checkPoint->GetColor() != ChessColor::GREEN)
					return false;
			}
		}
		return static_cast<uint32_t>(ChessColor::GREEN) + offset;
	}
	}
	return true;
}

void Chessboard::Skip()
{
	m_InputProps.QuickChoice = 0;
	m_TeamMoved = false;
	m_Props.ActiveTeam++;
	m_Props.TimeLimit = 9.999f;

	if (m_Props.ActiveTeam > static_cast<int>(m_Props.TeamCount) + 1)
	{
		m_Props.ActiveTeam = 0;
		m_Props.Round++;
	}
	// AI interface here(started after first player movement)
	if (!m_Props.GameMode && m_Props.ActiveTeam)
	{
		AIMakeMove(m_Props.ActiveTeam);
		m_Props.TimeLimit = 1.0f;	
		if (IsWinning())
		{
			m_Props.Winner = IsWinning();
		}
	}

	RefreshChessState();
}

void Chessboard::RefreshChessState()
{
	if(m_Props.GameMode)
	{
		for (auto& chessIt : m_Chesses[m_Props.ActiveTeam ? m_Props.ActiveTeam - 1 : static_cast<int>(m_Props.TeamCount) + 1])
			chessIt->SetEnableState(false);
		for (auto& chessIt : m_Chesses[m_Props.ActiveTeam])
			chessIt->SetEnableState(true);
	}
	else
	{
		for (auto& chessIt : m_Chesses[m_Props.ActiveTeam ? m_Props.ActiveTeam - 1 : static_cast<int>(m_Props.TeamCount) + 1])
			chessIt->SetEnableState(false);
		for (auto& playerIt : m_Chesses[0])
			playerIt->SetEnableState(!((bool)m_Props.ActiveTeam));
	}
	m_ChessChosen = nullptr;
}
// invalidate the whole chessboard, Pos and Chesses included
void Chessboard::Invalidate()
{
	m_Props.TeamCount = TeamCount::Two; // temp count
	m_Props.ActiveTeam = 0;
	m_Props.Winner = 0;
	m_InputProps.Invalidate();

	std::array<int, 2> chessboardCoordinate = { 0, 0 };
	for (int i = 0; i < 17; i++)
	{
		chessboardCoordinate[0] = i;
		for (int j = 0; j < 17; j++)
		{
			chessboardCoordinate[1] = j;
			if(IsValidBoardPosition(chessboardCoordinate))
			{
				m_BoardPos[i][j].reset(new ChessboardPos(i, j, chessRadius, nullptr));
				m_BoardList[i][16 - j] = static_cast<Engine::Button*>(m_BoardPos[i][j].get()); // upload list init after boardPos
			}
			else
			{
				m_BoardPos[i][j].reset(nullptr);
				m_BoardList[i][16 - j] = nullptr;
			}
		}
	}
	// since init do all other clean works(thanks to unique_ptr)
	// to reset teamCount and active team is enough
}

void Chessboard::OnEvent(Engine::Event& e)
{
	if (e.IsInCategory(Engine::EventCategory::EventCategoryMouse))
	{
		Engine::Input::SetCursorHiddenState(false);
		m_BoardChoice = glm::ivec2(-1);
	}
	Engine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Chessboard::OnMouseButtonPressed));
	dispatcher.Dispatch<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(Chessboard::OnKeyPressed));

}

void Chessboard::OnUpdate(Engine::Timestep ts)
{
	if (m_InputProps.ControlState)
	{
		ButtonEffect::UpdateButtonListD2(m_BoardList, m_BoardChoice, m_LockList);
		UpdateGamepadEvent(ts);
	}
	QuickChoose();
	// ui & time works
	m_Props.TimeLimit -= ts;
	if (m_Props.TimeLimit < 10E-2)
	{
		m_Props.TimeLimit = 9.999f;
		Skip(); // you can DISABLE it for DEBUG
	}
	m_UI.Submit(m_Props.TimeLimit, m_Props.Round, static_cast<int>(m_Props.TeamCount), m_Props.ActiveTeam);
	m_UI.OnUpdate(ts);
	// go through chesses is helpful with tex, but by boardPos could be convenient achieving cover relationship?
	for (int i = 0; i < static_cast<int>(m_Props.TeamCount) + 2; i++)
	{
		int j = 0;
		switch (i)
		{
		case 0:
		{
			EG_ASSERT((m_Chesses[0][0]) != nullptr, "first team is not init yet!");
			for (j; j < 10; j++)
			{
				ButtonEffect::UpdateButton(*m_Chesses[i][j], m_BlackChessChosenTex, m_BlackChessTex, "buttonEffectSound",
					PickProp{ m_ChessChosen ? m_ChessChosen->GetCoord() == m_Chesses[i][j]->GetCoord() : false }); // chess has unique coordinate
			}
			break;
		}
		case 1:
		{
			EG_ASSERT((m_Chesses[0][0]) != nullptr, "second team is not init yet!");
			for (j; j < 10; j++)
			{
				ButtonEffect::UpdateButton(*m_Chesses[i][j], m_YellowChessChosenTex, m_YellowChessTex, "buttonEffectSound",
					PickProp{ m_ChessChosen ? m_ChessChosen->GetCoord() == m_Chesses[i][j]->GetCoord() : false });
			}
			break;
		}
		case 2:
		{
			EG_ASSERT((m_Chesses[0][0]) != nullptr, "third team is not init yet!");
			for (j; j < 10; j++)
			{
				ButtonEffect::UpdateButton(*m_Chesses[i][j], m_RedChessChosenTex, m_RedChessTex, "buttonEffectSound",
					PickProp{ m_ChessChosen ? m_ChessChosen->GetCoord() == m_Chesses[i][j]->GetCoord() : false });
			}
			break;
		}
		case 3:
		{
			EG_ASSERT((m_Chesses[0][0]) != nullptr, "forth team is not init yet!");
			for (j; j < 10; j++)
			{
				ButtonEffect::UpdateButton(*m_Chesses[i][j], m_BlueChessChosenTex, m_BlueChessTex, "buttonEffectSound",
					PickProp{ m_ChessChosen ? m_ChessChosen->GetCoord() == m_Chesses[i][j]->GetCoord() : false });
			}
			break;
		}
		case 4:
		{
			EG_ASSERT((m_Chesses[0][0]) != nullptr, "fifth team is not init yet!");
			for (j; j < 10; j++)
			{
				ButtonEffect::UpdateButton(*m_Chesses[i][j], m_PurpleChessChosenTex, m_PurpleChessTex, "buttonEffectSound",
					PickProp{ m_ChessChosen ? m_ChessChosen->GetCoord() == m_Chesses[i][j]->GetCoord() : false });
			}
			break;
		}
		case 5:
		{
			EG_ASSERT((m_Chesses[0][0]) != nullptr, "sixth team is not init yet!");
			for (j; j < 10; j++)
			{
				ButtonEffect::UpdateButton(*m_Chesses[i][j], m_GreenChessChosenTex, m_GreenChessTex, "buttonEffectSound",
					PickProp{ m_ChessChosen ? m_ChessChosen->GetCoord() == m_Chesses[i][j]->GetCoord() : false });
			}
			break;
		}
		}
	}
	// no need cover. Blow updating valid chessboardPos
	for (int i = 0; i < 17; i++)
		for (int j = 0; j < 17; j++)
		{
			if (m_BoardList[i][16 - j])
				ButtonEffect::UpdateBoardPos(*m_BoardPos[i][j], m_PickCursorTex, glm::ivec2(i, 16 - j), m_BoardChoice, "buttonEffectSound");
		}

}
//--------------------------------------------------------------------------------------------------------------------

void Chessboard::Save(std::string& filepath)
{
	// save by bytes for now
	std::string saveData = "quickSave";
	saveData += static_cast<uint8_t>(m_Props.TeamCount);
	saveData += static_cast<uint8_t>(m_Props.TimeLimit);  // it's not accurate
	saveData += static_cast<uint8_t>(m_Props.Round);
	saveData += static_cast<uint8_t>(m_Props.ActiveTeam);
	saveData += static_cast<uint8_t>(m_Props.GameMode);
	// layout { bool, uint32_t, uint32_t, uint32_t }
	for (char x = 0; x < 17; x++)
	{
		for (char y = 0; y < 17; y++)
		{
			std::string singleData;
			// singleData += { x, y };
			if(m_BoardPos[x][y])
			{
				if (m_BoardPos[x][y]->HasChess())
				{
					singleData += (char)true;
					singleData += static_cast<char>(m_BoardPos[x][y]->GetChess()->GetColor());
				}
				else
				{
					singleData += (char)false;
					singleData += (char)false; // empty chess won't own a color
				}
			}
			else
			{
				singleData += (char)false;
				singleData += (char)true;
			}

			saveData += singleData;
		}
	}

	std::ofstream out(filepath, std::ios::out | std::ios::binary);

	if (out)
	{
		out.write(&saveData[0], saveData.size());
		if (!out)
		{
			EG_CORE_ERROR("Could not write file '{0}'", filepath);
		}
		out.close();
	}
	else
	{
		EG_CORE_ERROR("Could not open file '{0}'", filepath);
	}
	EG_INFO("successfully saved in {0}", filepath);
}

void Chessboard::Load(std::string& filepath)
{
	constexpr uint32_t stride = 2;
	constexpr uint32_t propsLength = 5;

	std::string loadData;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);	// read by binary originally, ',' and '|' both okay
	if (in)
	{
		in.seekg(0, std::ios::end);									// start from index 0 to end
		loadData.resize(in.tellg());									// adapt to file's size
		in.seekg(0, std::ios::beg);
		in.read(&loadData[0], loadData.size());
		in.close();
	}
	else
	{
		EG_ERROR("Could not open file '{0}'", filepath);
	}

	// reference: shader
	if (!loadData.empty())
	{
		const char* loadType = "quickSave";
		size_t loadTypeLength = strlen(loadType);
		size_t pos = loadData.find(loadType, 0);

		if (pos != std::string::npos)
		{
			{	// load settings
				std::string settingData = loadData.substr(pos + loadTypeLength, propsLength);
				m_Props.TeamCount = static_cast<TeamCount>(settingData[0]);
				m_Props.TimeLimit = settingData[1];
				m_Props.Round = settingData[2];
				m_Props.ActiveTeam = settingData[3];
				m_Props.GameMode = settingData[4];
			}

			// load chessboard
			size_t chessDataOffset = pos + loadTypeLength + propsLength;
			size_t loadTime = 0;
			std::unordered_map<char, uint32_t> colorCountingList;
			for (char i = 0; i < static_cast<char>(m_Props.TeamCount) + 1; i++)
				colorCountingList[i] = 0; // init counter, insert key-value pair by default implicitly

			for (uint32_t x = 0; x < 17; x++)
			{
				for (uint32_t y = 0; y < 17; y++)
				{
					std::string chessData = loadData.substr(chessDataOffset + stride * loadTime, stride);
					bool hasChess = chessData[0];
					char color = chessData[1];

					if (!hasChess && color)
						m_BoardPos[x][y] = nullptr;
					else if (hasChess)
					{
						// for a specific color 10 chess is a maximum
						m_Chesses[color][colorCountingList[color]] = Engine::CreateScope<Chess>(x, y, chessRadius, static_cast<ChessColor>(color));
						m_BoardPos[x][y]->SetChess(m_Chesses[color][colorCountingList[color]].get());
						colorCountingList[color]++;
						EG_ASSERT(colorCountingList[color] < 11, "Exception: Too many chess for team 'color'");
					}
					else
						;// do nothing since board has been cleared
					loadTime++;
				}
			}
			// replace it with renderAble stuff
			EG_INFO("Successfully loaded!");
		}
	}
	else
	{
		// some warn here( better to render it like that map)
	}
}

//--------------------------------------------------------------------------------------------------
// class Init func
ChessboardPos::ChessboardPos(const int xCoord, const int yCoord, const int radius, Chess* chess)
	:Engine::Button(glm::vec2(82 * tan30 * xCoord + 41 * tan30 * yCoord + xOffset - radius, 720 - (0 + 41 * yCoord + yOffset) + radius),
		glm::vec2(82 * tan30 * xCoord + 41 * tan30 * yCoord + xOffset + radius, 720 - (0 + 41 * yCoord + yOffset) - radius))
{
}

// AI Init function
Chessboard::Chessboard()
{
	CreateAI();
}

Chessboard::~Chessboard()
{
	delete m_AI;
}

void Chessboard::CreateAI()
{
	m_AI = new AI(this);
}

void Chessboard::RefreshCopyBoard()
{
	for (int i = 0; i < 17; ++i) {
		for (int j = 0; j < 17; ++j) {
			if (IsValidBoardPosition(BoardCoord(i, j)))
			{
				Chess* chess = m_BoardPos[i][j].get()->GetChess();
				if (chess)
				{
					ChessColor chessColor = chess->GetColor();
					switch (chessColor)
					{
					case ChessColor::BLACK:
						m_CopyBoard[i][j].SetColor(Black);
						break;
					case ChessColor::YELLOW:
						m_CopyBoard[i][j].SetColor(Yellow);
						break;
					case ChessColor::GREEN:
						m_CopyBoard[i][j].SetColor(Green);
						break;
					case ChessColor::BLUE:
						m_CopyBoard[i][j].SetColor(Blue);
						break;
					case ChessColor::PURPLE:
						m_CopyBoard[i][j].SetColor(Purple);
						break;
					case ChessColor::RED:
						m_CopyBoard[i][j].SetColor(Red);
						break;
					}
				}
				else if (!chess)
				{
					m_CopyBoard[i][j].SetColor(White);
				}
				m_CopyBoard[i][j].SetRaw(i);
				m_CopyBoard[i][j].SetColumn(j);
			}
		}
	}
}
