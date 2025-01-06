#include "egpch.h"
#include "SubTexture2D.h"

namespace Engine{

	SubTexture2D::SubTexture2D(const Ref<Texture2D> texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
	// Coord stands for the relative position in a spriteSheet whereas cellSize is a single sprite's size. SpriteSize supports huge texture taking several cells
	Ref<SubTexture2D> SubTexture2D::CreateFromCoord(const Ref<Texture2D> texture, const glm::vec2& coord, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{

		glm::vec2 min = { (coord.x * cellSize.x) / texture->GetWidth(), (coord.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coord.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coord.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };

		
		return CreateRef<SubTexture2D>(texture, min, max);
	}

}
