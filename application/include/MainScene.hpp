#pragma once
#include <DemonRenderer.hpp>
#include "core/saving.hpp"

enum class InteractionType { Digging, Extraction };
enum class InteractionState { Idle, InProgress };

struct SeedingPoint {
	glm::vec4 position = glm::vec4(-1.0f, -1.0f, -1.0f, 256.0f);
};

class Archo : public Layer
{
public:
	Archo(GLFWWindowImpl& win);
	
private:
	void onImGUIRender() override;
	void onRender() override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onFocus(WindowFocusEvent& e) override;
	void onLostFocus(WindowLostFocusEvent& e) override;
	void onResize(WindowResizeEvent& e) override;
	void onReset(GLFWWindowImpl& win) override;

	void createLayer();
	void resetLayer();

	void playGame();
	void mainSettings();
	void mainSave();
	void mainMenu();
	void pauseMenu();
	void settings_to_pauseMenu();
	void pauseInventory();
	void unpauseInventory();
	void pauseSettings();
	void pause_to_Game();
	void exitGame();
	void saveGame();
	void settings_to_mainMenu();
	void settings_to_Game();
	void saveAndExit();
	void deleteGameSave();
	void toggleFullscreen();
	void setupGenerator(Renderer& renderer, std::shared_ptr<Texture> target, std::shared_ptr<Texture> working, std::shared_ptr<Shader> shader);

	std::vector<SeedingPoint> getSeedingPoints();
	void placeRelics();
	void placeScenery();

	int seedingResolution = 32;
	std::shared_ptr<SSBO> m_seedingSSBO;
	std::vector<SeedingPoint> m_seedingPoints;

	std::shared_ptr<Material> m_seedingFinder;
	std::shared_ptr<Material> compute_GroundMaterial;
	std::vector<std::shared_ptr<Shader>> m_generators;

	std::vector<entt::entity> m_Relics;
	std::vector<entt::entity> m_Sceneries;

	std::shared_ptr<Scene> m_RelicScene;
	std::shared_ptr<Scene> m_screenScene;
	std::shared_ptr<Scene> m_mainMenu;
	std::shared_ptr<Scene> m_mainMenu_Settings;
	std::shared_ptr<Scene> m_mainMenu_Save;
	std::shared_ptr<Scene> m_gameMenu;
	std::shared_ptr<Scene> m_pauseMenu;
	std::shared_ptr<Scene> m_pauseMenu_Settings;
	std::shared_ptr<Scene> m_pauseMenu_Inventory;
	std::shared_ptr<Scene> m_SceneryScene;

	float initialRatio;

	entt::entity Quad;
	entt::entity finalQuad;
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
	bool focusMode{ true };
	bool modeToggle{ false };
	bool Reseting{ false };
	bool Pressed{ false };
	bool finished{ false };
	bool extrBegan{ false };
	bool Flip{ true };
	bool Fliping{ false };
	bool Pausing{ false };
	int Relics{ 32 };//512};
	int RemainingRelics{ 0 };
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
	Renderer m_generationRenderer;
	Renderer m_finalRenderer;
	Renderer m_seedingFinderRenderer;
	Renderer m_groundComputeRenderer;
	Renderer m_relicRenderer;
	Renderer m_sceneryRenderer;

	Settings_Save m_settings;
	Game_Save m_save;

	InteractionType m_interactionType;
	InteractionState m_interactionState;

	// Actor positions for ease of use and to avoid additonal magic numbers

	//size_t mainPassIdx;



	//Gui
	int Buffer{ 0 };


	FBOLayout defaultPassLayout = {
		{AttachmentType::ColourHDR,true}
	};

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

	FBOLayout sceneryPassLayout = {
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