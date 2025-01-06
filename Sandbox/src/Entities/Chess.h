#pragma once

#include <Engine.h>

enum class ChessColor
{
	BLACK, YELLOW, RED, BLUE, PURPLE, GREEN
};

class Chess : public Engine::Button
{
private:
	ChessColor m_Color;
public:
	Chess(const glm::vec2& leftLowCoord, const glm::vec2& rightHighCoord, const ChessColor& color, bool enabled = true);
	Chess(const int xCoord, const int yCoord, const int radius, const ChessColor& color, bool enabled = true);

	inline void SetScreenCoord(const std::array<glm::vec2, 2>& newCoord) { m_ScreenCoord = newCoord; }

	inline ChessColor& GetColor() { return m_Color; }
	inline void SetColor(uint32_t color) { m_Color = static_cast<ChessColor>(color); }
};

