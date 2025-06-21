#pragma once
#include <DemonRenderer.hpp>
#include "core/saving.hpp"

//Interaction states
enum class InteractionType { Digging, Extraction };
enum class InteractionState { Idle, InProgress };

//Used in finding seeding points
struct SeedingPoint {
	glm::vec4 position = glm::vec4(-1.0f, -1.0f, -1.0f, 128.0f);
};

//Used in simulating and rendering particles
struct ParticleData{
	glm::vec4 colour;
	glm::vec2 position;
	glm::vec2 direction;
	float size;
	float speed;
	float lifespan;
	float depth;
};

//Used in simulating particles
struct ParticleBehaviour{
	glm::vec2 target;
	float factor;
	int Mode;
};

//Used in tracking relics
struct Relic {
	bool Active;
	int Type;

};

//Used in rendering relics
struct RelicsBO {
	int Quantity{ 0 };
	int textureUnit;
	int xOffset;
	int yOffset;
};

//Used in terrain computing
struct DiggingSpot {
	glm::vec4 DigInfo{ 0,0,0,0 }; //Position: x, y | Radii | Depth
	float DigProgression{ 0 };// Progession of digging
	int DigMask{ 0 };// Pattern to use when digging
	float rotation{ 0 };
	int a{ 0 };// Reserved
};

//Used in tracking scenery
struct Scenery
{
	float DugOut = 0.0;
	int type = 0;
};

class Archo : public Layer
{
public:
	Archo(GLFWWindowImpl& win);

	//SSBO CPU Mirrors
	std::array<DiggingSpot, 16> m_digBOs;
	std::array<ParticleBehaviour, 8> m_particleTasks;

	//Extraction and digging parameters
	float Segments{ 0 };
	float timeToDig{ 1.0f };
	float timePerSegment{ 0.2f };
	float ProgressBar{ 0.0f };
	int DigSegments{ 3 };

	bool Pressed{ false };

	//Data layer sampled parameters
	int RelId{ -1 };
	int Rarity{ -1 };
	bool isScenery{ false };

	//Used for storing current relic being extracted
	entt::entity RelicEntity = entt::null;

	//Used for augmentation system to trigger effects
	bool RelicBeginTrigger{ false };
	bool RelicSegmentTrigger{ false };
	bool RelicFinishTrigger{ false };

	//Used for cases that need longer stretches of time being calculated
	float allTime{ 0.0f };

	//Utilized by augmentation system
	glm::vec2 RelicSceneryOffsetHold = glm::vec2(-1);

	float deltaTime{0.0f};

	glm::vec2 m_PointerPos = glm::vec2(m_winRef.getSizef() / 2.0f);
	glm::vec2 m_PointerPos_inGame = glm::vec2(0.5);
	glm::vec2 m_DigPos = glm::vec2(0.5);

	glm::vec2 m_relicZonePos;
	int m_relicLastTime{ 1 };

	std::shared_ptr<Scene> m_SceneryScene;
	std::vector<entt::entity> m_Sceneries;

	std::shared_ptr<Scene> m_RelicScene;
	std::vector<entt::entity> m_Relics;

	InteractionType m_interactionType;
	InteractionState m_interactionState;

	SoundManager m_soundManager;
	std::shared_ptr<Mix_Chunk> m_relicPingSound;
	int m_relicPingSound_channel{-1};

	Renderer m_sceneryRenderer;

	Game_Save m_save;
private:
	void onImGUIRender() override;
	void onRender() override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onFocus(WindowFocusEvent& e) override;
	void onLostFocus(WindowLostFocusEvent& e) override;
	void onResize(WindowResizeEvent& e) override;
	void onReset(GLFWWindowImpl& win) override;

	//Game Functions
	void createLayer();
	void UpdateParticleTasksSSBO();
	void ClearParticleTasksSSBO();
	void UpdateRelicsSSBO();
	void ClearRelicsSSBO();
	void UpdateDigSpotSSBO();
	void ClearDigSpotSSBO();
	void resetLayer();

	//Augmentation System
	void RefreshRelicFunctions();
	void RunRelicFunctions();

	//Menu button binding functions
	void playGame();				//Menu -> Game
	void mainSettings();			//Menu -> Settings
	void mainSave();				//Menu -> Save
	void mainMenu();				//* -> Menu
	void pauseMenu();				//* -> Pause
	void settings_to_pauseMenu();	//pSettings -> Pause
	void pauseInventory();			//* -> Inventory

	void equipToSlot1();			//[Relic Equip Slot 1]
	void equipToSlot2();			//[Relic Equip Slot 2]
	void equipToSlot3();			//[Relic Equip Slot 3]

	void unpauseInventory();		//Inventory -> Game
	void pauseSettings();			//Pause -> pSettings
	void pause_to_Game();			//Pause && * -> Game
	void exitGame();				//[Exit Game]
	void saveGame();				//[Save Game]
	void settings_to_mainMenu();	//Settings -> Menu
	void settings_to_Game();		//pSettings -> Game
	void saveAndExit();				//Pause -> Menu
	void deleteGameSave();			//[Delete Save]
	void toggleFullscreen();		//[Toggle fullscreen]

