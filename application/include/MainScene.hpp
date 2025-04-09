#pragma once
#include <DemonRenderer.hpp>



class Archo : public Layer
{
public:
	Archo(GLFWWindowImpl& win);
	
private:
	void onImGUIRender() override;
	void onRender() override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void playGame();
	void mainSettings();
	void mainSave();
	void mainMenu();
	void pauseMenu();
	void pauseInventory();
	void pauseSettings();
	void exitGame();
	void saveGame();



	std::vector<entt::entity> m_Relics;
	std::shared_ptr<Scene> m_RelicScene;
	std::shared_ptr<Scene> m_screenScene;
	std::shared_ptr<Scene> m_mainMenu;
	std::shared_ptr<Scene> m_mainMenu_Settings;
	std::shared_ptr<Scene> m_mainMenu_Save;
	std::shared_ptr<Scene> m_pauseMenu;
	std::shared_ptr<Scene> m_pauseMenu_Settings;
	std::shared_ptr<Scene> m_pauseMenu_Inventory;

	entt::entity Quad;
	entt::entity AAQuad;
	entt::entity backgroundQuad;
	entt::entity MenuQuad;
	entt::entity PauseQuad;


	SoundManager m_soundManager;
	std::shared_ptr<Mix_Chunk> sound;

	float width = m_winRef.getWidthf();
	float height = m_winRef.getHeightf();

	glm::vec2 m_ScreenSize = glm::vec2(m_winRef.getSizef());
	glm::vec2 m_PointerPos = glm::vec2(m_winRef.getSizef() / 2.0f);
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
	bool Pausing{ false };
	int Relics{ 100 };
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
	size_t BackgroundPassIDx;
	size_t ButtonPassIDx;
	size_t PauseButtonPassIDx;
	size_t FinalPausePassIDx;
	size_t FinalMainMenuPassIDx;

	//std::shared_ptr<SSBO> terrainParticlesStartPoints;
	//std::shared_ptr<SSBO> terrainParticles;


	/*struct Particle {
		glm::vec4 Position;
		glm::vec2 UV;
	};*/

	Renderer m_mainRenderer;
	Renderer m_mainMenuRenderer;
	Renderer m_computeRenderer;
	Renderer m_backgroundRenderer;
	Renderer m_pausedRenderer;



	// Actor positions for ease of use and to avoid additonal magic numbers

	//size_t mainPassIdx;



	//Gui
	int Buffer{ 0 };

	FBOLayout backgroundPassLayout = {
		{AttachmentType::ColourHDR,true}
	};

	FBOLayout buttonPassLayout = {
		{AttachmentType::ColourHDR,true}
	};

	FBOLayout AAPassLayout = {
		{AttachmentType::ColourHDR,true}
	};

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