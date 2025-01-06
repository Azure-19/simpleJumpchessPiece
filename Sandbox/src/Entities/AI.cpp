#include "AI.h"
#include"Chessboard.h"
#include <random>
#include <algorithm>
constexpr auto chessRadius = 15;

AI::AI(Chessboard* chessboard) : chessboard(chessboard) {
}

AI::~AI() {
}

void AI::setChessboard(Chessboard* chessboard) {
	this->chessboard = chessboard;
}



std::array<int, 2> AI::AI_FindSkip(CopyBoard copyboard[17][17], int visited[17][17], std::array<int, 2>Pos)
{
	int x_new = 0;
	int y_new = 0;
	std::array<int, 2> result;
	result[0] = Pos[0];
	result[1] = Pos[1];
	if (chessboard->IsValidBoardPosition(Pos))
	{
		Chess* chess = chessboard->m_BoardPos[Pos[0]][Pos[1]].get()->GetChess();
		if (!(chessboard->SkipValid(chess)))
		{
			for (int i = 0; i < 17; i++)
			{
				for (int j = 0; j < 17; j++)
				{
					if (visited[i][j] == 1)
					{
						visited[i][j] = 0;
					}
				}
			}
			visited[Pos[0]][Pos[1]] = 2;

			return result;
		}

		for (int i = 0; i < 12; i += 2) {
			int newX = Pos[0] + dx[i];
			int newY = Pos[1] + dy[i];
			if (newX >= 0 && newX < 17 && newY >= 0 && newY < 17) {
				if (chessboard->IsChessSkipValid(chess, AI_SetPos(newX, newY)) && !visited[newX][newY] && chessboard->IsValidBoardPosition(chessboard->BoardCoord(newX, newY))) {
					x_new = newX;
					y_new = newY;
					visited[Pos[0]][Pos[1]] = 1;
					return AI_FindSkip(copyboard, visited, AI_SetPos(x_new, y_new));
				}
			}
		}
		for (int i = 0; i < 17; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				if (visited[i][j] == 1)
				{
					visited[i][j] = 0;
				}
			}
		}
	}
	visited[Pos[0]][Pos[1]] = 2;
	return result;

}

std::array<int, 2> AI::AI_SetPos(int x, int y)
{
	if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(x, y)))
	{
		std::array<int, 2> BoardPos;
		BoardPos[0] = x;
		BoardPos[1] = y;
		return BoardPos;
	}
	return std::array<int, 2>();
}



std::array<int, 2> AI::AI_ScreenCoordtoBoard(Chess* chess)
{
	std::array<glm::vec2, 2> chessCoords = chess->GetCoord();
	glm::vec2 chessOriginCoord;
	chessOriginCoord.x = chessCoords[0].x + chessRadius;
	chessOriginCoord.y = chessCoords[0].y - chessRadius;
	std::array<int, 2>& castPosition = chessboard->ScreenCoordToBoard(chessOriginCoord);
	return castPosition;
}

bool AI::AI_HasChess(int x, int y)
{
	if (x >= 0 && x < 17 && y >= 0 && y < 17)
	{
		if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(x, y)))
		{
			Chess* chessPos = chessboard->m_BoardPos[x][y].get()->GetChess();
			if (chessPos != nullptr) return true;
		}
	}
	return false;
}


