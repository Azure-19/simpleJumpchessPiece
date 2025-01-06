#include "egpch.h"
#include "Renderer2D.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

constexpr auto aspectRatio = 16 / 9.0;
constexpr auto k = 2 / 9.0;


namespace Engine {
//-------------------------------------------   Data structs   -----------------------------------------------------

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> BlankTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferCache = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;		// used as a iterator i guess

		std::array<Ref<Texture2D>, MaxTextureSlots> Textures;
		uint32_t TextureSlotIndex = 1; // 0 for blankTexture, start from 1(where would it be incremented?)
	
		glm::vec4 QuadVertexPosition[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;



//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------     Renderer core funcs      ------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


	void Renderer2D::Init()
	{
		EG_PROFILE_FUNCTION();
		

		// VertexBuffer + IndexBuffer + VertexArray
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		BufferLayout squareLayout =
		{
			{ ShaderDataType::Vec3, "a_Position" },
			{ ShaderDataType::Vec4, "a_Color" },
			{ ShaderDataType::Vec2, "a_TexCoord" },
			{ ShaderDataType::Vec1, "a_TexIndex" },
			{ ShaderDataType::Vec1, "a_TilingFactor" }
		};
		s_Data.QuadVertexBuffer->SetLayout(squareLayout);

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		s_Data.QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		delete[] quadIndices; // could be quite risky when multiThreads is used

		// finally bind them together
		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		// Assign vertexBuffer storing all vertices
		s_Data.QuadVertexBufferCache = new QuadVertex[s_Data.MaxVertices];

		// Generate Samplers(assigning textureSlots)
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		// Shader
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");	
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Texture
		s_Data.BlankTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.BlankTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data.Textures[0] = s_Data.BlankTexture; // init slot 0 as blank textures

		s_Data.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		EG_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		EG_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1; // enable all texture start from 1
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferCache; // reset VB iterate pointer back to start
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		EG_PROFILE_FUNCTION();
		// temp code
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferCache;
	}

	void Renderer2D::EndScene()
	{
		EG_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferCache;	// cast, calculate by bytes
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferCache, dataSize); // dynamic draw all vertex in the offset we increase each call

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.Textures[i]->Bind(i);

		// drawCovered enable depth test
		RenderCommand::DrawCovered(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferCache;		// reset our pointer start

		s_Data.TextureSlotIndex = 1;
	}


//-----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------  Draw funcs  ----------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		EG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		EG_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* subTexCoord = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();


		// looking for a empty slot and fill it
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*(s_Data.Textures[i].get()) == *(texture.get())) // get raw pointer and dereference them for using the operator
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.Textures[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = subTexCoord[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		EG_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f; // blank texture
		constexpr glm::vec2 textureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float tilingFactor = 1.0f;

		// using a loop to simplify the code
		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
	
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();


		// looking for a empty slot and fill it
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*(s_Data.Textures[i].get()) == *(texture.get())) // get raw pointer and dereference them for using the operator
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.Textures[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		EG_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f; // blank texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
			
		s_Data.Stats.QuadCount++;
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		EG_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		// looking for a empty slot and fill it
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*(s_Data.Textures[i].get()) == *(texture.get())) // get raw pointer and dereference them for using the operator
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.Textures[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		EG_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			FlushAndReset();

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* subTexCoord = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		// looking for a empty slot and fill it
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*(s_Data.Textures[i].get()) == *(texture.get())) // get raw pointer and dereference them for using the operator
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.Textures[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = subTexCoord[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}



//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------   Customized draw funcs   ---------------------------------------------
//-----------------------------------------------------------------------------------------------------------------



	std::array<glm::vec2, 2> Renderer2D::ScreenCoordProcess(std::array<glm::vec2, 2>& screenCoord)
	{
		// y still in [-1, 1], x times aspectratio \ invalid type cast
		glm::vec2 leftLowPos = { (float)(k * (16.0f / 1280) * screenCoord[0].x - aspectRatio),
			(float)(-k * (9.0f / 720) * screenCoord[0].y + 1) };
		glm::vec2 rightHighPos = { (float)(k * (16.0f / 1280) * screenCoord[1].x - aspectRatio),
			(float)(-k * (9.0f / 720) * screenCoord[1].y + 1) };

		glm::vec2 position = { (leftLowPos.x + rightHighPos.x) / 2, (leftLowPos.y + rightHighPos.y) / 2 };
		glm::vec2 size = { rightHighPos.x - leftLowPos.x, rightHighPos.y - leftLowPos.y };

		return { position, size };
	}

	void Renderer2D::DrawCoord(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		std::array<glm::vec2, 2> data = {leftLowerCoord, rightHigherCoord};
		data = ScreenCoordProcess(data);

		DrawQuad(data[0], data[1], texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawCoord(const glm::vec2& leftLowerCoord, const glm::vec2& rightHigherCoord, const Ref<SubTexture2D>& subtexture, float tilingFactor, const glm::vec4& tintColor)
	{
		std::array<glm::vec2, 2> data = { leftLowerCoord, rightHigherCoord };
		data = ScreenCoordProcess(data);

		DrawQuad(data[0], data[1], subtexture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawSwitchButton(SwitchButton& button, const Ref<Texture2D>& buttonTex, const Ref<Texture2D>& optionTex, float tilingFactor, const glm::vec4& tintColor)
	{
		std::array<glm::vec2, 2>& data = ScreenCoordProcess(button.GetCoord());

		DrawQuad(data[0], data[1], buttonTex, tilingFactor, tintColor);
		// a quad demonstrating where is chosen.  Reference: DrawCoord
		std::array<glm::vec2, 2>& optionPos = button.GetOptionCoord();
		optionPos = ScreenCoordProcess(optionPos);
		DrawQuad(optionPos[0], optionPos[1], optionTex, tilingFactor, tintColor);

	}

	void Renderer2D::DrawSwitchButton(SwitchButton& button, const Ref<SubTexture2D>& optionTex, const Ref<SubTexture2D>& leftTex, const Ref<SubTexture2D>& rightTex, float tilingFactor, const glm::vec4& tintColor)
	{
		std::array<glm::vec2, 2>& data = button.GetCoord();

		std::array<glm::vec2, 2>& optionData = button.GetOptionCoord();
		std::array<glm::vec2, 2> leftTexData = { data[0], glm::vec2(optionData[0].x , optionData[1].y) };
		std::array<glm::vec2, 2> rightTexData = { glm::vec2(optionData[0].x, optionData[0].y), data[1] };
		rightTexData[0].x = rightTexData[0].x < data[1].x ? rightTexData[0].x : data[1].x;

		optionData = ScreenCoordProcess(optionData);
		leftTexData = ScreenCoordProcess(leftTexData);
		rightTexData = ScreenCoordProcess(rightTexData);

		DrawQuad(leftTexData[0], leftTexData[1], leftTex, 1.0f, tintColor);
		DrawQuad(rightTexData[0], rightTexData[1], rightTex, 1.0f, tintColor);
		DrawQuad(optionData[0], optionData[1], optionTex, 1.0f, tintColor);
	}



	//-----------------------------------------------------------------------------------------------------------------
	//------------------------------------------   Statistic funcs   --------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------------



	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}


}