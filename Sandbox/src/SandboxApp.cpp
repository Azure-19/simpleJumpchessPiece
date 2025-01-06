


//-------------------------		The Entry has shifted to GameApp	--------------------------------//
// 
// 
//=======================================================================================================
/*
#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

#include "Menu.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

Menu::Menu()
	: Layer("JUMPCHESS"), m_CameraController(1280.0f / 720.0f), m_StartButton({50, 700}, {250, 650})//m_StartButton({ -1.4f, -0.7f, 0.0f }, { 0.4f, 0.15f })
{
}

// testing codes before 2D Renderer
class ExampleLayer : public Engine::Layer
{
private:
	Engine::ShaderLibrary m_ShaderLibrary;					// on stack for only one would be used
	Engine::Ref<Engine::Shader> m_Shader;					// now just keep Ref since all api take in Ref
	Engine::Ref<Engine::VertexArray> m_VertexArray;

	Engine::Ref<Engine::Shader> m_FlatColorShader;
	Engine::Ref<Engine::VertexArray> m_SquareVA;

	Engine::Ref<Engine::Texture2D> m_Texture, m_Katana;

	Engine::OrthographicCameraController m_CameraController;// a 2D(mainly) camera
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };			// alpha locked as 0(controled by imgui)
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_VertexArray = (Engine::VertexArray::Create());				// create vertexArray and bind it automatically(7. 18)

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.7f, 0.9f, 0.1f, 1.0f,
		};

		Engine::Ref<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));	//create a vertexBuffer
		Engine::BufferLayout layout = {
			{ Engine::ShaderDataType::Vec3, "a_Position" },				// initialize list that visualize our layout
			{ Engine::ShaderDataType::Vec4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);			// also upload the layout, must set it first

		uint32_t indices[3] = { 0, 1, 2 };
		Engine::Ref<Engine::IndexBuffer> indexBuffer;
		indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = (Engine::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f
		};

		Engine::Ref<Engine::VertexBuffer> squareVB;
		squareVB = Engine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Engine::ShaderDataType::Vec3, "a_Position" },
			{ Engine::ShaderDataType::Vec2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine::Ref<Engine::IndexBuffer> squareIB;
		squareIB = Engine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Engine::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShadervertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderfragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";


		m_FlatColorShader = Engine::Shader::Create("FlatColor", flatColorShadervertexSrc, flatColorShaderfragmentSrc);
		// store
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = (Engine::Texture2D::Create("assets/textures/forTest/testTexture03.png"));
		m_Katana = (Engine::Texture2D::Create("assets/textures/forTest/testTexture05.png"));

		std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->UploadUniformInt1("u_Color", 0);
	}

	void OnUpdate(Engine::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.12f, 0.11f, 1.0f });
		Engine::RenderCommand::Clear();

		// real rernderer flow now
		Engine::Renderer::BeginScene(m_CameraController.GetCamera());
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));				// get a matrix that can scale a vec3 by 10%

			// cast it into that class to assess the two func of api-specific class
			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColorShader)->Bind();
			std::dynamic_pointer_cast<Engine::OpenGLShader>(m_FlatColorShader)->UploadUniformVec3("u_Color", m_SquareColor);

			for (int y = 0; y < 20; y++)
			{
				for (int x = 0; x < 20; x++)
				{
					glm::vec3 pos(x * 0.16f, y * 0.16f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;		// translate it into a mat4

					Engine::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
				}
			}
			// cool stuff using stored
			auto textureShader = m_ShaderLibrary.Get("Texture");
			
			// rescale it as the only transform
			m_Texture->Bind();
			Engine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));
			m_Katana->Bind();
			Engine::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));
			// Triangle
			// Engine::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Engine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Engine::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

};

class Sandbox : public Engine::Application
{
public:
	Sandbox()				// actually it has stolen the constructor from application class
	{
		// PushLayer(new ExampleLayer());
		//PushLayer(new Sandbox2D());
		PushLayer(new Menu());
	}
	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox;
}
*/