std::array<int, 5> AI::AI_FindDeeply(CopyBoard copyboard[17][17], ChessColor color_self, ChessColor color_oppo, int level, int depth)
{
	int maxdvalue = -999;
	//ÆðµãÆå×ÓµÄ×ø±êÎ»ÖÃ
	int startRaw = 0;
	int startColumn = 0;

	std::array<int, 5> result_self;
	std::array<int, 5> result_oppo;
	std::array<int, 5> final{};
	std::vector<std::array<int, 2>> selectedFirstPosition = AI_FindSelectPosition(copyboard, color_self);
	int visited[17][17] = {};
	CopyBoard tempchess[17][17];

	// Ê¹ÓÃ·¶Î§forÑ­»·±éÀúÏòÁ¿(ËùÓÐµÄÍ³Ò»ÑÕÉ«µÄÆå×Ó)
	for (int i = 0; i < 10 * level / 4; i++) {
		if (!selectedFirstPosition.empty())
		{
			startRaw = selectedFirstPosition[i][0];
			startColumn = selectedFirstPosition[i][1];

			// ¼ÆËãµ±Ç°Î»ÖÃµÄÆÀ¹ÀÖµ
			int demovalue = AI_EvaluateBoard(chessboard->BoardCoord(startRaw, startColumn), color_self);//ÆÀ¹ÀÆåÅÌµÄ³õÊ¼Î»ÖÃµÄÖµ

			//³õÊ¼»¯
			for (int i = 0; i < 17; i++)
			{
				for (int j = 0; j < 17; j++)
				{
					visited[i][j] = 0;
				}
			}

			std::array<int, 2> tempPosition{ 0, 0 };//³õÊ¼»¯
			std::vector<std::array<int, 2>> allJumpPosition;

			//±éÀú¿ÉÒÔÌøÔ¾µÄÇé¿ö allJumpPosition´æÊý¾Ý
			do
			{
				allJumpPosition.push_back(tempPosition);
				tempPosition = AI_FindSkip(copyboard, visited, chessboard->BoardCoord(startRaw, startColumn));
			} while (allJumpPosition.size() < 2 || (allJumpPosition[allJumpPosition.size() - 1] != tempPosition));

			for (int i = 1; i < 12; i += 2)
			{
				if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(startRaw, startColumn)))
				{
					// ¼ì²éÊÇ·ñ¿ÉÒÔÒÆ¶¯µ½Ä¿±êÎ»ÖÃ
					Chess* chess = chessboard->m_BoardPos[startRaw][startColumn].get()->GetChess();
					if (chessboard->IsChessAdjacent(chess, chessboard->BoardCoord(startRaw + dx[i], startColumn + dy[i])))
					{
						tempPosition[0] = startRaw + dx[i];
						tempPosition[1] = startColumn + dy[i];
						allJumpPosition.push_back(tempPosition);
					}
				}
			}

			for (int i = 0; i < allJumpPosition.size(); i++)
			{
				int destinationRaw = allJumpPosition[i][0];
				int destinationColumn = allJumpPosition[i][1];

				if ((destinationRaw + destinationColumn) && !(startRaw == destinationRaw && startColumn == destinationColumn))//Èç¹û²»ÊÇ0£¬0ÇÒ²»Ô­µØ
				{
					int nowdvalue = AI_EvaluateBoard({ destinationRaw,destinationColumn }, color_self) - demovalue;
					for (int i = 0; i < 17; i++)
					{
						for (int j = 0; j < 17; j++)
						{
							if (AI_HasChess(i, j))
							{
								Chess* chessPos = chessboard->m_BoardPos[i][j].get()->GetChess();
								ChessColor chesscolor = chessPos->GetColor();
								switch (chesscolor)
								{
								case ChessColor::BLACK:
									tempchess[i][j].SetColor(Black);
									break;
								case ChessColor::YELLOW:
									tempchess[i][j].SetColor(Yellow);
									break;
								case ChessColor::GREEN:
									tempchess[i][j].SetColor(Green);
									break;
								case ChessColor::BLUE:
									tempchess[i][j].SetColor(Blue);
									break;
								case ChessColor::PURPLE:
									tempchess[i][j].SetColor(Purple);
									break;
								case ChessColor::RED:
									tempchess[i][j].SetColor(Red);
									break;
								}
							}
							else if ((i + j) && !AI_HasChess(i, j))
							{
								tempchess[i][j].SetColor(White);
							}
							tempchess[i][j].SetRaw(i);
							tempchess[i][j].SetColumn(j);


						}
					}
					Color temp = tempchess[destinationRaw][destinationColumn].getColor();//¶¨ÒåÒ»¸öÁÙÊ±±äÁ¿£¬ÓÃÓÚ´æ´¢Æå×ÓµÄÑÕÉ«
					tempchess[destinationRaw][destinationColumn].SetColor(tempchess[startRaw][startColumn].getColor());//½»»»Æå×ÓµÄÑÕÉ«
					tempchess[startRaw][startColumn].SetColor(temp);

					for (int i = 0; i < depth; i++) {
						result_oppo = AI_Move(tempchess, color_oppo, level);
						nowdvalue -= result_oppo[4];

						temp = tempchess[result_oppo[0]][result_oppo[1]].getColor();//¶¨ÒåÒ»¸öÁÙÊ±±äÁ¿£¬ÓÃÓÚ´æ´¢Æå×ÓµÄÑÕÉ«
						tempchess[result_oppo[0]][result_oppo[1]].SetColor(tempchess[result_oppo[2]][result_oppo[3]].getColor());//½»»»Æå×ÓµÄÑÕÉ«
						tempchess[result_oppo[2]][result_oppo[3]].SetColor(temp);


						result_self = AI_Move(tempchess, color_self, level);
						nowdvalue += result_self[4];

						temp = tempchess[result_self[0]][result_self[1]].getColor();//¶¨ÒåÒ»¸öÁÙÊ±±äÁ¿£¬ÓÃÓÚ´æ´¢Æå×ÓµÄÑÕÉ«
						tempchess[result_self[0]][result_self[1]].SetColor(tempchess[result_self[2]][result_self[3]].getColor());//½»»»Æå×ÓµÄÑÕÉ«
						tempchess[result_self[2]][result_self[3]].SetColor(temp);

					}



					result_oppo = AI_Move(tempchess, color_oppo, level);
					nowdvalue -= result_oppo[4];

					if (nowdvalue > maxdvalue)
					{
						final[0] = startRaw;
						final[1] = startColumn;
						final[2] = destinationRaw;
						final[3] = destinationColumn;
						final[4] = nowdvalue;
						maxdvalue = nowdvalue;
					}

				}

			}
		}
	}

	return final;

}


