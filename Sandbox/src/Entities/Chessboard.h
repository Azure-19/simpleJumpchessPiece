#pragma once
#include <Engine.h>

#include "Chess.h"
#include "UI.h"
#include "Copyboard.h"

inline constexpr auto tan30 = 0.57735027;
inline constexpr auto tan60 = 1.7320508;

class AI;

enum class TeamCount
{
	Two, Three, Four, Five, Six
};

class ChessboardPos : public Engine::Button
{
private:
	Chess* m_Chess = nullptr;			// only free pointer

public:
	ChessboardPos(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, Chess* chess = nullptr)
		:Engine::Button(leftLowerCoord, rightHigherCoord), m_Chess(chess){}
	ChessboardPos(const int xCoord, const int yCoord, const int radius, Chess* chess = nullptr);

	// nullptr means there's no chess
	inline bool HasChess() { return m_Chess; }
	inline Chess* GetChess() { return m_Chess; }
	inline void SetChess(Chess* chess) { m_Chess = chess; }
};

struct BoardProps
{
	float TimeLimit = 9.999f;
	TeamCount TeamCount;
	uint8_t ActiveTeam = 0;
	uint8_t Round = 0;
	uint8_t Winner = 0; // offset between ChessColor value is 1
	bool GameMode = false; // false for AI, true for multiplayer
};

struct ChessBoardInput
{
	uint8_t QuickChoice = 0;
	bool ShakeFreeFlag = true; // true for allowing input
	bool GamepadShakeFreeFlag = true;
	bool ControlState = false; // true for controlling chessboard.
public:
	void Invalidate() {
		QuickChoice = 0;
		ShakeFreeFlag = true;
		GamepadShakeFreeFlag = true;
		ControlState = false;
	}
};

class Chessboard
{
private:
	// Logic and AI
	AI* m_AI;
	CopyBoard m_CopyBoard[17][17];
	bool m_AdjacentFlag = true;
	bool m_TeamMoved = false;
//-------------------------------------------------------------------------------------
	// (>-<) only some of them is valid(not nullptr)
	std::array<std::array<Engine::Scope<ChessboardPos>, 17>, 17> m_BoardPos;
	// RAW pointer for the special situation, coordinate is different with ^
	std::array<std::array<Engine::Button*, 17>, 17> m_BoardList;
	glm::ivec2 m_BoardChoice = { -1, -1 }; // shouldn't reset while changing control state. Reversed by m_BoardList
	ChessBoardInput m_InputProps;

	std::array<std::array<Engine::Scope<Chess>, 10>, 6> m_Chesses = { nullptr };	// not optimized yet
	Engine::Ref<Engine::Texture2D> m_ChessTextures, m_PickCursorTex;	// for all chesses
	Engine::Ref<Engine::SubTexture2D> m_BlackChessTex, m_YellowChessTex,
		m_RedChessTex, m_BlueChessTex, m_PurpleChessTex, m_GreenChessTex,
		m_BlackChessChosenTex, m_YellowChessChosenTex, m_RedChessChosenTex,
		m_BlueChessChosenTex, m_PurpleChessChosenTex, m_GreenChessChosenTex;
	Chess* m_ChessChosen = nullptr;										// make sure that didn't delete Ref ptr

	Engine::Ref<Engine::Texture2D> m_Texture;
	std::array<glm::vec2, 2> m_Coord = {glm::vec2(0, 0)}; // means it's invalidated

	GameUI m_UI; // move it to gameLayer maybe better
public:
	BoardProps m_Props;
private:
	void UpdateGamepadEvent(Engine::Timestep ts);
//-------------------------------------------------------------------------------------

	// return {0, 0} as invalid input.
	std::array<int, 2> FuzzilyCast(glm::vec2& mouseCoord);
	Chess* JudgeTeamRound(Chess* chess);
	bool IsValidBoardPosition(std::array<int, 2>& position) const;

//-------------------------------------------------------------------------------------
// Movement Logical function
	bool OnChessMoved(Chess* chess, std::array<int, 2> boardPosition);
	bool IsValidMovement(Chess* chess, std::array<int, 2> boardPosition) const;

	bool IsChessAdjacent(Chess* chess, std::array<int, 2> boardPosition) const;
	bool IsChessSkipValid(Chess* chess, std::array<int, 2> boardPosition) const;
// AI-relative
	std::array<int, 2>BoardCoord(int x, int y)const;
	bool SkipValid(Chess* chess)const;
	bool PosHasChess(int x, int y)const;
	ChessColor GetOpponentColor(ChessColor color);
	bool AIMakeMove(int activeteam);
//-------------------------------------------------------------------------------------
	bool OnKeyPressed(Engine::KeyPressedEvent& e);
	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e);
	// reversed Y for buttonList
	glm::ivec2 GetChooseCoord(uint8_t index);
	void QuickChoose();
public:
// for AI copy keyboard
	friend class AI;
	void CreateAI();
	void RefreshCopyBoard();
//-------------------------------------------------------------------------------------
	Chessboard();
	~Chessboard();
	inline void SetTeamCount(TeamCount teamCount) { m_Props.TeamCount = teamCount; }
	inline void SetControlState(bool state) { m_InputProps.ControlState = state; }
	inline bool GetControlState() { return m_InputProps.ControlState; }
	inline std::array<glm::vec2, 2>& GetCoord() { return m_Coord; }
	inline Engine::Ref<Engine::Texture2D>& GetTexture() { return m_Texture; }

	glm::vec2 BoardCoordToScreen(int xCoord, int yCoord) const;
	std::array<int, 2> ScreenCoordToBoard(glm::vec2& screenCoord) const;

	
	void OnUpdate(Engine::Timestep ts);
	// provide chess events here
	void OnEvent(Engine::Event& e);

	void Init(const TeamCount& teamCount, bool gameMode, bool loadMode, std::string& filepath, const glm::vec2& leftLowerCoord = { 640 - 180 * tan60 - 20, 720.0f }, const glm::vec2& rightHigherCoord = { 640 + 180 * tan60 + 20, 0.0f });
	// offset to ChessColor is 1
	uint32_t IsWinning();
	void Skip();
	void RefreshChessState();
	// PRIMITIVE
	void Invalidate();
	void Save(std::string& filepath);
	void Load(std::string& filepath);

};