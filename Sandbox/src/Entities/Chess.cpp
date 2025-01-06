#include "Chess.h"

// explained in chessboard.cpp
constexpr auto xOffset = 71.8874;
constexpr auto yOffset = 32;

constexpr auto tan30 = 0.57735027;


Chess::Chess(const glm::vec2& leftLowCoord, const glm::vec2& rightHighCoord, const ChessColor& color, bool enabled)
	:Button(leftLowCoord, rightHighCoord, enabled)
{
	m_Color = color;
}


Chess::Chess(const int xCoord, const int yCoord, const int radius, const ChessColor& color, bool enabled)
	:Button(glm::vec2(82 * tan30 * xCoord + 41 * tan30 * yCoord + xOffset - radius, 720 - (0 + 41 * yCoord + yOffset) + radius),
		glm::vec2(82 * tan30 * xCoord + 41 * tan30 * yCoord + xOffset + radius, 720 - (0 + 41 * yCoord + yOffset) - radius), enabled)
{
	m_Color = color;
}