int AI::AI_EvaluateBoard(std::array<int, 2> boardPos, ChessColor color)
{
	switch (color)
	{
	case ChessColor::YELLOW:
		return BLACK_YELLOW_VALUE[boardPos[0]][16 - boardPos[1]];
	case ChessColor::BLACK:
		return BLACK_YELLOW_VALUE[16 - boardPos[0]][boardPos[1]];

	case ChessColor::BLUE:
		return BLUE_RED_VALUE[16 - boardPos[0]][boardPos[1]];

	case ChessColor::RED:
		return BLUE_RED_VALUE[boardPos[0]][16 - boardPos[1]];

	case ChessColor::GREEN:
		return GREEN_PURPLE_VALUE[16 - boardPos[0]][boardPos[1]];

	case ChessColor::PURPLE:
		return GREEN_PURPLE_VALUE[boardPos[0]][16 - boardPos[1]];
	default:
		return 0;
	}
}

std::vector<std::array<int, 2>> AI::AI_FindSelectPosition(CopyBoard copyboard[17][17], ChessColor color)
{
	std::vector<std::array<int, 2>> AllPositions;

	int count = 0;
	// ¿ªÊ¼Ë«ÖØÑ­»·£¬±éÀúÆåÅÌÉÏµÄÃ¿Ò»¸öÎ»ÖÃ
	for (int i = 0; i < 17 && count < 10; ++i) {
		for (int j = 0; j < 17 && count < 10; ++j)
		{
			if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(i, j)))
			{
				Chess* chessPos = chessboard->m_BoardPos[i][j].get()->GetChess();
				if (chessPos)
				{
					ChessColor chesscolor = chessPos->GetColor();
					// Èç¹ûÆå×ÓÎ»ÖÃÓÐÐ§£¬½øÒ»²½¼ì²éÆå×ÓµÄÑÕÉ«
					if (chesscolor == color)
					{
						// Èç¹ûÆå×ÓÑÕÉ«ÓëÖ¸¶¨ÑÕÉ«Ïà·û£¬Ôò¼ÇÂ¼¸ÃÎ»ÖÃ
						// emplace_backÖ±½ÓÔÚÏòÁ¿Î²²¿²åÈëÒ»¸öÐÂµÄÔªËØ£¬Ð§ÂÊ¸ßÓÚpush_back
						std::array<int, 2> temp;
						temp[0] = i;
						temp[1] = j;
						AllPositions.emplace_back(temp);
						count++;
					}
				}
			}
		}
	}

	// ´´½¨Ò»¸öËæ»úÊýÉú³ÉÆ÷
	std::random_device rd;
	std::mt19937 g(rd());

	// Ê¹ÓÃstd::shuffle´òÂÒÏòÁ¿
	std::shuffle(AllPositions.begin(), AllPositions.end(), g);

	// º¯Êý½áÊøÊ±£¬·µ»ØÊÕ¼¯µ½µÄËùÓÐÖ¸¶¨ÑÕÉ«Æå×ÓµÄÎ»ÖÃÁÐ±í
	return AllPositions;
}


