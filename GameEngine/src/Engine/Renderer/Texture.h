#pragma once

#include <string>				// string rely
#include "Engine/Core/Core.h"		// Ref rely

namespace Engine {

	// for all texture kind
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		// upload the memory block to gpu
		virtual void SetData(void* data, uint32_t size) = 0;
		// slot:0~31
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};

}