	//Island generation and setup functions
	void setupGenerator(Renderer& renderer, std::shared_ptr<Texture> target, std::shared_ptr<Texture> working, std::shared_ptr<Shader> shader);

	std::vector<SeedingPoint> getSeedingPoints();
	void placeRelics();
	void placeScenery();

	//Seeding GPU and CPU storage and size
	int seedingResolution = 64;
	std::shared_ptr<SSBO> m_seedingSSBO;
	std::vector<SeedingPoint> m_seedingPoints;

	//Relic Rendering outside of game
	std::shared_ptr<Material> m_gameInventoryMat;
	std::shared_ptr<Material> m_itemInventoryMat;

	//Compute materials and shaders
	std::shared_ptr<Material> m_seedingFinder;
	std::shared_ptr<Material> compute_GroundMaterial;
	std::vector<std::shared_ptr<Shader>> m_generators;

	//Track relics found outside game in displays
	std::vector<entt::entity> m_Relics2;

	//Inventory Grid size
	glm::vec2 InvGridSize{ 6,8 };

	//Debug for Rarity testing
	int LastRareity{ -1 };

	//Used for rendering relics outside game context
	std::shared_ptr<SSBO> m_relicsSSBO;
	std::shared_ptr<Texture> RelicTexture1;

	//Used for keeping track of digging and computing digging
	std::shared_ptr<SSBO> m_digSSBO;

	//Inventory slot materials and functions
	std::array<std::shared_ptr<Material>,3> slotsButtonMats;

	std::array<std::function<void()>, 3> RelicFunctions;

	//Particle Compute and render
	std::shared_ptr<SSBO> m_particles;
	std::shared_ptr<SSBO> m_particleBehaviour;
	std::shared_ptr<Material> m_particlesComputeMat;

	//Menu Scenes
	std::shared_ptr<Scene> m_InventoryButtonScene;
	std::shared_ptr<Scene> m_InventoryRelicDisplayScene;
	std::shared_ptr<Scene> m_screenScene;
	std::shared_ptr<Scene> m_mainMenu;
	std::shared_ptr<Scene> m_mainMenu_Settings;
	std::shared_ptr<Scene> m_mainMenu_Save;
	std::shared_ptr<Scene> m_gameMenu;
	std::shared_ptr<Scene> m_pauseMenu;
	std::shared_ptr<Scene> m_pauseMenu_Settings;
	std::shared_ptr<Scene> m_pauseMenu_Inventory;
	std::shared_ptr<Scene> m_particleScene;


	float initialRatio;

	//Entities of objects in key scenes
	entt::entity Quad;
	entt::entity finalQuad;
	entt::entity AAQuad;
	entt::entity backgroundQuad;
	entt::entity MenuQuad;
	entt::entity PauseQuad;
	entt::entity InventoryItemsQuad;

	//Sound Testing variables
	std::shared_ptr<Mix_Chunk> sound;
	int ExtractionSound = -1;

	//Size of screen
	float width = m_winRef.getWidthf();
	float height = m_winRef.getHeightf();

	glm::vec2 m_ScreenSize = glm::vec2(m_winRef.getSizef());



	bool b{ true };
	bool focusMode{ true };
	bool modeToggle{ false };
	bool Reseting{ false };
	
	bool finished{ false };
	bool extrBegan{ false };
	bool Flip{ true };
	bool Fliping{ false };
	bool Pausing{ false };
	bool beganInput { false};

	//Used for Relic equiping and placement
	int Relics{ 16 };//512};
	int RemainingRelics{ 0 };
	int invRelicSelected{ -1 };
	int Equiped[3]{ -1,-1,-1 };
	int Digspots = 1;

	//Used for Augmentation and sound triggers
	float ProgressSegmentTarget{ 0.0f };
	float ProgressSegmentTarget_RelicTrigger{ 0.0f };

	
	//Game Texture size
	glm::ivec2 TerrainSize{ 1024,1024 };

	ImVec2 imageSize = ImVec2(width / 3, height / 3);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);

	glm::vec2 gameMouseLocation{ 0,0 };


	//Custom reseting of level functionality (depricated)
	float factor{ 1.0f };
	float ResetWave{ 1.0f };
	float RelicResetWave{ 1.0f };
	float RelicSetWave{ 0.0f };
	float Subby{ 0.6f };

	//Pass Ids found in multi pass renderers
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

	size_t ParticlePassIDx;

	//std::shared_ptr<SSBO> terrainParticlesStartPoints;
	//std::shared_ptr<SSBO> terrainParticles;


	/*struct Particle {
		glm::vec4 Position;
		glm::vec2 UV;
	};*/

	//Renderers used for visuals and compute
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
	Renderer m_particleRenderer;

	//Settings Save File
	Settings_Save m_settings;




	// Actor positions for ease of use and to avoid additonal magic numbers

	//size_t mainPassIdx;



	//Gui
	int Buffer{ 0 };

	//Buffer layouts used in different passes
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

	FBOLayout particlePassLayout = {
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