std::array<int, 5> AI::AI_Move(CopyBoard copyboard[17][17], ChessColor color, int level)
{
	int maxdvalue = -999;
	//ÆÀ¹À×î´óÖµÆå×ÓµÄÎ»ÖÃ
	int maxr_end = 0;
	int maxc_end = 0;

	//ÆðµãÆå×ÓµÄ×ø±êÎ»ÖÃ
	int startr = 0;
	int startc = 0;

	int maxr_start = 0;
	int maxc_start = 0;

	int count = 0;

	std::array<int, 5> result = { 0 , 0 , 0 , 0 ,  0 };

	std::vector<std::array<int, 2>> selectedFirstPosition = AI_FindSelectPosition(copyboard, color);

	int visited[17][17] = {};


	// Ê¹ÓÃ·¶Î§forÑ­»·±éÀúÏòÁ¿(ËùÓÐµÄÍ³Ò»ÑÕÉ«µÄÆå×Ó)
	for (int i = 0; i < 10 * level / 4; i++) {
		startr = selectedFirstPosition[i][0];
		startc = selectedFirstPosition[i][1];

		// ¼ÆËãµ±Ç°Î»ÖÃµÄÆÀ¹ÀÖµ
		int demovalue = AI_EvaluateBoard(chessboard->BoardCoord(startr, startc), color);//ÆÀ¹ÀÆåÅÌµÄ³õÊ¼Î»ÖÃµÄÖµ

		//³õÊ¼»¯
		for (int i = 0; i < 17; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				visited[i][j] = 0;
			}
		}


		std::array<int, 2> tempPosition{ 0, 0 };//³õÊ¼»¯

		std::vector<std::array<int, 2>> allJumpPosition;


		//±éÀú¿ÉÒÔÌøÔ¾µÄÇé¿ö allJumpPosition´æÊý¾Ý
		do
		{
			allJumpPosition.push_back(tempPosition);
			tempPosition = AI_FindSkip(copyboard, visited, chessboard->BoardCoord(startr, startc));
		} while (allJumpPosition.size() < 2 || (allJumpPosition[allJumpPosition.size() - 1] != tempPosition));

		for (int i = 1; i < 12; i += 2)
		{
			if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(startr, startc)))
			{
				// ¼ì²éÊÇ·ñ¿ÉÒÔÒÆ¶¯µ½Ä¿±êÎ»ÖÃ
				Chess* chessPos = chessboard->m_BoardPos[startr][startc].get()->GetChess();
				if (chessPos) {
					if (chessboard->IsChessAdjacent(chessPos, chessboard->BoardCoord(startr + dx[i], startc + dy[i])) && chessboard->IsValidBoardPosition(chessboard->BoardCoord(startr + dx[i], startc + dy[i])))
					{
						tempPosition[0] = startr + dx[i];
						tempPosition[1] = startc + dy[i];
						allJumpPosition.push_back(tempPosition);
					}
				}
			}
		}


		//±éÀúËùÓÐ¿ÉÒÔÌøÔ¾µÄÎ»ÖÃ ÕÒ×î´óÖµÆå×ÓµÄÎ»ÖÃ
		for (int i = 0; i < allJumpPosition.size(); i++)
		{
			int raw = allJumpPosition[i][0];
			int col = allJumpPosition[i][1];

			if ((raw + col) && !(startr == raw && startc == col))//Èç¹û²»ÊÇ0£¬0ÇÒ²»Ô­µØ
			{
				int nowdvalue = AI_EvaluateBoard(chessboard->BoardCoord(raw, col), color) - demovalue;
				if (nowdvalue > maxdvalue)//Èç¹û¿ÉÒÔ£¬Ôò¸üÐÂ×î´óÖµºÍ¶ÔÓ¦µÄ×ø±ê
				{
					if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(startr, startc)) && chessboard->IsValidBoardPosition(chessboard->BoardCoord(raw, col)))
					{
						maxr_start = startr;
						maxc_start = startc;

						maxr_end = raw;
						maxc_end = col;

						maxdvalue = nowdvalue;
					}
				}
			}

		}

	}
	if (chessboard->IsValidBoardPosition(chessboard->BoardCoord(maxr_end, maxc_end)))
	{
		Chess* chessposition = chessboard->m_BoardPos[maxr_end][maxc_end].get()->GetChess();//±ê¼Ç´Ë´¦
		if (chessposition != nullptr)
		{
			std::array<int, 5> error = { 0 , 0 , 0 , 0,0 };
			return error;

		}
		// ÉèÖÃ×îÖÕÑ¡ÔñµÄÎ»ÖÃ
		result[0] = maxr_start;
		result[1] = maxc_start;
		result[2] = maxr_end;
		result[3] = maxc_end;
		result[4] = maxdvalue;

		// ·µ»Ø×îÖÕÑ¡ÔñµÄÎ»ÖÃ
		return result;
	}
	return result;
}

