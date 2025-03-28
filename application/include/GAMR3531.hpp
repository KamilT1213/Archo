#pragma once
#include <DemonRenderer.hpp>
#include "include/ui.hpp"
#include "include/ImGui/bloomPanel.hpp"
#include "core/log.hpp"
#include "core/planeSweep.hpp"

enum class GameState {intro, running, gameOver};

class AsteriodBelt : public Layer
{
public:
	AsteriodBelt(GLFWWindowImpl& win);
private:
	void onRender() override;
	void onUpdate(float timestep) override;
	void onImGUIRender() override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void generateLevel();
	void generateLODs(std::shared_ptr<std::vector<std::shared_ptr<VAO>>> targetStorage, Model model, int levels = 4);
	void checkWaypointCollisions();
	void checkAsteroidCollisions();

	void TestMemory() {
		spdlog::info("AsteriodBelt\t\t\tsizeof {}\talignof {}", sizeof(AsteriodBelt), alignof(AsteriodBelt));

		spdlog::info("m_winRef\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_winRef), alignof(GLFWWindowImpl&), offsetof(AsteriodBelt, m_winRef));

		spdlog::info("m_mainScene\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_mainScene), alignof(std::shared_ptr<Scene>), offsetof(AsteriodBelt, m_mainScene));
		spdlog::info("m_screenScene\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_screenScene), alignof(std::shared_ptr<Scene>), offsetof(AsteriodBelt, m_screenScene));
		spdlog::info("m_bloomScenes\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_bloomScenes), alignof(std::vector<std::shared_ptr<Scene>>), offsetof(AsteriodBelt, m_bloomScenes));

		spdlog::info("m_mainRenderer\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_mainRenderer), alignof(Renderer), offsetof(AsteriodBelt, m_mainRenderer));
		spdlog::info("m_closeAsteroids\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_closeAsteroids), alignof(std::vector<glm::vec3>), offsetof(AsteriodBelt, m_closeAsteroids));
		spdlog::info("m_closeTargets\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_closeTargets), alignof(std::vector<glm::vec3>), offsetof(AsteriodBelt, m_closeTargets));

		spdlog::info("m_bloomPanel\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_bloomPanel), alignof(BloomPanel), offsetof(AsteriodBelt, m_bloomPanel));

		spdlog::info("m_ui\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_ui), alignof(UI), offsetof(AsteriodBelt, m_ui));

		spdlog::info("depthPrePassIdx\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::depthPrePassIdx), alignof(size_t), offsetof(AsteriodBelt, depthPrePassIdx));

		spdlog::info("camera\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::camera), alignof(entt::entity), offsetof(AsteriodBelt, camera));
		spdlog::info("ship\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::ship), alignof(entt::entity), offsetof(AsteriodBelt, ship));
		spdlog::info("nextTarget\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::nextTarget), alignof(entt::entity), offsetof(AsteriodBelt, nextTarget));
		spdlog::info("skyBox\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::skyBox), alignof(entt::entity), offsetof(AsteriodBelt, skyBox));

		spdlog::info("m_state\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_state), alignof(GameState), offsetof(AsteriodBelt, m_state));
		spdlog::info("speed\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::speed), alignof(float), offsetof(AsteriodBelt, speed));

		spdlog::info("m_introTexture\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_introTexture), alignof(std::shared_ptr<Texture>), offsetof(AsteriodBelt, m_introTexture));
		spdlog::info("m_gameOverTexture\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_gameOverTexture), alignof(std::shared_ptr<Texture>), offsetof(AsteriodBelt, m_gameOverTexture));
		spdlog::info("m_speedUIColours\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_speedUIColours), alignof(const std::array<glm::vec4, 16>), offsetof(AsteriodBelt, m_speedUIColours));

		spdlog::info("m_speedThresholds\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(AsteriodBelt::m_speedThresholds), alignof(const std::array<float, 15>), offsetof(AsteriodBelt, m_speedThresholds));

	};

private:
	UI m_ui; // Seperate user interface

	const std::array<glm::vec4, 16> m_speedUIColours = {
	glm::vec4(0.992156862745098f, 0.941176470588235f, 0.0117647058823529f, 0.85f),
	glm::vec4(0.984313725490196f, 0.894117647058824f, 0.0313725490196078f, 0.85f),
	glm::vec4(0.976470588235294f, 0.83921568627451f, 0.0549019607843137f, 0.85f),
	glm::vec4(0.972549019607843f, 0.788235294117647f, 0.0745098039215686f, 0.85f),
	glm::vec4(0.964705882352941f, 0.737254901960784f, 0.0941176470588235f, 0.85f),
	glm::vec4(0.956862745098039f, 0.686274509803922f, 0.113725490196078f, 0.85f),
	glm::vec4(0.949019607843137f, 0.631372549019608f, 0.137254901960784f, 0.85f),
	glm::vec4(0.941176470588235f, 0.580392156862745f, 0.156862745098039f, 0.85f),
	glm::vec4(0.933333333333333f, 0.529411764705882f, 0.176470588235294f, 0.85f),
	glm::vec4(0.933333333333333f, 0.462745098039216f, 0.180392156862745f, 0.85f),
	glm::vec4(0.945098039215686f, 0.337254901960784f, 0.133333333333333f, 0.85f),
	glm::vec4(0.96078431372549f, 0.203921568627451f, 0.0784313725490196f, 0.85f),
	glm::vec4(0.972549019607843f, 0.0784313725490196f, 0.0313725490196078f, 0.85f),
	glm::vec4(0.968627450980392f, 0.f,0.f, 0.85f),
	glm::vec4(0.937254901960784f, 0.f,0.f, 0.85f),
	glm::vec4(0.909803921568627f, 0.f,0.f, 0.85f)
	};

	Renderer m_mainRenderer;

	PlaneSweep collisionPlane{ glm::ivec2(1,1), glm::vec3(0,0,-1) };

	const std::array<float, 15> m_speedThresholds = {
		-0.82f,
		-1.14f,
		-1.46f,
		-1.78f,
		-2.1f,
		-2.42f,
		-2.74f,
		-3.06f,
		-3.38f,
		-3.7f,
		-4.02f,
		-4.34f,
		-4.66f,
		-4.98f,
		-5.3f
	};

	float speed{ 0.f };



	std::vector<std::shared_ptr<Scene>> m_bloomScenes;


	std::vector<glm::vec3> m_closeAsteroids; // Asteroids to be drawn in the UI (x,y) dist
	std::vector<glm::vec3> m_closeTargets; // Targets to be drawn in the UI (x,y) dist

	std::shared_ptr<Scene> m_mainScene;
	std::shared_ptr<Scene> m_screenScene; // Rename this!

	// ImGui panels
	BloomPanel m_bloomPanel = BloomPanel(m_mainRenderer);
	// Actor positions for ease of use and to avoid additonal magic numbers
	std::shared_ptr<Texture> m_introTexture{ nullptr };
	std::shared_ptr<Texture> m_gameOverTexture{ nullptr };

	size_t depthPrePassIdx;

	entt::entity camera{ entt::null };
	entt::entity ship{ entt::null };
	entt::entity nextTarget{ entt::null };
	entt::entity skyBox{ entt::null };


	GameState m_state{ GameState::intro };

	bool m_wireframe{ false };
};