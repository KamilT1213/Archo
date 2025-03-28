#pragma once
// UI rendering, use Demon Render but also use programmable vertex pulling
#include "DemonRenderer.hpp"
#include "core/log.hpp"

struct QuadVertex {
	glm::vec4 colour;
	glm::vec2 position;
	glm::vec2 texCoords;
	float texSlot;
	void TestMemory() {

		spdlog::info("QuadVertex\t\t\tsizeof {}\talignof {}", sizeof(QuadVertex), alignof(QuadVertex));

		spdlog::info("colour\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(QuadVertex::colour), alignof(glm::vec4), offsetof(QuadVertex, colour));
		spdlog::info("position\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(QuadVertex::position), alignof(glm::vec2), offsetof(QuadVertex, position));
		spdlog::info("texCoords\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(QuadVertex::texCoords), alignof(glm::vec2), offsetof(QuadVertex, texCoords));
		spdlog::info("texSlot\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(QuadVertex::texSlot), alignof(float), offsetof(QuadVertex, texSlot));
	}
};

struct CircleVertex {
	glm::vec4 colour;
	glm::vec2 position;
	glm::vec2 unitPosition;
	float thickness;

};
class UI
{
public:
	void init(const glm::ivec2& size);
	void onRender() const;
	void begin(); // Clear all quads, circles and rounded quads
	void end(); // Render all quads, circles and rounded quads drawn since begin
	void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour); // Draw a quad at position (top-left) with size (width and height)
	void drawTexturedQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture> texture); // Draw a quad at position (top-left) with size (width and height) and texture
	void drawCircle(const glm::vec2& centre, float radius, const glm::vec4& colour, float thickness); // Draw a circle at centre with radius;

	void TestMemory() {

		spdlog::info("UI\t\t\tsizeof {}\talignof {}", sizeof(UI), alignof(UI));

		spdlog::info("m_quadVertices\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_quadVertices), alignof(std::array<QuadVertex, m_maxQuadCount * 4>), offsetof(UI, m_quadVertices));

		spdlog::info("m_circleVertices\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_circleVertices), alignof(std::array<CircleVertex, m_maxCircleCount * 4>), offsetof(UI, m_circleVertices));

		spdlog::info("m_UIRenderer\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_UIRenderer), alignof(Renderer), offsetof(UI, m_UIRenderer));

		spdlog::info("slots\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::slots), alignof(std::array<int, 32>), offsetof(UI, slots));


		spdlog::info("m_UIScene\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_UIScene), alignof(std::shared_ptr<Scene>), offsetof(UI, m_UIScene));
		spdlog::info("m_quadsSSBO\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_quadsSSBO), alignof(std::shared_ptr<SSBO>), offsetof(UI, m_quadsSSBO));
		spdlog::info("m_circlesSSBO\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_circlesSSBO), alignof(std::shared_ptr<SSBO>), offsetof(UI, m_circlesSSBO));
		spdlog::info("m_defaultTexture\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_defaultTexture), alignof(std::shared_ptr<Texture>), offsetof(UI, m_defaultTexture));

		spdlog::info("m_defaultColour\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_defaultColour), alignof(const glm::vec4), offsetof(UI, m_defaultColour));
		spdlog::info("m_currentQuadCount\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_currentQuadCount), alignof(size_t), offsetof(UI, m_currentQuadCount));
		spdlog::info("m_currentCircleCount\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_currentCircleCount), alignof(size_t), offsetof(UI, m_currentCircleCount));
		spdlog::info("m_size\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_size), alignof(glm::ivec2), offsetof(UI, m_size));
		spdlog::info("m_quad\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_quad), alignof(entt::entity), offsetof(UI, m_quad));
		spdlog::info("m_circles\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(UI::m_circles), alignof(entt::entity), offsetof(UI, m_circles));


	}

private:

	static const int m_maxQuadCount = 256; // Maximum number of quads draws in the UI widget
	std::array<QuadVertex, m_maxQuadCount * 4> m_quadVertices; // Quad vertices, CPU side

	static const int m_maxCircleCount = 256; // Maximum number of circles draws in the UI widget
	std::array<CircleVertex, m_maxCircleCount * 4> m_circleVertices; // Circle vertices, CPU side

	Renderer m_UIRenderer; // Renderer to draw the scene

	std::array<int, 32> slots = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

	std::shared_ptr<Scene> m_UIScene; // Scene with three actors, quads, circles and rounded quads
	std::shared_ptr<SSBO> m_quadsSSBO; // SSBO to hold quad vertex data
	std::shared_ptr<SSBO> m_circlesSSBO; // SSBO to hold circle vertex data
	std::shared_ptr<Texture> m_defaultTexture; // Used for drawing an untextured quad

	const glm::vec4 m_defaultColour{ glm::vec4(1.f) }; //Used for drawing a textured quad
	size_t m_currentQuadCount{ 0 }; // Number of quads currently set to be rendered
	size_t m_currentCircleCount{ 0 }; // Number of circles currently set to be rendered
	glm::ivec2 m_size{ 0,0 }; // size of the UI;
	entt::entity m_quad{ entt::null };
	entt::entity m_circles{ entt::null };





};