std::array<int, 2> AI::AI_GetStartPos()
{
	return startPos;
}

void AI::AI_SetStartPos(std::array<int, 2> startPosition)
{
	startPos = startPosition;
}

std::array<int, 2> AI::AI_GetTargetPos()
{
	return targetPos;
}

void AI::AI_SetTargetPos(std::array<int, 2> targetPosition)
{
	targetPos = targetPosition;
}

int AI::AI_GetStatus()
{
	return Status;
}

void AI::AI_SetStatus(int status)
{
	Status = status;
}

CopyBoard::CopyBoard()
{
	raw = 0;          // ÐÐºÅÄ¬ÈÏÎª0
	col = 0;          // ÁÐºÅÄ¬ÈÏÎª0
	color = White;
}

CopyBoard::~CopyBoard()
{
}

bool CopyBoard::SetRaw(int r)
{
	if (r >= 0 && r <= 16) {
		raw = r; // ÉèÖÃÐÐºÅ
		return true;
	}
	return false;
}

bool CopyBoard::SetColumn(int c)
{
	if (c >= 0 && c <= 16) {
		col = c; // ÉèÖÃÁÐºÅ
		return true;
	}
	return false;
}

void CopyBoard::SetColor(Color c)
{
	color = c;
}

int CopyBoard::getRaw()
{
	return raw;
}

int CopyBoard::getCol()
{
	return col;
}

Color CopyBoard::getColor()
{
	return color;
}

