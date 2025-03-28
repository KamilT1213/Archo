#pragma once
#include <DemonRenderer.hpp>

enum class GameState {intro, running, gameOver};

class Archo : public Layer
{
public:
	Archo(GLFWWindowImpl& win);
private:
	void onRender() override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;

	std::vector<entt::entity> m_Relics;
	std::shared_ptr<Scene> m_RelicScene;
	std::shared_ptr<Scene> m_screenScene;

	entt::entity Quad;
	entt::entity AAQuad;

	SoundManager m_soundManager;
	std::shared_ptr<Mix_Chunk> sound;

	float width = m_winRef.getWidthf();
	float height = m_winRef.getHeightf();

	glm::vec2 m_ScreenSize = glm::vec2(m_winRef.getSizef());
	glm::vec2 m_PointerPos = glm::vec2(0.5);
	glm::vec2 m_DigPos = glm::vec2(0.5);

	bool b{ true };
	bool focusMode{ false };
	bool modeToggle{ false };
	bool Reseting{ false };
	bool Pressed{ false };
	bool finished{ false };
	bool extrBegan{ false };
	bool Flip{ true };
	bool Fliping{ false };
	int Relics{ 100 };
	bool ActiveRelics[100];
	float ProgressSegmentTarget{ 0.0f };

	ImVec2 imageSize = ImVec2(width / 3, height / 3);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);

	glm::vec2 gameMouseLocation{ 0,0 };

	float ProgressBar{ 0.0f };
	float allTime{ 0.0f };
	float factor{ 1.0f };
	float ResetWave{ 1.0f };
	float RelicResetWave{ 1.0f };
	float RelicSetWave{ 0.0f };
	float Subby{ 0.6f };

	size_t GroundComputePassIDx;
	size_t GroundNormalComputePassIDx;
	size_t ScreenGroundPassIDx;
	size_t ScreenRelicPassIDx;
	size_t AAPassIDx;

	//std::shared_ptr<SSBO> terrainParticlesStartPoints;
	//std::shared_ptr<SSBO> terrainParticles;


	/*struct Particle {
		glm::vec4 Position;
		glm::vec2 UV;
	};*/

	Renderer m_mainRenderer;
	Renderer m_computeRenderer;



	// Actor positions for ease of use and to avoid additonal magic numbers

	//size_t mainPassIdx;



	//Gui
	int Buffer{ 0 };

	FBOLayout mainScreenPassLayout = {
		{AttachmentType::ColourHDR,true},
		{AttachmentType::ColourHDR,true}

	};

	FBOLayout relicScreenPassLayout = {
		{AttachmentType::ColourHDR,true},
		{AttachmentType::ColourHDR,true},
		{AttachmentType::Depth,true}

	};



	//FBOLayout typicalLayout = {
	//	{AttachmentType::ColourHDR, true},
	//	{AttachmentType::ColourHDR, true},
	//	{AttachmentType::ColourHDR, true},
	//	{AttachmentType::ColourHDR, true},
	//	{AttachmentType::ColourHDR, true},
	//	{AttachmentType::Depth, true}
	//};

};