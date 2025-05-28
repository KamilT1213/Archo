#include "MainScene.hpp"
#include "scripts/include/controller.hpp"
#include "scripts/include/button.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <numeric> // For std::iota
#include <string>
#include "scripts/RelicFunctions.hpp"

float DigCurve1(float t, float s, float o) {
	float d = glm::floor(t * s) / s;
	t = glm::mod(t * s, 1.0f);
	t = glm::pow(t, o);
	t /= s;
	t += d;
	return t;
}

float DigCurve2(float t, float s) {
	float d = glm::floor(t * s) / s;
	t = glm::mod(t * s, 1.0f);
	t = (-glm::sqrt(1.0f - glm::pow(t, 2.0f))) + 1.0f;
	t /= s;
	t += d;
	return t;
}

void Archo::UpdateParticleTasksSSBO()
{
	m_particleBehaviour->edit(0,sizeof(ParticleBehaviour) * m_particleTasks.size(), m_particleTasks.data());
}

void Archo::ClearParticleTasksSSBO()
{
	for(int i = 0; i < m_particleTasks.size(); i++){
		m_particleTasks[i] = ParticleBehaviour();
		//m_particleTasks[i].Mode = 1;
	}
	UpdateParticleTasksSSBO();
}

void Archo::UpdateRelicsSSBO()
{
    for (int i = 0; i < m_save.s_Items.size(); i++) {
		RelicsBO current;
		current.Quantity = m_save.s_Items.at(i).second;
		current.textureUnit = 0;
		current.xOffset = glm::floor(glm::mod(float(m_save.s_Items[i].first), 8.0f));
		current.yOffset = glm::floor(m_save.s_Items[i].first / 8.0f);
		m_relicsSSBO->edit(m_save.s_Items[i].first * sizeof(RelicsBO), sizeof(RelicsBO), &current);
	}
}

void Archo::ClearRelicsSSBO() {
	for (int i = 0; i < 48; i++) {
		RelicsBO current;
		current.Quantity = 0;
		current.textureUnit = 0;
		current.xOffset = glm::floor(glm::mod(float(i), 8.0f));
		current.yOffset = glm::floor(i / 8.0f);
		m_relicsSSBO->edit(i * sizeof(RelicsBO), sizeof(RelicsBO), &current);
	}
}

void Archo::UpdateDigSpotSSBO() {
	m_digSSBO->edit(0, m_digBOs.size() * sizeof(DiggingSpot), m_digBOs.data());
}
void Archo::ClearDigSpotSSBO(){
	m_digBOs = std::array<DiggingSpot,16>();
	m_digSSBO->edit(0, m_digBOs.size() * sizeof(DiggingSpot), m_digBOs.data());
}

Archo::Archo(GLFWWindowImpl& win) : Layer(win)
{

	spdlog::info("Support for multi image binding in compute: {}", glfwExtensionSupported("GL_ARB_shader_image_load_store"));
	int ver;
	int ver2;
	int ver3;
	glfwGetVersion(&ver, &ver2, &ver3);
	spdlog::info("OpenGL Version: {}.{}.{}", ver, ver2, ver3);

	GLint imgLim;
	glGetIntegerv(GL_MAX_IMAGE_UNITS, &imgLim);
	spdlog::info("Image Binding Units Limit: {}", imgLim);

	m_seedingSSBO = std::make_shared<SSBO>(sizeof(SeedingPoint) * (seedingResolution * seedingResolution), (seedingResolution * seedingResolution));
	m_seedingSSBO->bind(3);

	m_relicsSSBO = std::make_shared<SSBO>(sizeof(RelicsBO) * (48), (48));
	m_relicsSSBO->bind(4);
	for (int i = 0; i < 48; i++) {
		RelicsBO current;
		current.Quantity = 0;
		current.textureUnit = 0;
		current.xOffset = glm::floor(glm::mod(float(i), 8.0f));
		current.yOffset = glm::floor(i / 8.0f);
		m_relicsSSBO->edit(i * sizeof(RelicsBO), sizeof(RelicsBO), &current);
	}

	UpdateRelicsSSBO();

	m_digSSBO = std::make_shared<SSBO>(sizeof(DiggingSpot) * 16, 16);
	m_digSSBO->bind(5);

	UpdateDigSpotSSBO();

	m_particles = std::make_shared<SSBO>(sizeof(ParticleData) * (128), 128);
	m_particles->bind(6);

	ParticleData test;
	test.colour = glm::vec4(0,0,0,0);
	test.direction = glm::vec2(0.0);
	test.lifespan = 0.0f;
	test.position = glm::vec2(0.0f);
	test.depth = 0.0f;
	test.size = 0.0f;
	test.speed = 0.0f;

	m_particles->edit(0,sizeof(ParticleData),&test);

	m_particleBehaviour = std::make_shared<SSBO>(sizeof(ParticleBehaviour) * 8, 8);
	m_particleBehaviour->bind(7);
	ClearParticleTasksSSBO();
	m_particleTasks[0].Mode = 1;

	//m_seedingPoints = m_seedingSSBO->writeToCPU<SeedingPoint>();
	//for (int i = 0; i < m_seedingPoints.size(); i++) {
	//	m_seedingPoints[i] = SeedingPoint();
	//}
	m_seedingPoints.reserve((seedingResolution * seedingResolution));
	for (int i = 0; i < (seedingResolution * seedingResolution); i++) {
		m_seedingPoints.push_back(SeedingPoint());
	}

	m_seedingSSBO->edit(0, sizeof(SeedingPoint) * m_seedingPoints.size(), m_seedingPoints.data());
	//for (int i = 0; i < m_seedingPoints.size(); i++) {
	//	spdlog::info("Item: {},{},{},{} , At: {}", 
	//		m_seedingPoints[i].position.x, 
	//		m_seedingPoints[i].position.y, 
	//		m_seedingPoints[i].position.z, 
	//		m_seedingPoints[i].position.w, i);
	//}

	glEnable(GL_PROGRAM_POINT_SIZE);

	createLayer();
	RefreshRelicFunctions();
}

void Archo::onImGUIRender()
{

	// Scripts widgets
}

void Archo::onRender()
{
	
	m_backgroundRenderer.render();

	//m_relicRenderer.render();

	if (state == GameState::MainMenu) {
		m_mainMenuRenderer.render();
	}
	else if (state == GameState::InGame) {
		ZoneScoped;
		m_particleRenderer.render();
		m_mainRenderer.render();
		glDisable(GL_BLEND);
	}
	else if (state == GameState::Paused) {
		m_pausedRenderer.render();
	}

	m_finalRenderer.render();
}

void Archo::onUpdate(float timestep)
{

	if (m_winRef.doIsKeyPressed(GLFW_KEY_ESCAPE) && !Pausing) {
		if (state == GameState::InGame) {
			pauseMenu();
		}
		else if (state == GameState::Paused) {
			if (pauseState == PauseState::Pause) {
				pause_to_Game();
			}
			else if (pauseState == PauseState::Settings) {
				settings_to_pauseMenu();
			}
			else if (pauseState == PauseState::Inventory) {
				unpauseInventory();
			}
		}
		Pausing = true;
	}
	else if (!m_winRef.doIsKeyPressed(GLFW_KEY_ESCAPE) && Pausing) {
		Pausing = false;
	}

	//if (m_winRef.doIsKeyPressed(GLFW_KEY_TAB) && !modeToggle) {
	//	modeToggle = true;
	//	focusMode = !focusMode;
	//	m_winRef.doSwitchInput();
	//}
	//if (!m_winRef.doIsKeyPressed(GLFW_KEY_TAB) && modeToggle) {
	//	modeToggle = false;
	//}






	auto& backgroundPass = m_backgroundRenderer.getRenderPass(BackgroundPassIDx);
	auto& BackgroundQuad = backgroundPass.scene->m_entities.get<Render>(backgroundQuad).material;
	auto& generatePass = m_generationRenderer.getComputePass(0).material;

	if (focusMode) {
		m_PointerPos += (m_winRef.doGetMouseVector() * glm::vec2(1, -1)) / glm::min((float)backgroundPass.viewPort.width, (float)backgroundPass.viewPort.height) * 1000.0f * (1.0f / (m_settings.s_ResolutionFract * m_settings.s_ResolutionFract));
		m_PointerPos = glm::clamp((m_PointerPos), glm::vec2(0), glm::vec2((float)backgroundPass.viewPort.width, (float)backgroundPass.viewPort.height));
		//spdlog::info("Mouse Position: x:{}  y:{}", m_PointerPos.x, m_PointerPos.y);
	}

	allTime += timestep / 10.0f;

	if (allTime > 1000) {
		allTime = (allTime - 1000);
	}

	//spdlog::info("Relics Left: {}", RemainingRelics);
	//generatePass->setValue("Seed", allTime);

	BackgroundQuad->setValue("allTime", allTime);

	auto& FinalQuad = m_finalRenderer.getRenderPass(0).scene->m_entities.get<Render>(finalQuad).material;

	if (state == GameState::MainMenu) {
		FinalQuad->setValue("u_State", 0.0f);
		auto& menuPassMat = m_mainMenuRenderer.getRenderPass(FinalMainMenuPassIDx).scene->m_entities.get<Render>(MenuQuad).material;
		menuPassMat->setValue("u_mousePos", m_PointerPos);

		if (menuState == MenuState::Main) {
			auto view = m_mainMenu->m_entities.view<ScriptComp>();
			for (auto& entity : view) {
				ScriptComp script = view.get<ScriptComp>(entity);
				script.onUpdate(timestep);
			}
		}
		else if (menuState == MenuState::Settings) {
			auto view = m_mainMenu_Settings->m_entities.view<ScriptComp>();
			for (auto& entity : view) {
				ScriptComp script = view.get<ScriptComp>(entity);
				script.onUpdate(timestep);
			}
		}
		else if (menuState == MenuState::Save) {
			auto view = m_mainMenu_Save->m_entities.view<ScriptComp>();
			for (auto& entity : view) {
				ScriptComp script = view.get<ScriptComp>(entity);
				script.onUpdate(timestep);
			}
		}

	}
	else if (state == GameState::Paused) {
		FinalQuad->setValue("u_State", 2.0f);
		auto& pausePassMat = m_pausedRenderer.getRenderPass(FinalPausePassIDx).scene->m_entities.get<Render>(PauseQuad).material;
		pausePassMat->setValue("u_mousePos", m_PointerPos);

		if (pauseState == PauseState::Pause) {
			auto view = m_pauseMenu->m_entities.view<ScriptComp>();
			for (auto& entity : view) {
				ScriptComp script = view.get<ScriptComp>(entity);
				script.onUpdate(timestep);
			}
		}
		else if (pauseState == PauseState::Settings) {
			auto view = m_pauseMenu_Settings->m_entities.view<ScriptComp>();
			for (auto& entity : view) {
				ScriptComp script = view.get<ScriptComp>(entity);
				script.onUpdate(timestep);
			}
		}
		else if (pauseState == PauseState::Inventory) {

			
			//Inventory selection

			Transform& transComp = m_pauseMenu_Inventory->m_entities.get<Transform>(InventoryItemsQuad);

			glm::vec2 mousePos = m_PointerPos;
			mousePos.y = height - mousePos.y;
			std::pair<glm::vec2, glm::vec2> boundingBox;
			boundingBox.first = glm::vec2(transComp.translation - glm::abs(transComp.scale));
			boundingBox.second = glm::vec2(transComp.translation + glm::abs(transComp.scale));

			if (invRelicSelected >= 0) {
				m_itemInventoryMat->setValue("u_index", invRelicSelected);
			}

			//auto& matRef = m_scene->m_entities.get<Material>(m_entity);
			if (mousePos.x > boundingBox.first.x && mousePos.x < boundingBox.second.x && mousePos.y > boundingBox.first.y && mousePos.y < boundingBox.second.y) {

				mousePos -= boundingBox.first;
				mousePos /= glm::vec2(transComp.scale.x, transComp.scale.y) * 2.0f;
				mousePos *= InvGridSize;
				mousePos = glm::floor(mousePos);

				int index = mousePos.x + (mousePos.y * InvGridSize.x);

				
				m_itemInventoryMat->setValue("Hovered", 1.0f);
				m_itemInventoryMat->setValue("u_index2", index);

				if (m_winRef.doIsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {


					m_itemInventoryMat->setValue("Pressed", 1.0f);
					bool found = false;
					for (auto item : m_save.s_Items) {
						if (item.first == index) {
							found = true;
							break;
						}
					}

					if (index != invRelicSelected && found) {
						invRelicSelected = index;

						//spdlog::info("Index Selected: {}", index);
					}
					//spdlog::info("Button Active at; x:{} | y:{}", mousePos.x, mousePos.y);

				}
				else {
					m_itemInventoryMat->setValue("Pressed", 0.0f);
				}
			}
			else {
				m_itemInventoryMat->setValue("Hovered", 0.0f);
				m_itemInventoryMat->setValue("u_index2", -1);
				m_itemInventoryMat->setValue("Pressed", 0.0f);
			}

			auto view = m_pauseMenu_Inventory->m_entities.view<ScriptComp>();
			for (auto& entity : view) {
				ScriptComp script = view.get<ScriptComp>(entity);
				script.onUpdate(timestep);
			}

		}
	}
	else if (state == GameState::InGame) {

		deltaTime = timestep;
		m_particlesComputeMat->setValue("dt",timestep);
		m_particlesComputeMat->setValue("allTime",allTime);

		auto view = m_gameMenu->m_entities.view<ScriptComp>();
		for (auto& entity : view) {
			ScriptComp script = view.get<ScriptComp>(entity);
			script.onUpdate(timestep);
		}

		FinalQuad->setValue("u_State", 1.0f);
		ZoneScoped;

		m_gameInventoryMat->setValue("Time", allTime / 100.f);

		//auto& computeGroundPass = m_mainRenderer.getComputePass(GroundComputePassIDx);
		auto& relicPass = m_relicRenderer.getRenderPass(ScreenRelicPassIDx);
		auto& screenGroundPass = m_mainRenderer.getRenderPass(ScreenGroundPassIDx);
		auto& screenAAPass = m_mainRenderer.getRenderPass(AAPassIDx);

		auto& QuadMat = screenGroundPass.scene->m_entities.get<Render>(Quad).material;
		auto& AAQuadMat = screenAAPass.scene->m_entities.get<Render>(AAQuad).material;

		float sHeight, sWidth;
		sHeight = backgroundPass.viewPort.height;
		sWidth = backgroundPass.viewPort.width;


		m_PointerPos_inGame = m_PointerPos; //-(m_winRef.getSizef() * 0.5f);
		//temp /= glm::vec2(sHeight, sWidth);
		if (sHeight > sWidth) {
			float diff = (sHeight - sWidth) / 2.0f;
			m_PointerPos_inGame.y -= diff;
			m_PointerPos_inGame /= (glm::vec2(sWidth, sHeight) - glm::vec2(0, diff * 2.0f));
		}
		else if (sHeight < sWidth) {
			float diff = (sWidth - sHeight) / 2.0f;
			m_PointerPos_inGame.x -= diff;

			m_PointerPos_inGame /= (glm::vec2(sWidth, sHeight) - glm::vec2(diff * 2.0f, 0));
			//spdlog::info("MousePos: x:{} , y:{}", temp.x, temp.y);
		}
		m_PointerPos_inGame = glm::clamp(m_PointerPos_inGame, glm::vec2(0), glm::vec2(1));

		if (m_interactionState == InteractionState::Idle) {
			m_DigPos = m_PointerPos_inGame;

		}



		float x = ProgressBar;

		glm::vec2 temp = m_DigPos * glm::min(width, height);
		float a = glm::max(width, height) - glm::min(width, height);
		a /= 2;
		if (width > height) {
			temp.x += a;
		}
		else {
			temp.y += a;
		}
		//temp = m_DigPos;
		temp -= glm::clamp(temp, 0.01f, 0.99f);
		temp = glm::floor(temp);
		//temp = glm::clamp(temp, glm::vec2(0), glm::vec2(width, height));

		screenGroundPass.target->use();
		float UVData[4];
		glNamedFramebufferReadBuffer(screenGroundPass.target->getID(), screenGroundPass.target->m_colAttachments[1]);
		glReadPixels(temp.x, temp.y, 1, 1, GL_RGBA, GL_FLOAT, &UVData);

		//spdlog::info("Data Layer info: [0]:{} [1]:{} [2]:{} [3]:{}", UVData[0], UVData[1], UVData[2], UVData[3]);

		////for (int i = 0; i < 4; i++) {
		////	UVData[i] = glm::clamp(UVData[i], 0.0f, 1.0f);
		////}

		//gameMouseLocation = glm::vec2(UVData[0], UVData[1]);
		////spdlog::info("mouse x: {:03.2f}, mouse y: {:03.2f}", gameMouseLocation.x, gameMouseLocation.y);
		////spdlog::info("Relic id: {:03.5f}", glm::round(UVData[2] * (Relics + 1)));
		Segments = 0;
		timePerSegment = 0.2f;

		RelId = -1;
		Rarity = -1;
		isScenery = (bool)glm::round(UVData[1]);

		//m_digBOs[0].DigInfo = glm::vec4(0, 0, 25.0f, 0.10f);
		//m_digBOs[0].DigMask = 0;

		for (int i = 0; i <= Digspots; i++) {
			m_digBOs[i].DigInfo = glm::vec4(glm::vec2(m_digBOs[i].DigInfo), 25.0f, 0.10f);
			m_digBOs[i].DigMask = 0;
		}

		if(isScenery){
			RelId = (int)glm::round(UVData[2] * (m_Sceneries.size() + 1));
			Segments = 64;
		}
		else{
			RelId = (int)glm::round((UVData[2]) * (Relics + 1));
			Rarity = (int)glm::round(UVData[0] * 64.0f);
			//spdlog::info("Rarity: {}", UVData[0]);
			Segments = (Rarity + 1);//((Rarity + 1) * (Rarity + 1)) + 5.0f;
		}

		if (LastRareity != Rarity) {
			LastRareity = Rarity;
			//spdlog::info("Switched Rarity: {} at: x:{} , y:{}", LastRareity, temp.x, temp.y);
		}

		m_digBOs[0].DigInfo.x = m_DigPos.x;
		m_digBOs[0].DigInfo.y = m_DigPos.y;

		RunRelicFunctions();
		UpdateDigSpotSSBO();
		UpdateParticleTasksSSBO();


		//spdlog::info("Functions Active: {} | {} | {}", RelicFunctions[0] == NULL, RelicFunctions[1] == NULL, RelicFunctions[2] == NULL);
		//spdlog::info("Dig Spot Data: {} | {} | {} | {}", m_digBOs[0].DigInfo.x, m_digBOs[0].DigInfo.y, m_digBOs[0].DigInfo.z, m_digBOs[0].DigInfo.w);
	
		//spdlog::info("Rarity: {}", Rarity);


		//spdlog::info("ID: {}", RelId - 1);

		if (m_interactionState == InteractionState::Idle) {
			if (RelId != 0) m_interactionType = InteractionType::Extraction;
			else m_interactionType = InteractionType::Digging;
			//spdlog::info("Digging?: {}", m_interactionType == InteractionType::Digging);
		}


		RelicBeginTrigger = false;
		if (m_winRef.doIsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && UVData[3] > 0) {
			Pressed = true;
			if(m_interactionState == InteractionState::Idle) RelicBeginTrigger = true;

			m_particleTasks[0].target = glm::vec2(m_digBOs[0].DigInfo);
			m_particleTasks[0].factor = 1.0f / m_digBOs[0].DigInfo.z;
			m_particleTasks[0].Mode = 1;
		}
		else {

			m_particleTasks[0].Mode = 0;
			Pressed = false;
		}

		if (ProgressBar != 0) {
			m_interactionState = InteractionState::InProgress;
		}
		else {
			m_interactionState = InteractionState::Idle;
			beganInput = false;

		}

		bool LevelComplete = false;

		if (m_interactionType == InteractionType::Digging) {
			if (Pressed && UVData[3] > 0.0f) {

				compute_GroundMaterial->setValue("Mode", 1.0f);

				
				if (!finished) {
					Pressed = true;
					ProgressBar += timestep / timeToDig;
					if (ProgressBar > 1) {
						finished = true;
						ProgressBar = 1;
					}
					if ((RelId != 0)) {
						finished = true;

					}
					//m_digBOs[0].DigProgression = 1.0f - DigCurve1(ProgressBar, 3.0f, 10.0f);
					for (int i = 0; i < 16; i++) {
						m_digBOs[i].DigProgression = 1.0f - DigCurve1(ProgressBar, (float)DigSegments, 10.0f);
					}
					compute_GroundMaterial->setValue("Factor", ProgressBar);

				}
				else {


					finished = true;
					RelicFinishTrigger = true;
					compute_GroundMaterial->setValue("Mode", 1.5f);
					m_interactionType == InteractionType::Extraction;
					//m_computeRenderer.render();
					ProgressBar -= timestep * 5;
					if (ProgressBar < 0) {
						ProgressBar = 0;
						extrBegan = false;
						ProgressSegmentTarget = timeToDig / (float)DigSegments;
						ProgressSegmentTarget_RelicTrigger = timeToDig / (float)DigSegments;
						Pressed = false;
						finished = false;
						RelicFinishTrigger = false;
					}
				}

				
				//spdlog::info("Checking: {}",ProgressBar * timeToDig);
				if (ProgressBar * timeToDig >= ProgressSegmentTarget) {
					//spdlog::info("Check: {} is >= to {}",ProgressBar * timeToDig,ProgressSegmentTarget);
					//int r = rand() % 2;
					//int r2 = rand() % 4;

					//std::string s2 = "./assets/sounds/Extraction_soft_var";
					//s2 += char('0' + r2);
					//s2.append(".wav");

					//m_soundManager.playSound(s2.c_str());

					//std::string s = "./assets/sounds/Digging_soft_var";
					//s += char('0' + r);
					//s.append(".wav");



					//m_soundManager.playSound(s.c_str());
					//spdlog::info("PlaySound");
					//for (int i = 1; i < 16; i++) {
						//m_digBOs[i].DigInfo.x = Randomiser::uniformFloatBetween(0, 1);
						//m_digBOs[i].DigInfo.y = Randomiser::uniformFloatBetween(0, 1);
					//}
					//m_digBOs[0].rotation = Randomiser::uniformFloatBetween(-glm::pi<float>(), glm::pi<float>());
					//m_soundManager.playSound(sound.get(),0);
					//m_soundManager.playSoundAtPosition(sound.get(),256,glm::vec3(0,0,-1),glm::vec3(glm::vec3(0,0,-1)));
					//spdlog::info("Playing sound at {}",ProgressSegmentTarget);
					//spdlog::info("Dug Segment: {}",ProgressSegmentTarget);
					m_soundManager.playRandomFromLibrary(ExtractionSound);
					ProgressSegmentTarget += timeToDig / (float)DigSegments;
					RelicSegmentTrigger = true;
				}
			}
			else {
				compute_GroundMaterial->setValue("Mode", 1.5f);
				finished = true;
				//m_computeRenderer.render();
				ProgressBar -= timestep * 5;
				if (ProgressBar < 0) {
					ProgressBar = 0;
					extrBegan = false;
					ProgressSegmentTarget = timeToDig / (float)DigSegments;
					ProgressSegmentTarget_RelicTrigger = timeToDig / (float)DigSegments;
					Pressed = false;
					finished = false;
					RelicFinishTrigger = false;

				}
				//m_digBOs[0].DigProgression = 1.0f - DigCurve1(ProgressBar, 3.0f, 10.0f);
				for (int i = 0; i < 16; i++) {
					m_digBOs[i].DigProgression = 1.0f - DigCurve1(ProgressBar, 3.0f, 10.0f);
				}
			}
			UpdateDigSpotSSBO();
			m_groundComputeRenderer.render();
		}
		else if (m_interactionType == InteractionType::Extraction) {

			if (!extrBegan) {
				extrBegan = true;
				ProgressBar = 0;
			}

			if (Pressed) {
				if (!finished) {
					Pressed = true;
					if (isScenery && ProgressBar <= 0) {
						auto& sceneComp = m_SceneryScene->m_entities.get<Scenery>(m_Sceneries.at(RelId - 1));
						ProgressBar = sceneComp.DugOut;
						ProgressSegmentTarget = (ProgressBar * Segments) + 1;
						ProgressSegmentTarget_RelicTrigger = (ProgressBar * Segments) + 1;
					}

					ProgressBar += timestep * ((1 / timePerSegment) / Segments);

					if (isScenery) {
						auto& renderComp = m_SceneryScene->m_entities.get<Render>(m_Sceneries.at(RelId - 1));
						auto& sceneComp = m_SceneryScene->m_entities.get<Scenery>(m_Sceneries.at(RelId - 1));
						sceneComp.DugOut = glm::max(ProgressBar, sceneComp.DugOut);
						renderComp.material->setValue("u_DugOut", floor(ProgressBar* Segments) / Segments);
						m_sceneryRenderer.render();
					}


					if (ProgressBar > 1) {
						finished = true;
						ProgressBar = 1;

						if(isScenery){
							auto& renderComp = m_SceneryScene->m_entities.get<Render>(m_Sceneries.at(RelId - 1));
							auto& sceneComp = m_SceneryScene->m_entities.get<Scenery>(m_Sceneries.at(RelId - 1));
							sceneComp.DugOut = ProgressBar;
							renderComp.material->setValue("u_active", 0.0f);
							m_sceneryRenderer.render();
						}
						else{

							
							auto& relicComp = m_RelicScene->m_entities.get<Relic>(m_Relics.at(RelId - 1));
							auto& renderComp = m_RelicScene->m_entities.get<Render>(m_Relics.at(RelId - 1));
							renderComp.material->setValue("u_active", 0.0f);
							RelId = (Relics + 1) - RelId;
							auto& renderComp2 = m_InventoryButtonScene->m_entities.get<Render>(m_Relics2.at(RelId - 1));
							renderComp2.material->setValue("u_active", 0.0f);

							bool found = false;
							for(auto& item : m_save.s_Items){
								if(item.first == Rarity){
									item.second++;
									found = true;
									break;
								}
							}
							if(!found){
								m_save.s_Items.emplace_back(std::pair<int,int>(Rarity,1));
							}

							//spdlog::info("Picked Up: {} : {}", RelId, UVData[0]);

							saveGame();
							UpdateRelicsSSBO();
							RefreshRelicFunctions();
							m_relicRenderer.render();

							relicComp.Active = false;
							RemainingRelics--;
							if(RemainingRelics <= 0){
								m_generationRenderer.getComputePass(0).material->setValue("Seed", glm::mod(allTime, 1.0f));
								m_generationRenderer.render();
						
								placeRelics();
								placeScenery();

								compute_GroundMaterial->setValue("Mode",0.0f);
								m_groundComputeRenderer.render();
							}
						}
					}
				}
				else {
					finished = true;
					RelicFinishTrigger = true;
					ProgressBar -= timestep * 5;
					if (ProgressBar < 0) {
						extrBegan = false;
						ProgressBar = 0;
						ProgressSegmentTarget = 1;
						ProgressSegmentTarget_RelicTrigger = 1;
						Pressed = false;
						finished = false;
						RelicFinishTrigger = false;
					}
				}

				if (ProgressBar * Segments >= ProgressSegmentTarget) {

					// int r = rand() % 4;

					// std::string s = "./assets/sounds/Extraction_soft_var";
					// s += char('0' + r);
					// s.append(".wav");

					//m_soundManager.playSound(sound.get(),0);
					//m_soundManager.playSoundAtPosition(sound.get(),256,glm::vec3(0,0,-1),glm::vec3(glm::vec3(0,0,-1)));
					//spdlog::info("PlaySound");
					m_soundManager.playRandomFromLibrary(ExtractionSound);
					RelicSegmentTrigger = true;
					ProgressSegmentTarget++;
				}

				x = floor(ProgressBar * Segments) / Segments;
			}
			else {
				finished = true;
				//m_computeRenderer.render();
				ProgressBar -= timestep * 5;
				if (ProgressBar < 0) {
					ProgressBar = 0;
					extrBegan = false;
					ProgressSegmentTarget = 0.5;
					ProgressSegmentTarget_RelicTrigger = 0.5;
					Pressed = false;
					finished = false;
				}
			}

		}


		if (ProgressBar < 0) {
			ProgressBar = 0;
		}
		


		//QuadMat->setValue("MousePos", (m_PointerPos));
		QuadMat->setValue("DigPos", (m_DigPos));
		QuadMat->setValue("Progress", x);
		//QuadMat->setValue("DigStyle", glm::vec4(25.0f,0.05f,0.0f,0.0f));
		QuadMat->setValue("isDigging", (float)(int)(m_interactionType == InteractionType::Digging));
		//QuadMat->setValue("RelicFill", RelicSetWave);
		//QuadMat->setValue("u_flip", (float)(int)Flip);

		AAQuadMat->setValue("MousePos", m_PointerPos);
	}
}

void Archo::onKeyPressed(KeyPressedEvent& e)
{
	if (e.getKeyCode() == GLFW_KEY_R) {
		//spdlog::info("Seed: {}", glm::mod(allTime,1.0f));
		m_generationRenderer.getComputePass(0).material->setValue("Seed", glm::mod(allTime, 1.0f));
		m_generationRenderer.render();

		placeRelics();
		placeScenery();

		compute_GroundMaterial->setValue("Mode",0.0f);
		m_groundComputeRenderer.render();
	}
	if (e.getKeyCode() == GLFW_KEY_F11) {

		toggleFullscreen();
	}
	//if (e.getKeyCode() == GLFW_KEY_R) {

	//	state = GameState::Reset;
	//}
	/*if (e.getKeyCode() == GLFW_KEY_SPACE && m_state == GameState::intro) m_state = GameState::running;
	for (auto it = m_RelicScene->m_actors.begin(); it != m_RelicScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}*/
	//if (e.getKeyCode() == GLFW_KEY_O)
	//{
	//	if (m_wireframe) {
	//		m_wireframe = false;
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	}
	//	else {
	//		m_wireframe = true;
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	}
	//}
}

void Archo::onFocus(WindowFocusEvent& e)
{
	focusMode = true;
	//spdlog::info("Focused: {}", focusMode);
	m_winRef.doSwitchInputTo(false);
}

void Archo::onLostFocus(WindowLostFocusEvent& e)
{
	focusMode = false;
	//spdlog::info("Unfocused: {}",focusMode);
	m_winRef.doSwitchInputTo(true);
}

void Archo::onResize(WindowResizeEvent& e)
{

	m_winRef.m_Resizing = true;
	m_winRef.doSwitchInputTo(true);

	auto& pass = m_finalRenderer.getRenderPass(0);
	pass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	pass.camera.projection = glm::ortho(0.f, (float)e.getWidth(), (float)e.getHeight(), 0.f);
	pass.UBOmanager.setCachedValue("b_finalcamera2D", "u_finalprojection2D", pass.camera.projection);

	auto& trans = pass.scene->m_entities.get<Transform>(finalQuad);

	trans.scale = glm::vec3((float)e.getWidth(), (float)e.getHeight(), 1.0f);
	trans.recalc();

	/*m_mainRenderer.onResize(e,m_settings.s_ResolutionFract);
	m_mainMenuRenderer.onResize(e, m_settings.s_ResolutionFract);
	m_computeRenderer.onResize(e, m_settings.s_ResolutionFract);
	m_backgroundRenderer.onResize(e, m_settings.s_ResolutionFract);
	m_pausedRenderer.onResize(e, m_settings.s_ResolutionFract);
	m_generationRenderer.onResize(e, m_settings.s_ResolutionFract);
	m_finalRenderer.onResize(e, m_settings.s_ResolutionFract);

	auto& pausePass = m_pausedRenderer.getRenderPass(FinalPausePassIDx);
	auto& finalPass = m_finalRenderer.getRenderPass(0);
	auto& menuPass = m_mainMenuRenderer.getRenderPass(FinalMainMenuPassIDx);
	auto& groundPass = m_mainRenderer.getRenderPass(ScreenGroundPassIDx);
	auto& aaPass = m_mainRenderer.getRenderPass(AAPassIDx);
	auto& bgPass = m_backgroundRenderer.getRenderPass(BackgroundPassIDx);
	auto& pauseBtPass = m_pausedRenderer.getRenderPass(PauseButtonPassIDx);
	auto& mainBtPass = m_mainMenuRenderer.getRenderPass(ButtonPassIDx);
	auto& relicPass = m_mainRenderer.getRenderPass(ScreenRelicPassIDx);

	auto& pRenderMat = pausePass.scene->m_entities.get<Render>(PauseQuad).material;
	pRenderMat->setValue("u_background", bgPass.target->getTarget(0));
	pRenderMat->setValue("u_buttons", pauseBtPass.target->getTarget(0));
	pRenderMat->setValue("u_ScreenSize", glm::vec2(e.getSize()));

	auto& fRenderMat = finalPass.scene->m_entities.get<Render>(finalQuad).material;
	fRenderMat->setValue("u_PausedIn", pausePass.target->getTarget(0));
	fRenderMat->setValue("u_MainMenuIn", menuPass.target->getTarget(0));
	fRenderMat->setValue("u_InGameIn", aaPass.target->getTarget(0));

	auto& mmRenderMat = menuPass.scene->m_entities.get<Render>(MenuQuad).material;
	mmRenderMat->setValue("u_background", bgPass.target->getTarget(0));
	mmRenderMat->setValue("u_buttons", mainBtPass.target->getTarget(0));
	mmRenderMat->setValue("u_ScreenSize", glm::vec2(e.getSize()));

	auto& sRenderMat = groundPass.scene->m_entities.get<Render>(Quad).material;
	sRenderMat->setValue("u_RelicTexture", relicPass.target->getTarget(0));
	sRenderMat->setValue("u_RelicDataTexture", relicPass.target->getTarget(1));
	sRenderMat->setValue("u_ScreenSize", glm::vec2(e.getSize()));

	auto& aaRenderMat = aaPass.scene->m_entities.get<Render>(AAQuad).material;
	aaRenderMat->setValue("u_inputTexture", groundPass.target->getTarget(0));
	aaRenderMat->setValue("u_background", bgPass.target->getTarget(0));
	aaRenderMat->setValue("u_ScreenSize", glm::vec2(e.getSize()));*/


	//spdlog::info("resized");
	//m_mainRenderer.updateRenderAndDepthPassSize(e.getSize());
	//m_mainMenuRenderer.updateRenderAndDepthPassSize(e.getSize());
	//m_computeRenderer.updateRenderAndDepthPassSize(e.getSize());
	//m_backgroundRenderer.updateRenderAndDepthPassSize(e.getSize());
	//m_pausedRenderer.updateRenderAndDepthPassSize(e.getSize());
	//m_generationRenderer.updateRenderAndDepthPassSize(e.getSize());
}

void Archo::onReset(GLFWWindowImpl& win)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Unbind textures
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Unbind VAO, VBO, EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind framebuffer and shader program
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);


	state = GameState::MainMenu;
	resetLayer();
	createLayer();
}

void Archo::createLayer()
{
	m_settings = Load_Settings();
	m_save = Load_Game();

	const char* soundFile = "./assets/sounds/Extraction_soft_var0.wav";
	sound = (std::shared_ptr<Mix_Chunk>)Mix_LoadWAV(soundFile);
	if (!sound) {
		std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
		//Mix_CloseAudio();
		//SDL_Quit();
		//return;
	}

	soundFile = "./assets/sounds/Sonar_var0.wav";
	m_relicPingSound = (std::shared_ptr<Mix_Chunk>)Mix_LoadWAV(soundFile);
	if (!m_relicPingSound) {
		std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
		//Mix_CloseAudio();
		//SDL_Quit();
		//return;
	}

	soundFile = "./assets/sounds/Music_var0.wav";
	Mix_Music* music = Mix_LoadMUS(soundFile);
	if (!music) {
		std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
		//Mix_CloseAudio();
		//SDL_Quit();
		//return;
	}
	
	Mix_PlayMusic(music,-1);
	Mix_VolumeMusic(128);

	ExtractionSound = m_soundManager.addNewSoundsToLibrary("Extraction_soft",5);

	m_winRef.setVSync(false);
	m_RelicScene.reset(new Scene);
	m_InventoryButtonScene.reset(new Scene);
	m_screenScene.reset(new Scene);
	m_mainMenu.reset(new Scene);
	m_mainMenu_Settings.reset(new Scene);
	m_mainMenu_Save.reset(new Scene);
	m_gameMenu.reset(new Scene);
	m_pauseMenu.reset(new Scene);
	m_pauseMenu_Settings.reset(new Scene);
	m_pauseMenu_Inventory.reset(new Scene);
	m_SceneryScene.reset(new Scene);
	m_particleScene.reset(new Scene);

	initialRatio = width / height;



	//Textures -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<Texture> testRelicTexture = std::make_shared<Texture>("./assets/textures/Relic.png");
	std::shared_ptr<Texture> playButtonTexture = std::make_shared<Texture>("./assets/textures/UI/PlayButton.png");
	std::shared_ptr<Texture> exitButtonTexture = std::make_shared<Texture>("./assets/textures/UI/ExitButton.png");
	std::shared_ptr<Texture> saveButtonTexture = std::make_shared<Texture>("./assets/textures/UI/SaveButton.png");
	std::shared_ptr<Texture> deleteSaveButtonTexture = std::make_shared<Texture>("./assets/textures/UI/DeleteSaveButton.png");
	std::shared_ptr<Texture> settingsButtonTexture = std::make_shared<Texture>("./assets/textures/UI/SettingsButton.png");
	std::shared_ptr<Texture> scenery_ArchTexture = std::make_shared<Texture>("./assets/textures/Scenery/Arch/ArchTexture.png");
	std::shared_ptr<Texture> DigBrushMask = std::make_shared<Texture>("./assets/textures/Compute/DigMasks.png");

	RelicTexture1 = std::make_shared<Texture>("./assets/textures/Relics/RelicPack_1.png");

	TextureDescription groundTextureDesc;
	groundTextureDesc.height = TerrainSize.y;//1024.0f;//4096.0f / 2.0f;
	groundTextureDesc.width = TerrainSize.x;//1024.0f;//4096.0f / 2.0f;
	groundTextureDesc.channels = 4;
	groundTextureDesc.isHDR = true;

	std::shared_ptr<Texture> groundTexture = std::make_shared<Texture>(groundTextureDesc);
	std::shared_ptr<Texture> groundTextureTemp = std::make_shared<Texture>(groundTextureDesc);
	//std::shared_ptr<Texture> groundNormalTexture = std::make_shared<Texture>(groundTextureDesc);

	//Cube maps -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//Materials -------------------------------------------------------------------------------------------------------------------------------------------------------------

	//Level Generators
	ShaderDescription compute_Generator_Type1_ShaderDesc;
	compute_Generator_Type1_ShaderDesc.type = ShaderType::compute;
	compute_Generator_Type1_ShaderDesc.computeSrcPath = "./assets/shaders/compute_Generate_Type1.glsl";
	m_generators.push_back(std::make_shared<Shader>(compute_Generator_Type1_ShaderDesc));
		
	//Particle Compute
	ShaderDescription compute_Particle_ShaderDesc;
	compute_Particle_ShaderDesc.type = ShaderType::compute;
	compute_Particle_ShaderDesc.computeSrcPath = "./assets/shaders/compute_Particles.glsl";
	std::shared_ptr<Shader> Particle_computeShader = std::make_shared<Shader>(compute_Particle_ShaderDesc);
	m_particlesComputeMat = std::make_shared<Material>(Particle_computeShader);
	


	//Locate Seeding points
	ShaderDescription compute_FindSeedingLocations_ShaderDesc;
	compute_FindSeedingLocations_ShaderDesc.type = ShaderType::compute;
	compute_FindSeedingLocations_ShaderDesc.computeSrcPath = "./assets/shaders/compute_FindSeedingLocations.glsl";

	//m_generators.push_back(std::make_shared<Shader>(compute_FindSeedingLocations_ShaderDesc));
	m_seedingFinder = std::make_shared<Material>(std::make_shared<Shader>(compute_FindSeedingLocations_ShaderDesc));
	m_seedingFinder->setValue("Size", glm::vec2(groundTexture->getWidthf(), groundTexture->getHeightf()));

	//Final screen post processing pass (screen effects) material
	ShaderDescription screenQuadShaderDesc;
	screenQuadShaderDesc.type = ShaderType::rasterization;
	screenQuadShaderDesc.vertexSrcPath = "./assets/shaders/QuadVert.glsl";
	screenQuadShaderDesc.fragmentSrcPath = "./assets/shaders/QuadFrag.glsl";
	std::shared_ptr<Shader> screenQuadShader = std::make_shared<Shader>(screenQuadShaderDesc);
	std::shared_ptr<Material> screenQuadMaterial = std::make_shared<Material>(screenQuadShader);

	screenQuadMaterial->setValue("u_ScreenSize", m_ScreenSize);
	screenQuadMaterial->setValue("DigSpotTotal", Digspots);

	//Particle Material
	ShaderDescription particleShaderDesc;
	particleShaderDesc.type = ShaderType::rasterization;
	particleShaderDesc.vertexSrcPath = "./assets/shaders/ParticleVert.glsl";
	particleShaderDesc.fragmentSrcPath = "./assets/shaders/ParticleFrag.glsl";
	std::shared_ptr<Shader> particleShader = std::make_shared<Shader>(particleShaderDesc);
	std::shared_ptr<Material> particleMaterial = std::make_shared<Material>(particleShader);
	particleMaterial->setPrimitive(GL_POINTS);

	//Final main menu screen
	ShaderDescription menuQuadShaderDesc;
	menuQuadShaderDesc.type = ShaderType::rasterization;
	menuQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	menuQuadShaderDesc.fragmentSrcPath = "./assets/shaders/MainMenuFrag.glsl";
	std::shared_ptr<Shader> menuQuadShader = std::make_shared<Shader>(menuQuadShaderDesc);
	std::shared_ptr<Material> menuQuadMaterial = std::make_shared<Material>(menuQuadShader);

	menuQuadMaterial->setValue("u_ScreenSize", m_ScreenSize);


	//Pause menu screen
	ShaderDescription pauseMenuQuadShaderDesc;
	pauseMenuQuadShaderDesc.type = ShaderType::rasterization;
	pauseMenuQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	pauseMenuQuadShaderDesc.fragmentSrcPath = "./assets/shaders/PauseMenuFrag.glsl";
	std::shared_ptr<Shader> pauseMenuQuadShader = std::make_shared<Shader>(pauseMenuQuadShaderDesc);
	std::shared_ptr<Material> pauseMenuQuadMaterial = std::make_shared<Material>(pauseMenuQuadShader);

	pauseMenuQuadMaterial->setValue("u_ScreenSize", m_ScreenSize);


	//background screen pass mat
	ShaderDescription backgroundQuadShaderDesc;
	backgroundQuadShaderDesc.type = ShaderType::rasterization;
	backgroundQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	backgroundQuadShaderDesc.fragmentSrcPath = "./assets/shaders/BackGroundFrag.glsl";
	std::shared_ptr<Shader> backgroundQuadShader = std::make_shared<Shader>(backgroundQuadShaderDesc);
	std::shared_ptr<Material> backgroundQuadMaterial = std::make_shared<Material>(backgroundQuadShader);

	backgroundQuadMaterial->setValue("u_ScreenSize", m_ScreenSize);


	//AA screen pass mat
	ShaderDescription screenAAQuadShaderDesc;
	screenAAQuadShaderDesc.type = ShaderType::rasterization;
	screenAAQuadShaderDesc.vertexSrcPath = "./assets/shaders/QuadVert.glsl";
	screenAAQuadShaderDesc.fragmentSrcPath = "./assets/shaders/AAFrag.glsl";
	std::shared_ptr<Shader> screenAAQuadShader = std::make_shared<Shader>(screenAAQuadShaderDesc);
	std::shared_ptr<Material> screenAAQuadMaterial = std::make_shared<Material>(screenAAQuadShader);

	screenAAQuadMaterial->setValue("u_ScreenSize", m_ScreenSize);


	//final screen pass mat
	ShaderDescription finalQuadShaderDesc;
	finalQuadShaderDesc.type = ShaderType::rasterization;
	finalQuadShaderDesc.vertexSrcPath = "./assets/shaders/FinalVert.glsl";
	finalQuadShaderDesc.fragmentSrcPath = "./assets/shaders/FinalFrag.glsl";
	std::shared_ptr<Shader> finalQuadShader = std::make_shared<Shader>(finalQuadShaderDesc);
	std::shared_ptr<Material> finalQuadMaterial = std::make_shared<Material>(finalQuadShader);


	//Relic mat
	ShaderDescription RelicShaderDesc;
	RelicShaderDesc.type = ShaderType::rasterization;
	RelicShaderDesc.vertexSrcPath = "./assets/shaders/RelicVert.glsl";
	RelicShaderDesc.fragmentSrcPath = "./assets/shaders/RelicFrag.glsl";
	std::shared_ptr<Shader> RelicShader = std::make_shared<Shader>(RelicShaderDesc);

	ShaderDescription InvButRelicShaderDesc;
	InvButRelicShaderDesc.type = ShaderType::rasterization;
	InvButRelicShaderDesc.vertexSrcPath = "./assets/shaders/RelicVert.glsl";
	InvButRelicShaderDesc.fragmentSrcPath = "./assets/shaders/InvButRelicFrag.glsl";
	std::shared_ptr<Shader> InvButRelicShader = std::make_shared<Shader>(InvButRelicShaderDesc);

	ShaderDescription Scenery_ShaderDesc;
	Scenery_ShaderDesc.type = ShaderType::rasterization;
	Scenery_ShaderDesc.vertexSrcPath = "./assets/shaders/SceneryVert.glsl";
	Scenery_ShaderDesc.fragmentSrcPath = "./assets/shaders/SceneryFrag.glsl";
	std::shared_ptr<Shader> Scenery_Shader = std::make_shared<Shader>(Scenery_ShaderDesc);


	//Menu Button material
	ShaderDescription buttonQuadShaderDesc;
	buttonQuadShaderDesc.type = ShaderType::rasterization;
	buttonQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	buttonQuadShaderDesc.fragmentSrcPath = "./assets/shaders/UI/ButtonFrag.glsl";
	std::shared_ptr<Shader> buttonQuadShader = std::make_shared<Shader>(buttonQuadShaderDesc);	

	//Equip Button material
	ShaderDescription equipButtonQuadShaderDesc;
	equipButtonQuadShaderDesc.type = ShaderType::rasterization;
	equipButtonQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	equipButtonQuadShaderDesc.fragmentSrcPath = "./assets/shaders/UI/InventoryEquipFrag.glsl";
	std::shared_ptr<Shader> equipButtonQuadShader = std::make_shared<Shader>(equipButtonQuadShaderDesc);
	
	//Inventory Button material
	ShaderDescription invButtonQuadShaderDesc;
	invButtonQuadShaderDesc.type = ShaderType::rasterization;
	invButtonQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	invButtonQuadShaderDesc.fragmentSrcPath = "./assets/shaders/UI/InventoryButtonFrag.glsl";
	std::shared_ptr<Shader> invButtonQuadShader = std::make_shared<Shader>(invButtonQuadShaderDesc);	
	
	//Inventory Items material
	ShaderDescription invItemsQuadShaderDesc;
	invItemsQuadShaderDesc.type = ShaderType::rasterization;
	invItemsQuadShaderDesc.vertexSrcPath = "./assets/shaders/UI/ButtonVert.glsl";
	invItemsQuadShaderDesc.fragmentSrcPath = "./assets/shaders/UI/InventoryItemsFrag.glsl";
	std::shared_ptr<Shader> invItemsQuadShader = std::make_shared<Shader>(invItemsQuadShaderDesc);
	//std::shared_ptr<Material> buttonQuadMaterial = std::make_shared<Material>(buttonQuadShader);

	ShaderDescription compute_GroundShaderDesc;
	compute_GroundShaderDesc.type = ShaderType::compute;
	compute_GroundShaderDesc.computeSrcPath = "./assets/shaders/compute_Ground.glsl";
	std::shared_ptr<Shader> compute_GroundShader = std::make_shared<Shader>(compute_GroundShaderDesc);
	compute_GroundMaterial = std::make_shared<Material>(compute_GroundShader);
	compute_GroundMaterial->setValue("Size", glm::vec2(groundTexture->getWidthf(), groundTexture->getHeightf()));
	//compute_GroundMaterial->setValue("DigStyle", glm::vec4(25.0f,0.25f,0.0f,0.0f));
	compute_GroundMaterial->setValue("DigSpotTotal", Digspots);
	
	//m_digBOs[0].DigInfo = glm::vec4(0, 0, 25.0f, 0.25f);
	//m_digBOs[0].DigMask = 5;
	//for (int i = 1; i < 16; i++) {
	//	m_digBOs[i].DigInfo = glm::vec4(0, 0, Randomiser::uniformFloatBetween(25.f,15.f), 0.25f);
	//	m_digBOs[i].DigMask = Randomiser::uniformIntBetween(0,5);
	//}

	//ShaderDescription compute_GroundNormalShaderDesc;
	//compute_GroundNormalShaderDesc.type = ShaderType::compute;
	//compute_GroundNormalShaderDesc.computeSrcPath = "./assets/shaders/compute_CDMnormals.glsl";
	//std::shared_ptr<Shader> compute_GroundNormalShader = std::make_shared<Shader>(compute_GroundNormalShaderDesc);
	//std::shared_ptr<Material> compute_GroundNormalMaterial = std::make_shared<Material>(compute_GroundNormalShader);

	//VAOs ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//Button Quad VAO

	std::vector<float> buttonVertices = std::vector<float>{
		//Position               UV
		-1.0f, -1.0f,  0.0f,     0.0f, 1.0f, // Bottom Left
		1.0f,  -1.0f,  0.0f,     1.0f, 1.0f, // Bottom Right
		1.0f,   1.0f,  0.0f,     1.0f, 0.0f, // Top Right
		-1.0f,  1.0f,  0.0f,     0.0f, 0.0f  // Top Left
	};
	const std::vector<uint32_t> buttonIndices = { 0,1,2,2,3,0 };

	std::shared_ptr<VAO> buttonQuadVAO;
	buttonQuadVAO = std::make_shared<VAO>(buttonIndices);
	buttonQuadVAO->addVertexBuffer(buttonVertices, {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
		});

	//Screen Quad VAOs
	std::vector<float> screenVertices;
	std::vector<float> screenAAVertices;
	std::vector<float> FinalVertices;

	screenAAVertices = std::vector<float>{
		//Position               UV
	0.0f,  0.0f,   0.0f,     0.0f, 1.0f, // Bottom Left
	width, 0.0f,   0.0f,     1.0f, 1.0f, // Bottom Right
	width, height, 0.0f,     1.0f, 0.0f, // Top Right
	0.0f,  height, 0.0f,     0.0f, 0.0f  // Top Left
	};

	FinalVertices = std::vector<float>{
		//Position               UV
	0.0f,  0.0f,   0.0f,     0.0f, 1.0f, // Bottom Left
	1.0f, 0.0f,   0.0f,     1.0f, 1.0f, // Bottom Right
	1.0f, 1.0f, 0.0f,     1.0f, 0.0f, // Top Right
	0.0f,  1.0f, 0.0f,     0.0f, 0.0f  // Top Left
	};

	float a = glm::max(width, height);
	float b = glm::min(width, height);
	float c = (a - b) / 2;

	float widthOffset, heightOffset;
	if (width > height) {
		screenVertices = std::vector<float>{
			//Position               UV
		c,  0.0f,   0.0f,     0.0f, 1.0f, // Bottom Left
		width - c, 0.0f,   0.0f,     1.0f, 1.0f, // Bottom Right
		width - c, height, 0.0f,     1.0f, 0.0f, // Top Right
		c,  height, 0.0f,     0.0f, 0.0f  // Top Left
		};
	}
	else {
		screenVertices = std::vector<float>{
			//Position               UV
		0.0f,  c,   0.0f,     0.0f, 1.0f, // Bottom Left
		width, c,   0.0f,     1.0f, 1.0f, // Bottom Right
		width, height - c, 0.0f,     1.0f, 0.0f, // Top Right
		0.0f,  height - c, 0.0f,     0.0f, 0.0f  // Top Left
		};
	}



	const std::vector<uint32_t> screenIndices = { 0,1,2,2,3,0 };

	std::shared_ptr<VAO> screenQuadVAO;
	screenQuadVAO = std::make_shared<VAO>(screenIndices);
	screenQuadVAO->addVertexBuffer(screenVertices, {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
		});

	std::shared_ptr<VAO> screenAAQuadVAO;
	screenAAQuadVAO = std::make_shared<VAO>(screenIndices);
	screenAAQuadVAO->addVertexBuffer(screenAAVertices, {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
		});

	std::shared_ptr<VAO> FinalQuadVAO;
	FinalQuadVAO = std::make_shared<VAO>(screenIndices);
	FinalQuadVAO->addVertexBuffer(FinalVertices, {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
		});

	std::vector<float> RelicVertices;

	RelicVertices = std::vector<float>{
		//Position               UV
	-1.0f,  -1.0f,   0.0f,     0.0f, 1.0f, // Bottom Left
	 1.0f,  -1.0f,   0.0f,     1.0f, 1.0f, // Bottom Right
	 1.0f,   1.0f,   0.0f,     1.0f, 0.0f, // Top Right
	-1.0f,   1.0f,   0.0f,     0.0f, 0.0f  // Top Left
	};

	std::shared_ptr<VAO> RelicVAO;
	RelicVAO = std::make_shared<VAO>(screenIndices);
	RelicVAO->addVertexBuffer(RelicVertices, {
		{GL_FLOAT,3},  // position
		{GL_FLOAT,2}   // UV
	});


	VBOLayout archLayout = {
		{GL_FLOAT, 3},
		{GL_FLOAT, 3},
		{GL_FLOAT, 2}
	};

	uint32_t archAttributeTypes = Model::VertexFlags::positions |
	Model::VertexFlags::normals |
	Model::VertexFlags::uvs;
	//Arch
	Model archModel("./assets/models/Scenery/Arch/Arch1.obj", archAttributeTypes);
	std::shared_ptr<VAO> archVAO;
	archVAO = std::make_shared<VAO>(archModel.m_meshes[0].indices);
	archVAO->addVertexBuffer(archModel.m_meshes[0].vertices, archLayout);

	Model rockModel("./assets/models/Scenery/RockPile.obj", archAttributeTypes);
	std::shared_ptr<VAO> rockVAO;
	rockVAO = std::make_shared<VAO>(rockModel.m_meshes[0].indices);
	rockVAO->addVertexBuffer(rockModel.m_meshes[0].vertices, archLayout);

	//Actors ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	/*************************
	*  Main Menu Buttons
	**************************/

	float widthRatio = 1.0f / (width / height);

	{
		entt::entity startButton = m_mainMenu->m_entities.create();

		Render& renderComp = m_mainMenu->m_entities.emplace<Render>(startButton);
		Transform& transformComp = m_mainMenu->m_entities.emplace<Transform>(startButton);
		ScriptComp& scriptComp = m_mainMenu->m_entities.emplace<ScriptComp>(startButton);

		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> startButtonMat = std::make_shared<Material>(buttonQuadShader);
		startButtonMat->setValue("u_ButtonTexture", playButtonTexture);

		renderComp.material = startButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(-(width / 5.f) * widthRatio, 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::playGame, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(startButton, m_mainMenu, m_winRef, m_PointerPos, height, transformComp, *(startButtonMat.get()), boundFunc, m_soundManager,sound);
	}

	{
		entt::entity settingsButton = m_mainMenu->m_entities.create();

		Render& renderComp = m_mainMenu->m_entities.emplace<Render>(settingsButton);
		Transform& transformComp = m_mainMenu->m_entities.emplace<Transform>(settingsButton);
		ScriptComp& scriptComp = m_mainMenu->m_entities.emplace<ScriptComp>(settingsButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> settingsButtonMat = std::make_shared<Material>(buttonQuadShader);
		settingsButtonMat->setValue("u_ButtonTexture", settingsButtonTexture);

		renderComp.material = settingsButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::mainSettings, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(settingsButton, m_mainMenu, m_winRef, m_PointerPos, height, transformComp, *(settingsButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity saveButton = m_mainMenu->m_entities.create();

		Render& renderComp = m_mainMenu->m_entities.emplace<Render>(saveButton);
		Transform& transformComp = m_mainMenu->m_entities.emplace<Transform>(saveButton);
		ScriptComp& scriptComp = m_mainMenu->m_entities.emplace<ScriptComp>(saveButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> saveButtonMat = std::make_shared<Material>(buttonQuadShader);
		saveButtonMat->setValue("u_ButtonTexture", saveButtonTexture);

		renderComp.material = saveButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, -height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3((width / 5.f) * widthRatio, 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::mainSave, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(saveButton, m_mainMenu, m_winRef, m_PointerPos, height, transformComp, *(saveButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity exitButton = m_mainMenu->m_entities.create();

		Render& renderComp = m_mainMenu->m_entities.emplace<Render>(exitButton);
		Transform& transformComp = m_mainMenu->m_entities.emplace<Transform>(exitButton);
		ScriptComp& scriptComp = m_mainMenu->m_entities.emplace<ScriptComp>(exitButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
		exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = exitButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(0, height / 5.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::exitGame, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(exitButton, m_mainMenu, m_winRef, m_PointerPos, height, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	/*************************
	*  Main Menu Settings Buttons
	**************************/

	{
		entt::entity exitButton = m_mainMenu_Settings->m_entities.create();

		Render& renderComp = m_mainMenu_Settings->m_entities.emplace<Render>(exitButton);
		Transform& transformComp = m_mainMenu_Settings->m_entities.emplace<Transform>(exitButton);
		ScriptComp& scriptComp = m_mainMenu_Settings->m_entities.emplace<ScriptComp>(exitButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
		exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = exitButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f);// +glm::vec3(-(width / 5.f), 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::settings_to_mainMenu, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(exitButton, m_mainMenu_Settings, m_winRef, m_PointerPos, height, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	/*************************
	*  Main Menu Save Buttons
	**************************/

	{
		entt::entity deleteSaveButton = m_mainMenu_Save->m_entities.create();

		Render& renderComp = m_mainMenu_Save->m_entities.emplace<Render>(deleteSaveButton);
		Transform& transformComp = m_mainMenu_Save->m_entities.emplace<Transform>(deleteSaveButton);
		ScriptComp& scriptComp = m_mainMenu_Save->m_entities.emplace<ScriptComp>(deleteSaveButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> deleteSaveButtonMat = std::make_shared<Material>(buttonQuadShader);
		deleteSaveButtonMat->setValue("u_ButtonTexture", deleteSaveButtonTexture);

		renderComp.material = deleteSaveButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, -height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(-(width / 5.f) * widthRatio, 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::deleteGameSave, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(deleteSaveButton, m_mainMenu_Save, m_winRef, m_PointerPos, height, transformComp, *(deleteSaveButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity exitButton = m_mainMenu_Save->m_entities.create();

		Render& renderComp = m_mainMenu_Save->m_entities.emplace<Render>(exitButton);
		Transform& transformComp = m_mainMenu_Save->m_entities.emplace<Transform>(exitButton);
		ScriptComp& scriptComp = m_mainMenu_Save->m_entities.emplace<ScriptComp>(exitButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
		exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = exitButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3((width / 5.f) * widthRatio, 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::mainMenu, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(exitButton, m_mainMenu_Save, m_winRef, m_PointerPos, height, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	/*************************
	*  Game Menu Buttons
	**************************/

	{
		entt::entity inventoryButton = m_gameMenu->m_entities.create();

		Render& renderComp = m_gameMenu->m_entities.emplace<Render>(inventoryButton);
		Transform& transformComp = m_gameMenu->m_entities.emplace<Transform>(inventoryButton);
		ScriptComp& scriptComp = m_gameMenu->m_entities.emplace<ScriptComp>(inventoryButton);


		renderComp.geometry = buttonQuadVAO;

		m_gameInventoryMat = std::make_shared<Material>(invButtonQuadShader);
		//m_gameInventoryMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = m_gameInventoryMat;

		transformComp.scale = glm::vec3(c / 2.0f, c / 2.0f, 1.f);
		transformComp.translation = glm::vec3(width - (c / 2.0f), c / 2.0f, 0.f);// +glm::vec3(0, height / 5.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::pauseInventory, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(inventoryButton, m_gameMenu, m_winRef, m_PointerPos, height, transformComp, *(m_gameInventoryMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity PauseButton = m_gameMenu->m_entities.create();

		Render& renderComp = m_gameMenu->m_entities.emplace<Render>(PauseButton);
		Transform& transformComp = m_gameMenu->m_entities.emplace<Transform>(PauseButton);
		ScriptComp& scriptComp = m_gameMenu->m_entities.emplace<ScriptComp>(PauseButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
		exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = exitButtonMat;

		transformComp.scale = glm::vec3(c/2.0f, c/2.0f, 1.f);
		transformComp.translation = glm::vec3(c / 2.0f, c / 2.0f, 0.f);// +glm::vec3(0, height / 5.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::pauseMenu, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(PauseButton, m_gameMenu, m_winRef, m_PointerPos, height, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	/*************************
	*  Pause Menu Buttons
	**************************/

	{
		entt::entity startButton = m_pauseMenu->m_entities.create();

		Render& renderComp = m_pauseMenu->m_entities.emplace<Render>(startButton);
		Transform& transformComp = m_pauseMenu->m_entities.emplace<Transform>(startButton);
		ScriptComp& scriptComp = m_pauseMenu->m_entities.emplace<ScriptComp>(startButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> startButtonMat = std::make_shared<Material>(buttonQuadShader);
		startButtonMat->setValue("u_ButtonTexture", playButtonTexture);

		renderComp.material = startButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(-(width / 5.f) * widthRatio, 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::pause_to_Game, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(startButton, m_pauseMenu, m_winRef, m_PointerPos, height, transformComp, *(startButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity settingsButton = m_pauseMenu->m_entities.create();

		Render& renderComp = m_pauseMenu->m_entities.emplace<Render>(settingsButton);
		Transform& transformComp = m_pauseMenu->m_entities.emplace<Transform>(settingsButton);
		ScriptComp& scriptComp = m_pauseMenu->m_entities.emplace<ScriptComp>(settingsButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> settingsButtonMat = std::make_shared<Material>(buttonQuadShader);
		settingsButtonMat->setValue("u_ButtonTexture", settingsButtonTexture);

		renderComp.material = settingsButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::pauseSettings, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(settingsButton, m_pauseMenu, m_winRef, m_PointerPos, height, transformComp, *(settingsButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity saveButton = m_pauseMenu->m_entities.create();

		Render& renderComp = m_pauseMenu->m_entities.emplace<Render>(saveButton);
		Transform& transformComp = m_pauseMenu->m_entities.emplace<Transform>(saveButton);
		ScriptComp& scriptComp = m_pauseMenu->m_entities.emplace<ScriptComp>(saveButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> saveButtonMat = std::make_shared<Material>(buttonQuadShader);
		saveButtonMat->setValue("u_ButtonTexture", saveButtonTexture);

		renderComp.material = saveButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, -height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3((width / 5.f) * widthRatio, 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::saveGame, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(saveButton, m_pauseMenu, m_winRef, m_PointerPos, height, transformComp, *(saveButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity saveAndExitButton = m_pauseMenu->m_entities.create();

		Render& renderComp = m_pauseMenu->m_entities.emplace<Render>(saveAndExitButton);
		Transform& transformComp = m_pauseMenu->m_entities.emplace<Transform>(saveAndExitButton);
		ScriptComp& scriptComp = m_pauseMenu->m_entities.emplace<ScriptComp>(saveAndExitButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> saveAndExitButtonMat = std::make_shared<Material>(buttonQuadShader);
		saveAndExitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = saveAndExitButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(0, height / 5.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::saveAndExit, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(saveAndExitButton, m_pauseMenu, m_winRef, m_PointerPos, height, transformComp, *(saveAndExitButtonMat.get()), boundFunc, m_soundManager, sound);
	}


	//{
	//	entt::entity exitButton = m_pauseMenu->m_entities.create();

	//	Render& renderComp = m_pauseMenu->m_entities.emplace<Render>(exitButton);
	//	Transform& transformComp = m_pauseMenu->m_entities.emplace<Transform>(exitButton);
	//	ScriptComp& scriptComp = m_pauseMenu->m_entities.emplace<ScriptComp>(exitButton);


	//	renderComp.geometry = buttonQuadVAO;

	//	std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
	//	exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

	//	renderComp.material = exitButtonMat;

	//	transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
	//	transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(0, height / 5.f, 0);

	//	transformComp.recalc();

	//	std::function<void()> boundFunc = std::bind(&Archo::playGame, this);
	//	//boundFunc();
	//	scriptComp.attachScript<ButtonScript>(exitButton, m_pauseMenu, m_winRef, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	//}

	/*************************
	*  Pause Menu Settings Buttons
	**************************/

	{
		entt::entity exitButton = m_pauseMenu_Settings->m_entities.create();

		Render& renderComp = m_pauseMenu_Settings->m_entities.emplace<Render>(exitButton);
		Transform& transformComp = m_pauseMenu_Settings->m_entities.emplace<Transform>(exitButton);
		ScriptComp& scriptComp = m_pauseMenu_Settings->m_entities.emplace<ScriptComp>(exitButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
		exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = exitButtonMat;

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f);// +glm::vec3(-(width / 5.f), 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::settings_to_pauseMenu, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(exitButton, m_pauseMenu_Settings, m_winRef, m_PointerPos, height, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	/*************************
	*  Pause Menu Inventory Buttons
	**************************/

	{
		entt::entity exitButton = m_pauseMenu_Inventory->m_entities.create();

		Render& renderComp = m_pauseMenu_Inventory->m_entities.emplace<Render>(exitButton);
		Transform& transformComp = m_pauseMenu_Inventory->m_entities.emplace<Transform>(exitButton);
		ScriptComp& scriptComp = m_pauseMenu_Inventory->m_entities.emplace<ScriptComp>(exitButton);


		renderComp.geometry = buttonQuadVAO;

		std::shared_ptr<Material> exitButtonMat = std::make_shared<Material>(buttonQuadShader);
		exitButtonMat->setValue("u_ButtonTexture", exitButtonTexture);

		renderComp.material = exitButtonMat;

		transformComp.scale = glm::vec3(c / 2.0f, c / 2.0f, 1.f);
		transformComp.translation = glm::vec3(width - (c / 2.0f), c / 2.0f, 0.f);// +glm::vec3(-(width / 5.f), 0, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::unpauseInventory, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(exitButton, m_pauseMenu_Inventory, m_winRef, m_PointerPos, height, transformComp, *(exitButtonMat.get()), boundFunc, m_soundManager, sound);
	}

	{
		InventoryItemsQuad = m_pauseMenu_Inventory->m_entities.create();

		Render& renderComp = m_pauseMenu_Inventory->m_entities.emplace<Render>(InventoryItemsQuad);
		Transform& transformComp = m_pauseMenu_Inventory->m_entities.emplace<Transform>(InventoryItemsQuad);
		
		renderComp.geometry = buttonQuadVAO;

		m_itemInventoryMat = std::make_shared<Material>(invItemsQuadShader);
		m_itemInventoryMat->setValue("u_Texture", RelicTexture1);
		m_itemInventoryMat->setValue("u_GridSize", InvGridSize);
		m_itemInventoryMat->setValue("Hovered", 0.0f);
		m_itemInventoryMat->setValue("u_index", -1);
		m_itemInventoryMat->setValue("u_index2", -1);
		m_itemInventoryMat->setValue("Pressed", 0.0f);

		renderComp.material = m_itemInventoryMat;

		transformComp.scale = glm::vec3((width / 10.f * 3.0f) * widthRatio, height / 10.f * 4.0f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(0, height / 5.f * -0.5f, 0);

		transformComp.recalc();

	}

	{
		entt::entity slot1Button = m_pauseMenu_Inventory->m_entities.create();

		Render& renderComp = m_pauseMenu_Inventory->m_entities.emplace<Render>(slot1Button);
		Transform& transformComp = m_pauseMenu_Inventory->m_entities.emplace<Transform>(slot1Button);
		ScriptComp& scriptComp = m_pauseMenu_Inventory->m_entities.emplace<ScriptComp>(slot1Button);


		renderComp.geometry = buttonQuadVAO;

		slotsButtonMats[0] = std::make_shared<Material>(equipButtonQuadShader);
		slotsButtonMats[0]->setValue("u_Texture", RelicTexture1);
		slotsButtonMats[0]->setValue("u_index", m_save.s_Equiped[0]);

		renderComp.material = slotsButtonMats[0];

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(-(width / 5.f) * widthRatio, height / 5.f * 2.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::equipToSlot1, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(slot1Button, m_pauseMenu_Inventory, m_winRef, m_PointerPos, height, transformComp, *(slotsButtonMats[0].get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity slot2Button = m_pauseMenu_Inventory->m_entities.create();

		Render& renderComp = m_pauseMenu_Inventory->m_entities.emplace<Render>(slot2Button);
		Transform& transformComp = m_pauseMenu_Inventory->m_entities.emplace<Transform>(slot2Button);
		ScriptComp& scriptComp = m_pauseMenu_Inventory->m_entities.emplace<ScriptComp>(slot2Button);


		renderComp.geometry = buttonQuadVAO;

		slotsButtonMats[1] = std::make_shared<Material>(equipButtonQuadShader);
		slotsButtonMats[1]->setValue("u_Texture", RelicTexture1);
		slotsButtonMats[1]->setValue("u_index", m_save.s_Equiped[1]);
		renderComp.material = slotsButtonMats[1];

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3(0, height / 5.f * 2.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::equipToSlot2, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(slot2Button, m_pauseMenu_Inventory, m_winRef, m_PointerPos, height, transformComp, *(slotsButtonMats[1].get()), boundFunc, m_soundManager, sound);
	}

	{
		entt::entity slot3Button = m_pauseMenu_Inventory->m_entities.create();

		Render& renderComp = m_pauseMenu_Inventory->m_entities.emplace<Render>(slot3Button);
		Transform& transformComp = m_pauseMenu_Inventory->m_entities.emplace<Transform>(slot3Button);
		ScriptComp& scriptComp = m_pauseMenu_Inventory->m_entities.emplace<ScriptComp>(slot3Button);


		renderComp.geometry = buttonQuadVAO;

		slotsButtonMats[2] = std::make_shared<Material>(equipButtonQuadShader);
		slotsButtonMats[2]->setValue("u_Texture", RelicTexture1);
		slotsButtonMats[2]->setValue("u_index", m_save.s_Equiped[2]);
		renderComp.material = slotsButtonMats[2];

		transformComp.scale = glm::vec3((width / 10.f) * widthRatio, height / 10.f, 1.f);
		transformComp.translation = glm::vec3(width / 2.f, height / 2.f, 0.f) + glm::vec3((width / 5.f) * widthRatio, height / 5.f * 2.f, 0);

		transformComp.recalc();

		std::function<void()> boundFunc = std::bind(&Archo::equipToSlot3, this);
		//boundFunc();
		scriptComp.attachScript<ButtonScript>(slot3Button, m_pauseMenu_Inventory, m_winRef, m_PointerPos, height, transformComp, *(slotsButtonMats[2].get()), boundFunc, m_soundManager, sound);
	}

	/*************************
	*  Relics
	**************************/

	//int Curve[6]{ 13,8,5,3,2,1 };
	int Curve[6]{ 6,4,2,2,1,1 };
	std::array<std::pair<int, int>, 6> Ranges{ std::pair<int,int>(0,15), std::pair<int,int>(16,26), std::pair<int,int>(27,34), std::pair<int,int>(35,40), std::pair<int,int>(41,44), std::pair<int,int>(45,47) };



	for (int i = 0; i < Relics; i++) {
		
		float rarity = 0;
		int counter = 0;
		for (int j = 0; j < std::size(Curve); j++) {
			counter += Curve[j];
			if (i < counter) {
				rarity = Randomiser::uniformIntBetween(Ranges[j].first, Ranges[j].second);;
				//spdlog::info("Added of rarity: {}", j);
				break;
			}
		}

		entt::entity relic = m_RelicScene->m_entities.create();
		m_Relics.push_back(relic);
		//m_RelicScene->m_entities.emplace<Relic>(relic);
		Render& renderComp = m_RelicScene->m_entities.emplace<Render>(relic);
		Transform& transformComp = m_RelicScene->m_entities.emplace<Transform>(relic);
		Relic& relicComp = m_RelicScene->m_entities.emplace<Relic>(relic);
		renderComp.geometry = RelicVAO;

		relicComp.Active = true;

		std::shared_ptr<Material> RelicMaterial = std::make_shared<Material>(RelicShader);
		RelicMaterial->setValue("u_RelicTexture", RelicTexture1);
		RelicMaterial->setValue("u_Rarity", rarity + 0.001f);
		RelicMaterial->setValue("u_Id", (float)(i + 1) / (Relics + 1.0f));
		RelicMaterial->setValue("u_active", (float)(int)false);

		transformComp.scale = glm::vec3(Randomiser::uniformFloatBetween(100.0f, 150.0f) * ((0.5f * ((7.0f - (1 + rarity)) / 7.0f)) + 0.5f));

		transformComp.translation = glm::vec3(Randomiser::uniformFloatBetween(transformComp.scale.x, 1024.0f - transformComp.scale.x), Randomiser::uniformFloatBetween(transformComp.scale.y, 1024.0f - transformComp.scale.y), (1.0f - (rarity / 6.0f)) - 0.5f);


		renderComp.material = RelicMaterial;

		transformComp.recalc();

		entt::entity progressRelic = m_InventoryButtonScene->m_entities.create();
		m_Relics2.push_back(progressRelic);
		//m_Relics.push_back(relic);
		Render& renderComp2 = m_InventoryButtonScene->m_entities.emplace<Render>(progressRelic);
		Transform& transformComp2 = m_InventoryButtonScene->m_entities.emplace<Transform>(progressRelic);
		Relic& relicComp2 = m_InventoryButtonScene->m_entities.emplace<Relic>(progressRelic);
		renderComp2.geometry = RelicVAO;
		std::shared_ptr<Material> RelicMaterial2 = std::make_shared<Material>(InvButRelicShader);
		RelicMaterial2->setValue("u_RelicTexture", RelicTexture1);
		RelicMaterial2->setValue("u_Rarity", rarity + 0.001f);
		//RelicMaterial2->setValue("u_Id", -1.0f);
		RelicMaterial2->setValue("u_active", (float)(int)true);
		transformComp2.translation = glm::vec3(glm::mod((float)i, 4.f), floorf(i / 4.f), -0.5f);
		transformComp2.translation += glm::vec3(0.5f, 0.5f, 0);
		transformComp2.translation /= glm::vec3(4.f, 4.f, 1);
		transformComp2.translation *= glm::vec3(1024.0f , 1024.0f, 1);

		transformComp2.scale = glm::vec3((1024.0f / 8.0f) - 1.0f);

		renderComp2.material = RelicMaterial2;
		transformComp2.recalc();
	}

	/*************************
	*  Scenery
	**************************/

	for(int i = 0; i < 10; i++){
		entt::entity arch = m_SceneryScene->m_entities.create();
		m_Sceneries.push_back(arch);

		Render& renderComp = m_SceneryScene->m_entities.emplace<Render>(arch);
		Transform& transformComp = m_SceneryScene->m_entities.emplace<Transform>(arch);
		Scenery& sceneComp = m_SceneryScene->m_entities.emplace<Scenery>(arch);

		sceneComp.type = 0;

		renderComp.geometry = archVAO;
		renderComp.depthGeometry = archVAO;

		std::shared_ptr<Material> archmat = std::make_shared<Material>(Scenery_Shader);
		archmat->setValue("u_SceneryTexture",scenery_ArchTexture);
		archmat->setValue("u_Id",(float)(i + 1) / (30 + 1.0f));
		archmat->setValue("u_active",1.f);

		renderComp.material = archmat;
		renderComp.depthMaterial = archmat;

		float r = Randomiser::uniformFloatBetween(-0.52359878f, 0.52359878f);
		transformComp.rotation = glm::vec3(0,0,r);

		float s = Randomiser::uniformFloatBetween(100.0f, 300.0f);
		transformComp.scale = glm::vec3(s,s,1.0f);
		//transformComp.rotation = glm::quat(glm::vec3(Randomiser::uniformFloatBetween(-3.14159f, 3.14159f),Randomiser::uniformFloatBetween(-3.14159f, 3.14159f),0.0f));
		transformComp.translation = glm::vec3(Randomiser::uniformFloatBetween(transformComp.scale.x, TerrainSize.x - transformComp.scale.x), Randomiser::uniformFloatBetween(transformComp.scale.y, TerrainSize.y - transformComp.scale.y), -1.0f);
		transformComp.recalc();

	}	
	
	for(int i = 0; i < 20; i++){
		entt::entity rock = m_SceneryScene->m_entities.create();
		m_Sceneries.push_back(rock);

		Render& renderComp = m_SceneryScene->m_entities.emplace<Render>(rock);
		Transform& transformComp = m_SceneryScene->m_entities.emplace<Transform>(rock);
		Scenery& sceneComp = m_SceneryScene->m_entities.emplace<Scenery>(rock);

		sceneComp.type = 1;

		renderComp.geometry = rockVAO;
		renderComp.depthGeometry = rockVAO;

		std::shared_ptr<Material> archmat = std::make_shared<Material>(Scenery_Shader);
		archmat->setValue("u_SceneryTexture",scenery_ArchTexture);
		archmat->setValue("u_Id",(float)(10 + i + 1) / (30 + 1.0f));
		archmat->setValue("u_active",1.f);

		renderComp.material = archmat;
		renderComp.depthMaterial = archmat;

		float r = Randomiser::uniformFloatBetween(-0.52359878f, 0.52359878f);
		transformComp.rotation = glm::vec3(0,0,r);

		float s = Randomiser::uniformFloatBetween(100.0f, 150.0f);
		transformComp.scale = glm::vec3(s,s,1.0f);
		//transformComp.rotation = glm::quat(glm::vec3(Randomiser::uniformFloatBetween(-3.14159f, 3.14159f),Randomiser::uniformFloatBetween(-3.14159f, 3.14159f),0.0f));
		transformComp.translation = glm::vec3(Randomiser::uniformFloatBetween(transformComp.scale.x, TerrainSize.x - transformComp.scale.x), Randomiser::uniformFloatBetween(transformComp.scale.y, TerrainSize.y - transformComp.scale.y), -1.0f);
		transformComp.recalc();

	}

	for (int i = 0; i < m_save.s_Items.size(); i++) {
		RelicsBO current;
		current.Quantity = m_save.s_Items.at(i).second;
		current.textureUnit = RelicTexture1->getUnit();
		current.xOffset = glm::floor(glm::mod(float(m_save.s_Items[i].first), 8.0f));
		current.yOffset = glm::floor(m_save.s_Items[i].first / 8.0f);
		m_relicsSSBO->edit(m_save.s_Items[i].first * sizeof(RelicsBO), sizeof(RelicsBO), &current);
	}

	//Particles

	{
		entt::entity particle = m_particleScene->m_entities.create();
		Render& renderComp = m_particleScene->m_entities.emplace<Render>(particle);
		Transform& transformComp = m_particleScene->m_entities.emplace<Transform>(particle);

		//renderComp.geometry = RelicVAO;
		//renderComp.depthGeometry = RelicVAO;
		renderComp.SSBOgeometry = m_particles;

		renderComp.material = particleMaterial;

		transformComp.translation = glm::vec3(0,0,-0.5f);
		transformComp.scale = glm::vec3(1 ,1,1);
		transformComp.recalc();
	}



	//Particles
	//Actor particles;

	//uint32_t numberOfParticles = 4096 * 4;

	//terrainParticlesStartPoints = std::make_shared<SSBO>(sizeof(Particle) * numberOfParticles, numberOfParticles);
	//terrainParticlesStartPoints->bind(0);
	//terrainParticles = std::make_shared<SSBO>(sizeof(Particle) * numberOfParticles, numberOfParticles);
	//terrainParticles->bind(1);
	////ParticleMaterial->setValue("particles", terrainParticles->getID());

	//particles.SSBOgeometry = terrainParticlesStartPoints;
	//particles.material = ParticleMaterial;
	////particles.SSBOdepthGeometry = terrainParticlesStartPoints;
	////particles.depthMaterial = ParticleMaterial;
	////particles.translation = glm::vec3(-5, -3, -5);
	//particles.recalc();
	//particlesIdx = m_mainScene->m_actors.size();
	//m_mainScene->m_actors.push_back(particles);

	//Main Camera

	//Actor camera;
	//cameraIdx = m_mainScene->m_actors.size();
	//m_mainScene->m_actors.push_back(camera);

	/*************************
	*  Compute Pass
	**************************/

	ComputePass ParticleComputePass;
	ParticleComputePass.material = m_particlesComputeMat;
	ParticleComputePass.workgroups = {8,1,1};
	ParticleComputePass.barrier = MemoryBarrier::All;

	m_particleRenderer.addComputePass(ParticleComputePass);

	setupGenerator(m_generationRenderer, groundTexture, groundTextureTemp, m_generators.at(0));

	ComputePass SeedingComputePass;
	SeedingComputePass.material = m_seedingFinder;
	SeedingComputePass.workgroups = { TerrainSize.x / 16,TerrainSize.y / 16,1 };// { seedingResolution, seedingResolution, 1 };
	SeedingComputePass.barrier = MemoryBarrier::ShaderImageAccess;

	Image InImg;
	InImg.mipLevel = 0;
	InImg.layered = false;
	InImg.texture = groundTexture;
	InImg.imageUnit = 0;
	InImg.access = TextureAccess::ReadOnly;

	SeedingComputePass.images.push_back(InImg);

	m_seedingFinderRenderer.addComputePass(SeedingComputePass);

	//Terrain:

	//Terrain Normals Compute Pass
	ComputePass GroundComputePass;
	GroundComputePass.material = compute_GroundMaterial;
	GroundComputePass.workgroups = { glm::max(groundTexture->getWidth() / 32, (unsigned)1),glm::max(groundTexture->getHeight() / 32, (unsigned)1),1 };
	GroundComputePass.barrier = MemoryBarrier::ShaderImageAccess;

	Image GroundImg;
	GroundImg.mipLevel = 0;
	GroundImg.layered = false;
	GroundImg.texture = groundTexture;
	GroundImg.imageUnit = 0;
	GroundImg.access = TextureAccess::ReadWrite;

	Image GroundImgTemp;
	GroundImgTemp.mipLevel = 0;
	GroundImgTemp.layered = false;
	GroundImgTemp.texture = groundTextureTemp;
	GroundImgTemp.imageUnit = 1;
	GroundImgTemp.access = TextureAccess::ReadWrite;

	Image DigBrushMaskImg;
	DigBrushMaskImg.mipLevel = 0;
	DigBrushMaskImg.layered = false;
	DigBrushMaskImg.texture = DigBrushMask;
	DigBrushMaskImg.imageUnit = 2;
	DigBrushMaskImg.access = TextureAccess::ReadWrite;

	

	GroundComputePass.images.push_back(GroundImg);
	GroundComputePass.images.push_back(GroundImgTemp);
	GroundComputePass.images.push_back(DigBrushMaskImg);

	GroundComputePassIDx = m_groundComputeRenderer.getSumPassCount();
	m_groundComputeRenderer.addComputePass(GroundComputePass);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ComputePass GroundNormalComputePass;
	//GroundNormalComputePass.material = compute_GroundNormalMaterial;
	//GroundNormalComputePass.workgroups = { 128,128,1 };
	//GroundNormalComputePass.barrier = MemoryBarrier::ShaderImageAccess;

	//Image GroundNormalImg;
	//GroundNormalImg.mipLevel = 0;
	//GroundNormalImg.layered = false;
	//GroundNormalImg.texture = groundNormalTexture;
	//GroundNormalImg.imageUnit = GroundNormalComputePass.material->m_shader->m_imageBindingPoints["groundNormalImg"];
	//GroundNormalImg.access = TextureAccess::ReadWrite;

	//GroundNormalComputePass.images.push_back(GroundImg);
	//GroundNormalComputePass.images.push_back(GroundNormalImg);

	//GroundNormalComputePassIDx = m_computeRenderer.getSumPassCount();
	//m_computeRenderer.addComputePass(GroundNormalComputePass);
	//m_computeRenderer.render();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






	//AAquad.translation = glm::vec3(500.9f,0,0);
	//AAquad.scale = glm::vec3(2);
	//AAquad.recalc();

	/*************************
	*  Background Render Pass
	**************************/

	m_screenScene.reset(new Scene);

	backgroundQuad = m_screenScene->m_entities.create();
	{
		Render& renderComp = m_screenScene->m_entities.emplace<Render>(backgroundQuad);
		m_screenScene->m_entities.emplace<Transform>(backgroundQuad);
		renderComp.geometry = screenAAQuadVAO;
		renderComp.material = backgroundQuadMaterial;
	}

	RenderPass BackgroundPass;
	BackgroundPass.scene = m_screenScene;
	BackgroundPass.parseScene();
	BackgroundPass.target = std::make_shared<FBO>(m_winRef.doGetSize(), backgroundPassLayout);
	BackgroundPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	BackgroundPass.isScreen = true;

	BackgroundPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	BackgroundPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", BackgroundPass.camera.view);
	BackgroundPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", BackgroundPass.camera.projection);

	BackgroundPassIDx = m_backgroundRenderer.getSumPassCount();
	m_backgroundRenderer.addRenderPass(BackgroundPass);

	/*************************
	*  Screen Game Button Render Pass
	**************************/

	RenderPass ScreenGameButtonPass;
	ScreenGameButtonPass.scene = m_gameMenu;
	ScreenGameButtonPass.parseScene();
	ScreenGameButtonPass.target = std::make_shared<FBO>(m_winRef.doGetSize(), buttonPassLayout);
	ScreenGameButtonPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	ScreenGameButtonPass.isScreen = true;

	ScreenGameButtonPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	ScreenGameButtonPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuView", ScreenGameButtonPass.camera.view);
	ScreenGameButtonPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuProjection", ScreenGameButtonPass.camera.projection);


	//ScreenGroundPassIDx = m_mainRenderer.getSumPassCount();
	m_mainRenderer.addRenderPass(ScreenGameButtonPass);

	/*************************
	*  Pause Menu Render Pass
	**************************/

	RenderPass PauseButtonPass;
	PauseButtonPass.scene = m_pauseMenu;
	PauseButtonPass.parseScene();
	PauseButtonPass.target = std::make_shared<FBO>(m_winRef.doGetSize(), buttonPassLayout);
	PauseButtonPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	PauseButtonPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	PauseButtonPass.isScreen = true;

	PauseButtonPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuView", PauseButtonPass.camera.view);
	PauseButtonPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuProjection", PauseButtonPass.camera.projection);

	PauseButtonPassIDx = m_pausedRenderer.getSumPassCount();

	m_pausedRenderer.addRenderPass(PauseButtonPass);


	m_screenScene.reset(new Scene);

	pauseMenuQuadMaterial->setValue("u_background", BackgroundPass.target->getTarget(0));
	pauseMenuQuadMaterial->setValue("u_buttons", PauseButtonPass.target->getTarget(0));

	PauseQuad = m_screenScene->m_entities.create();
	{
		Render& renderComp = m_screenScene->m_entities.emplace<Render>(PauseQuad);
		m_screenScene->m_entities.emplace<Transform>(PauseQuad);

		renderComp.geometry = screenAAQuadVAO;
		renderComp.material = pauseMenuQuadMaterial;
	}

	RenderPass PauseMenuPass;
	PauseMenuPass.scene = m_screenScene;
	PauseMenuPass.parseScene();
	PauseMenuPass.target = std::make_shared<FBO>(m_winRef.getSize(), defaultPassLayout);
	PauseMenuPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	PauseMenuPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	PauseMenuPass.isScreen = true;

	PauseMenuPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuView", PauseMenuPass.camera.view);
	PauseMenuPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuProjection", PauseMenuPass.camera.projection);

	FinalPausePassIDx = m_pausedRenderer.getSumPassCount();

	m_pausedRenderer.addRenderPass(PauseMenuPass);


	finalQuadMaterial->setValue("u_PausedIn", PauseMenuPass.target->getTarget(0));

	/*************************
	*  Main Menu Render Pass
	**************************/

	RenderPass ButtonPass;
	ButtonPass.scene = m_mainMenu;
	ButtonPass.parseScene();
	ButtonPass.target = std::make_shared<FBO>(m_winRef.doGetSize(), buttonPassLayout);
	ButtonPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	ButtonPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	ButtonPass.isScreen = true;

	ButtonPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuView", ButtonPass.camera.view);
	ButtonPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuProjection", ButtonPass.camera.projection);

	ButtonPassIDx = m_mainMenuRenderer.getSumPassCount();

	m_mainMenuRenderer.addRenderPass(ButtonPass);


	m_screenScene.reset(new Scene);


	menuQuadMaterial->setValue("u_background", BackgroundPass.target->getTarget(0));
	menuQuadMaterial->setValue("u_buttons", ButtonPass.target->getTarget(0));

	MenuQuad = m_screenScene->m_entities.create();
	{
		Render& renderComp = m_screenScene->m_entities.emplace<Render>(MenuQuad);
		m_screenScene->m_entities.emplace<Transform>(MenuQuad);

		renderComp.geometry = screenAAQuadVAO;
		renderComp.material = menuQuadMaterial;
	}

	RenderPass MainMenuPass;
	MainMenuPass.scene = m_screenScene;
	MainMenuPass.parseScene();
	MainMenuPass.target = std::make_shared<FBO>(m_winRef.getSize(), defaultPassLayout);
	MainMenuPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	MainMenuPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	MainMenuPass.isScreen = true;

	MainMenuPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuView", MainMenuPass.camera.view);
	MainMenuPass.UBOmanager.setCachedValue("b_menuCamera", "u_menuProjection", MainMenuPass.camera.projection);

	FinalMainMenuPassIDx = m_mainMenuRenderer.getSumPassCount();

	m_mainMenuRenderer.addRenderPass(MainMenuPass);


	finalQuadMaterial->setValue("u_MainMenuIn", MainMenuPass.target->getTarget(0));



	/*************************
	*  Screen Relic Render Pass
	**************************/

	RenderPass ScreenRelicPass;
	ScreenRelicPass.scene = m_RelicScene;
	ScreenRelicPass.parseScene();
	ScreenRelicPass.target = std::make_shared<FBO>(glm::ivec2(1024, 1024), relicScreenPassLayout);
	ScreenRelicPass.viewPort = { 0,0,1024, 1024 };

	ScreenRelicPass.camera.projection = glm::ortho(0.f, 1024.0f, 1024.0f, 0.f);

	ScreenRelicPass.UBOmanager.setCachedValue("b_relicCamera2D", "u_relicView2D", ScreenRelicPass.camera.view);
	ScreenRelicPass.UBOmanager.setCachedValue("b_relicCamera2D", "u_relicProjection2D", ScreenRelicPass.camera.projection);

	ScreenRelicPassIDx = m_relicRenderer.getSumPassCount();
	m_relicRenderer.addRenderPass(ScreenRelicPass);
	m_relicRenderer.render();


	screenQuadMaterial->setValue("u_GroundDepthTexture", groundTexture);
	screenQuadMaterial->setValue("u_RelicTexture", ScreenRelicPass.target->getTarget(0));
	screenQuadMaterial->setValue("u_RelicDataTexture", ScreenRelicPass.target->getTarget(1));

	/*************************
	*  Inventory Display Relic Render Pass
	**************************/

	RenderPass InventoryDisplayScreenRelicPass;
	InventoryDisplayScreenRelicPass.scene = m_InventoryButtonScene;
	InventoryDisplayScreenRelicPass.parseScene();
	InventoryDisplayScreenRelicPass.target = std::make_shared<FBO>(glm::ivec2(1024, 1024), relicScreenPassLayout);
	InventoryDisplayScreenRelicPass.viewPort = { 0,0,1024, 1024 };

	InventoryDisplayScreenRelicPass.camera.projection = glm::ortho(0.f, 1024.0f, 1024.0f, 0.f);

	InventoryDisplayScreenRelicPass.UBOmanager.setCachedValue("b_relicCamera2D", "u_relicView2D", InventoryDisplayScreenRelicPass.camera.view);
	InventoryDisplayScreenRelicPass.UBOmanager.setCachedValue("b_relicCamera2D", "u_relicProjection2D", InventoryDisplayScreenRelicPass.camera.projection);

	m_relicRenderer.addRenderPass(InventoryDisplayScreenRelicPass);
	m_relicRenderer.render();

	m_gameInventoryMat->setValue("u_ButtonTexture", InventoryDisplayScreenRelicPass.target->getTarget(0));



	/*************************
	*  Scenery Render Pass
	**************************/

	RenderPass SceneryPass;
	SceneryPass.scene = m_SceneryScene;
	SceneryPass.parseScene();
	SceneryPass.target = std::make_shared<FBO>(glm::ivec2(1024, 1024), sceneryPassLayout);
	SceneryPass.viewPort = { 0,0,1024, 1024 };

	SceneryPass.camera.projection = glm::ortho(0.f, 1024.0f, 1024.0f, 0.f,0.0f,1.0f);

	SceneryPass.UBOmanager.setCachedValue("b_sceneryCamera2D", "u_sceneryView2D", SceneryPass.camera.view);
	SceneryPass.UBOmanager.setCachedValue("b_sceneryCamera2D", "u_sceneryProjection2D", SceneryPass.camera.projection);

	//entt::basic_view view = m_RelicScene->m_entities.view<Render>();
	//for (auto& rel : view) {
	//	Render render = m_RelicScene->m_entities.get<Render>(rel);
	//	render.material->setValue("u_relicView2D", ScreenRelicPass.camera.view);
	//	render.material->setValue("u_relicProjection2D", ScreenRelicPass.camera.projection);
	//}

	//ScreenRelicPassIDx = m_sceneryRenderer.getSumPassCount();
	m_sceneryRenderer.addRenderPass(SceneryPass);
	m_sceneryRenderer.render();


	screenQuadMaterial->setValue("u_SceneryTexture", SceneryPass.target->getTarget(0));
	screenQuadMaterial->setValue("u_SceneryDepthTexture", SceneryPass.target->getTarget(2));
	screenQuadMaterial->setValue("u_SceneryDataTexture", SceneryPass.target->getTarget(1));

	//screenQuadMaterial->setValue("u_SceneryDataTexture", ScreenRelicPass.target->getTarget(1));


	m_screenScene.reset(new Scene);

	Quad = m_screenScene->m_entities.create();
	{
		Render& renderComp = m_screenScene->m_entities.emplace<Render>(Quad);
		m_screenScene->m_entities.emplace<Transform>(Quad);
		renderComp.geometry = screenQuadVAO;

		renderComp.material = screenQuadMaterial;
	}

	/*************************
	*  particle Render Pass
	**************************/

	RenderPass particlePass;
	particlePass.scene = m_particleScene;
	particlePass.parseScene();
	particlePass.target = std::make_shared<FBO>(glm::ivec2(1024, 1024), particlePassLayout);
	particlePass.viewPort = { 0,0,1024, 1024 };

	particlePass.camera.projection = glm::ortho(0.f, 1024.0f, 1024.0f, 0.f);

	particlePass.UBOmanager.setCachedValue("b_particleCamera2D", "u_particleView2D", particlePass.camera.view);
	particlePass.UBOmanager.setCachedValue("b_particleCamera2D", "u_particleProjection2D", particlePass.camera.projection);

	ParticlePassIDx = m_particleRenderer.getSumPassCount();
	m_particleRenderer.addRenderPass(particlePass);

	screenQuadMaterial->setValue("u_ParticleTexture", particlePass.target->getTarget(0));
	screenQuadMaterial->setValue("u_ParticleDepthTexture", particlePass.target->getTarget(1));

	/*************************
	*  Screen Ground Render Pass
	**************************/
	RenderPass ScreenGroundPass;
	ScreenGroundPass.scene = m_screenScene;
	ScreenGroundPass.parseScene();
	ScreenGroundPass.target = std::make_shared<FBO>(m_winRef.doGetSize(), mainScreenPassLayout);
	ScreenGroundPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	ScreenGroundPass.isScreen = true;

	ScreenGroundPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	ScreenGroundPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", ScreenGroundPass.camera.view);
	ScreenGroundPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", ScreenGroundPass.camera.projection);

	Buffer = ScreenGroundPass.target->getTarget(1)->getID();

	ScreenGroundPassIDx = m_mainRenderer.getSumPassCount();
	m_mainRenderer.addRenderPass(ScreenGroundPass);

	pauseMenuQuadMaterial->setValue("u_game", ScreenGroundPass.target->getTarget(0));



	/*************************
	*  AA Render Pass
	**************************/


	screenAAQuadMaterial->setValue("u_inputTexture", ScreenGroundPass.target->getTarget(0));
	screenAAQuadMaterial->setValue("u_background", BackgroundPass.target->getTarget(0));
	screenAAQuadMaterial->setValue("u_gameButtons", ScreenGameButtonPass.target->getTarget(0));

	m_screenScene.reset(new Scene);

	AAQuad = m_screenScene->m_entities.create();
	{
		Render& renderComp = m_screenScene->m_entities.emplace<Render>(AAQuad);
		m_screenScene->m_entities.emplace<Transform>(AAQuad);
		renderComp.geometry = screenAAQuadVAO;
		renderComp.material = screenAAQuadMaterial;
	}

	RenderPass ScreenAAPass;
	ScreenAAPass.scene = m_screenScene;
	ScreenAAPass.parseScene();
	ScreenAAPass.target = std::make_shared<FBO>(m_winRef.getSize(), defaultPassLayout);
	ScreenAAPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	ScreenAAPass.isScreen = true;

	ScreenAAPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	ScreenAAPass.UBOmanager.setCachedValue("b_camera2D", "u_view2D", ScreenAAPass.camera.view);
	ScreenAAPass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", ScreenAAPass.camera.projection);

	AAPassIDx = m_mainRenderer.getSumPassCount();
	m_mainRenderer.addRenderPass(ScreenAAPass);


	finalQuadMaterial->setValue("u_InGameIn", ScreenAAPass.target->getTarget(0));

	finalQuadMaterial->setValue("u_State", 0.0f);

	m_screenScene.reset(new Scene);

	m_winRef.m_isFullScreen = !m_settings.s_Fullscreen;
	toggleFullscreen();

	finalQuad = m_screenScene->m_entities.create();
	{
		Render& renderComp = m_screenScene->m_entities.emplace<Render>(finalQuad);
		Transform& transComp = m_screenScene->m_entities.emplace<Transform>(finalQuad);

		transComp.scale = glm::vec3(width, height, 1);
		transComp.recalc();

		renderComp.geometry = FinalQuadVAO;
		renderComp.material = finalQuadMaterial;
	}



	RenderPass FinalPass;
	FinalPass.scene = m_screenScene;
	FinalPass.parseScene();
	FinalPass.target = std::make_shared<FBO>();
	FinalPass.viewPort = { 0,0,m_winRef.getWidth(), m_winRef.getHeight() };
	FinalPass.isScreen = true;

	FinalPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	FinalPass.UBOmanager.setCachedValue("b_finalcamera2D", "u_finalview2D", FinalPass.camera.view);
	FinalPass.UBOmanager.setCachedValue("b_finalcamera2D", "u_finalprojection2D", FinalPass.camera.projection);


	m_finalRenderer.addRenderPass(FinalPass);



}



void Archo::resetLayer()
{

}

void Archo::RefreshRelicFunctions()
{
	int currentDigSlot = 1;
	int currentTaskSlot = 1;
	for (int i = 0; i < 3; i++) {
		int Grade = 0;
		for (int j = 0; j < m_save.s_Items.size(); j++) {
			if (m_save.s_Items[j].first == m_save.s_Equiped[i]) {
				int counter = 0;
				for (int k = 0; k < 8; k++) {
					counter += int(pow(k + 1, 2));
					if (m_save.s_Items[j].second <= counter) {
						Grade = k;
						break;
					}
				}
				break;
			}
		}
		FunctionAllocationData FuncDat = BindNewFunction(m_save.s_Equiped[i], Grade, this, std::pair<int, int>{currentDigSlot, m_digBOs.size() - currentDigSlot}, std::pair<int, int>{currentTaskSlot, m_particleTasks.size() - currentTaskSlot});
		currentDigSlot += FuncDat.DigSlotsUsed;
		currentTaskSlot += FuncDat.ParticleTasksUsed;
		RelicFunctions[i] = FuncDat.BoundFunc;
		//spdlog::info("Grade: {}",Grade);
	}
	if (currentDigSlot == 0) currentDigSlot = 1;

	compute_GroundMaterial->setValue("DigSpotTotal", currentDigSlot);
	m_mainRenderer.getRenderPass(ScreenGroundPassIDx).scene->m_entities.get<Render>(Quad).material->setValue("DigSpotTotal", currentDigSlot);

	ClearDigSpotSSBO();
	ClearParticleTasksSSBO();
}

void Archo::RunRelicFunctions()
{
	//if (ProgressBar * timeToDig >= ProgressSegmentTarget_RelicTrigger) {
		//RelicSegmentTrigger = true;
				//spdlog::info("Trigger with: {} with dig time of {}",ProgressSegmentTarget_RelicTrigger,timeToDig);
		//ProgressSegmentTarget_RelicTrigger += timeToDig / (float)DigSegments;

	//}

	//if (ProgressBar * Segments >= ProgressSegmentTarget_RelicTrigger) {
		//RelicSegmentTrigger = true;
				//spdlog::info("Trigger with: {}",ProgressSegmentTarget_RelicTrigger);
		//ProgressSegmentTarget_RelicTrigger++;
	//}
	timeToDig = 1.0f;
	for (int i = 0; i < 3; i++) {
		if(RelicFunctions[i] != NULL) RelicFunctions[i]();
	}

	if (RelicSegmentTrigger == true) RelicSegmentTrigger = false;
	if (RelicFinishTrigger == true) RelicFinishTrigger = false;
	if (RelicBeginTrigger == true) RelicBeginTrigger = false;
	
}

void Archo::playGame()
{
	m_generationRenderer.getComputePass(0).material->setValue("Seed", glm::mod(allTime, 1.0f));
	//spdlog::info("dispatched with: {}",allTime);
	m_generationRenderer.render();

	placeRelics();
	placeScenery();

	compute_GroundMaterial->setValue("Mode",0.0f);
	m_groundComputeRenderer.render();

	state = GameState::InGame;
	pauseState = PauseState::Pause;
}

void Archo::mainSettings()
{
	auto& pass = m_mainMenuRenderer.getRenderPass(ButtonPassIDx);
	pass.scene = m_mainMenu_Settings;
	pass.parseScene();

	menuState = MenuState::Settings;
}

void Archo::mainSave()
{
	auto& pass = m_mainMenuRenderer.getRenderPass(ButtonPassIDx);
	pass.scene = m_mainMenu_Save;
	pass.parseScene();

	menuState = MenuState::Save;
}

void Archo::mainMenu()
{
	auto& pass = m_mainMenuRenderer.getRenderPass(ButtonPassIDx);
	pass.scene = m_mainMenu;
	pass.parseScene();

	menuState = MenuState::Main;

}

void Archo::pauseMenu()
{
	state = GameState::Paused;
	pauseState = PauseState::Pause;
}

void Archo::settings_to_pauseMenu()
{
	Save_Settings(m_settings);

	auto& pass = m_pausedRenderer.getRenderPass(PauseButtonPassIDx);
	pass.scene = m_pauseMenu;
	pass.parseScene();

	pauseState = PauseState::Pause;
	state = GameState::Paused; 
}

void Archo::pauseInventory()
{
	//std::array<RelicsBO, 48> temp;

	//m_relicsSSBO->edit(0, 48 * sizeof(RelicsBO), &temp);


	UpdateRelicsSSBO();

	auto& pass = m_pausedRenderer.getRenderPass(PauseButtonPassIDx);
	pass.scene = m_pauseMenu_Inventory;
	pass.parseScene();

	pauseState = PauseState::Inventory;
	state = GameState::Paused;
}

void Archo::equipToSlot1()
{
	for (int i = 0; i < 3; i++) {
		if (m_save.s_Equiped[i] == invRelicSelected) {
			m_save.s_Equiped[i] = m_save.s_Equiped[0];
			slotsButtonMats[i]->setValue("u_index", m_save.s_Equiped[i]);
		}
	}
	m_save.s_Equiped[0] = invRelicSelected;
	slotsButtonMats[0]->setValue("u_index", m_save.s_Equiped[0]);
	
	RefreshRelicFunctions();
}

void Archo::equipToSlot2()
{
	for (int i = 0; i < 3; i++) {
		if (m_save.s_Equiped[i] == invRelicSelected) {
			m_save.s_Equiped[i] = m_save.s_Equiped[1];
			slotsButtonMats[i]->setValue("u_index", m_save.s_Equiped[i]);
		}
	}
	m_save.s_Equiped[1] = invRelicSelected;
	slotsButtonMats[1]->setValue("u_index", m_save.s_Equiped[1]);

	RefreshRelicFunctions();
}

void Archo::equipToSlot3()
{
	for (int i = 0; i < 3; i++) {
		if (m_save.s_Equiped[i] == invRelicSelected) {
			m_save.s_Equiped[i] = m_save.s_Equiped[2];
			slotsButtonMats[i]->setValue("u_index", m_save.s_Equiped[i]);
		}
	}
	m_save.s_Equiped[2] = invRelicSelected;
	slotsButtonMats[2]->setValue("u_index", m_save.s_Equiped[2]);

	RefreshRelicFunctions();
}

void Archo::unpauseInventory()
{
	auto& pass = m_pausedRenderer.getRenderPass(PauseButtonPassIDx);
	pass.scene = m_pauseMenu;
	pass.parseScene();

	//m_generationRenderer.getComputePass(0).material->setValue("Seed", glm::mod(allTime, 1.0f));
	//m_generationRenderer.render();

	//placeRelics();
	//placeScenery();

	//compute_GroundMaterial->setValue("Mode", 0.0f);
	//m_groundComputeRenderer.render();

	pauseState = PauseState::Pause;
	state = GameState::InGame;
}

void Archo::pauseSettings()
{
	auto& pass = m_pausedRenderer.getRenderPass(PauseButtonPassIDx);
	pass.scene = m_pauseMenu_Settings;
	pass.parseScene();

	pauseState = PauseState::Settings;
}

void Archo::pause_to_Game()
{
	state = GameState::InGame;
	pauseState = PauseState::Pause;
}

void Archo::exitGame()
{
	Save_Game(m_save);
	Save_Settings(m_settings);
	//_sleep(1000);
	state = GameState::Exit;
}

void Archo::saveGame()
{
	//spdlog::info("Game Saved! (not implemented yet)");
	Save_Game(m_save);
}

void Archo::settings_to_mainMenu()
{
	Save_Settings(m_settings);

	auto& pass = m_mainMenuRenderer.getRenderPass(ButtonPassIDx);
	pass.scene = m_mainMenu;
	pass.parseScene();

	menuState = MenuState::Main;
}

void Archo::settings_to_Game()
{
	Save_Settings(m_settings);
	state = GameState::InGame;
	pauseState = PauseState::Pause;
}

void Archo::saveAndExit()
{
	Save_Game(m_save);
	state = GameState::MainMenu;
}

void Archo::deleteGameSave()
{
	m_save = Game_Save();
	Save_Game(m_save);
	ClearRelicsSSBO();
	for (int i = 0; i < 3; i++) {
		if (m_save.s_Equiped[i] == invRelicSelected) {
			m_save.s_Equiped[i] = -1;
			slotsButtonMats[i]->setValue("u_index", m_save.s_Equiped[i]);
		}
	}
	RefreshRelicFunctions();
}

#include "windows/window.hpp"
#include "windows/GLFW_GL_GC.hpp"
void Archo::toggleFullscreen()
{
	if (m_winRef.m_isFullScreen) {
		glfwSetWindowMonitor(((IWindow<GLFWwindow, GLFWWinDeleter, ModernGLFW_GL_GC>*) & m_winRef)->m_nativeWindow.get(), nullptr, (glfwGetVideoMode(glfwGetPrimaryMonitor())->width / (m_settings.s_ResolutionFract)) * 0.05f, (glfwGetVideoMode(glfwGetPrimaryMonitor())->height / (m_settings.s_ResolutionFract)) * 0.05f, glfwGetVideoMode(glfwGetPrimaryMonitor())->width / (m_settings.s_ResolutionFract * 1.1f), glfwGetVideoMode(glfwGetPrimaryMonitor())->height / (m_settings.s_ResolutionFract * 1.1f), 0);
		m_winRef.m_isFullScreen = false;
	}
	else {
		glfwSetWindowMonitor(((IWindow<GLFWwindow, GLFWWinDeleter, ModernGLFW_GL_GC>*) & m_winRef)->m_nativeWindow.get(), glfwGetPrimaryMonitor(), 0, 0, glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, 0);

		m_winRef.m_isFullScreen = true;
	}
	m_settings.s_Fullscreen = m_winRef.m_isFullScreen;
	Save_Settings(m_settings);
}

std::vector<glm::vec2> IslandSeeder(int lines) {
	std::vector<glm::vec2> pointsOut;
	for (int i = 0; i < lines; i++) {
		glm::vec2 newPoint1;
		glm::vec2 newPoint2;

		newPoint1 = glm::vec2(Randomiser::uniformFloatBetween(-0.5, 0.5), Randomiser::uniformFloatBetween(-0.5, 0.5));
		newPoint2 = glm::vec2(Randomiser::uniformFloatBetween(-0.5, 0.5), Randomiser::uniformFloatBetween(-0.5, 0.5));

		pointsOut.push_back(newPoint1);
		pointsOut.push_back(newPoint2);
	}
	return pointsOut;
}

void Archo::setupGenerator(Renderer& renderer, std::shared_ptr<Texture> target, std::shared_ptr<Texture> working, std::shared_ptr<Shader> shader)
{


	std::vector<int> Layers{ 0, 1, 1, 2, 1, 1, 1, 2, 4 , 3, 4, 3, 4, 3, 4, 3, 4 };//1,2,3,2,3,1,2,3,2,4,2 };

	std::shared_ptr<Texture> flipper[2]{ working, target };

	bool isWorking = !(Layers.size() & 1); // is Layers size Even

	for (int i = 0; i < Layers.size(); i++) {

		std::shared_ptr<Material> mat = std::make_shared<Material>(shader);

		mat->setValue("Type", (float)Layers[i]);
		mat->setValue("Size", glm::vec2(target->getWidthf(), target->getHeightf()));
		mat->setValue("Depth", (float)i);

		ComputePass ComputePass;
		ComputePass.material = mat;
		ComputePass.workgroups = { glm::max(target->getWidth() / 32, (unsigned)1),glm::max(target->getHeight() / 32, (unsigned)1),1 };
		ComputePass.barrier = MemoryBarrier::ShaderImageAccess;

		Image InImg;
		InImg.mipLevel = 0;
		InImg.layered = false;
		InImg.texture = flipper[(int)isWorking];
		InImg.imageUnit = 0;//ComputePass.material->m_shader->m_imageBindingPoints["ImgIn"];
		InImg.access = TextureAccess::ReadWrite;

		Image OutImg;
		OutImg.mipLevel = 0;
		OutImg.layered = false;
		OutImg.texture = flipper[(int)(!isWorking)];;
		OutImg.imageUnit = 1;//ComputePass.material->m_shader->m_imageBindingPoints["ImgOut"];
		OutImg.access = TextureAccess::ReadWrite;

		ComputePass.images.push_back(InImg);
		ComputePass.images.push_back(OutImg);

		renderer.addComputePass(ComputePass);

		isWorking = !isWorking;
	}


}

std::vector<SeedingPoint> Archo::getSeedingPoints()
{
	for (int i = 0; i < m_seedingPoints.size(); i++) {
		m_seedingPoints[i] = SeedingPoint();
	}
	m_seedingSSBO->edit(0, sizeof(SeedingPoint) * m_seedingPoints.size(), m_seedingPoints.data());

	m_seedingFinderRenderer.render();


	//auto view = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.view<Relic>();

	//for (auto relic : view) {
	//	Render& rendComp = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.get<Render>(relic);
	//	rendComp.material->setValue("u_active", (float)(int)false);
	//}
	m_seedingPoints = m_seedingSSBO->writeToCPU<SeedingPoint>();

	std::vector<SeedingPoint> outputPoints;

	int counter = 0;
	for (int i = 0; i < m_seedingPoints.size(); i++) {
		SeedingPoint current = m_seedingPoints[i];
		if (current.position.x > -0.5f) {

			outputPoints.push_back(current);

			//if (false) {
			//	Render& rendComp = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.get<Render>(view[counter]);
			//	rendComp.material->setValue("u_active", (float)(int)true);
			//	rendComp.material->setValue("u_Rarity", 6.0f);

			//	Transform& transComp = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.get<Transform>(view[counter]);
			//	transComp.translation = glm::vec3(glm::vec2(current.position) * float(m_relicRenderer.getRenderPass(ScreenRelicPassIDx).viewPort.height / m_generationRenderer.getComputePass(0).images[0].texture->getHeight()), -0.5f);
			//	transComp.scale = glm::vec3(4096.f / (current.position.w / 2.0f));
			//	transComp.recalc();

			//	counter++;
			//}
		}
	}

	return outputPoints;
}

void Archo::placeRelics()
{
	std::vector<SeedingPoint> points = getSeedingPoints();

	auto view = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.view<Relic>();
	RemainingRelics = 0;
	//int Curve[6]{ 13,8,5,3,2,1 };
	int Curve[6]{ 6,4,2,2,1,1 };
	std::array<std::pair<int, int>, 6> Ranges{ std::pair<int,int>(0,16), std::pair<int,int>(16,27), std::pair<int,int>(27,35), std::pair<int,int>(35,41), std::pair<int,int>(41,45), std::pair<int,int>(45,48) };
	for (int i = 0; i < view.size(); i++) {
		Render& renderComp = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.get<Render>(view[i]);
		Transform& transComp = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.get<Transform>(view[i]);
		Relic& relicComp = m_relicRenderer.getRenderPass(ScreenRelicPassIDx).scene->m_entities.get<Relic>(view[i]);
		float rarity = 0;
		int counter = 0;
		for (int j = 0; j < std::size(Curve); j++) {
			counter += Curve[j];
			if (i < counter) {
				rarity = Randomiser::uniformIntBetween(Ranges[j].first, Ranges[j].second);;
				//spdlog::info("Added of rarity: {}", j);
				break;
			}
		}

		if (points.size() > 0) {

			//spdlog::info("Rarities: {} : {}", i, rarity);

			int random = std::rand() % points.size();
			renderComp.material->setValue("u_active", (float)(int)true);
			renderComp.material->setValue("u_Rarity", rarity + 0.001f);
			transComp.translation = glm::vec3(glm::vec2(points[random].position) *
				float(m_relicRenderer.getRenderPass(ScreenRelicPassIDx).viewPort.height
					/ m_generationRenderer.getComputePass(0).images[0].texture->getHeight()),
				-0.5f);
			transComp.scale = glm::vec3(1024.f / (points[random].position.w / 2.0f));
			transComp.recalc();

			RemainingRelics++;

			points.erase(points.begin() + random);

			auto& renderComp2 = m_InventoryButtonScene->m_entities.get<Render>(m_Relics2[i]);
			renderComp2.material->setValue("u_active", (float)(int)true);
			renderComp2.material->setValue("u_Rarity", rarity + 0.001f);
			relicComp.Active = true;
		}
		else {
			renderComp.material->setValue("u_active", (float)(int)false);
		}
	}

	//for (int i = 0; i < m_Relics2.size(); i++) {

	//}
	m_relicRenderer.render();
}

void Archo::placeScenery()
{
	auto view = m_SceneryScene->m_entities.view<Transform>();
	for(auto obj : view){

		std::vector<SeedingPoint> points = getSeedingPoints();

		Transform& transformComp = m_SceneryScene->m_entities.get<Transform>(obj);
		Render& renderComp = m_SceneryScene->m_entities.get<Render>(obj);
		Scenery& sceneComp = m_SceneryScene->m_entities.get<Scenery>(obj);

		sceneComp.DugOut = 0.0f;

		if (points.size() > 0) {
			int random = std::rand() % points.size();
			renderComp.material->setValue("u_active", (float)(int)true);
			renderComp.material->setValue("u_DugOut", sceneComp.DugOut);
			transformComp.translation = glm::vec3(glm::vec2(points[random].position) *
				float(m_sceneryRenderer.getRenderPass(0).viewPort.height
					/ m_generationRenderer.getComputePass(0).images[0].texture->getHeight()),
				-1.0f);

			if (sceneComp.type == 0) {
				float s = Randomiser::uniformFloatBetween(12.5f * 2, 37.5f * 2);
				transformComp.scale = glm::vec3(s, s, 1.0f);

				float r = Randomiser::uniformFloatBetween(-0.52359878f, 0.52359878f);
				transformComp.rotation = glm::vec3(0, 0, r);
			}
			else if (sceneComp.type == 1) {
				float s = Randomiser::uniformFloatBetween(12.5f * 2, 18.75f * 2);
				transformComp.scale = glm::vec3(s, s, 1.0f);

				float r = Randomiser::uniformFloatBetween(-0.52359878f, 0.52359878f);
				transformComp.rotation = glm::vec3(0, 0, r);
			}



			transformComp.recalc();

			points.erase(points.begin() + random);
		}
		else {
			renderComp.material->setValue("u_active", (float)(int)false);
		}



		//transformComp.rotation = glm::quat(glm::vec3(Randomiser::uniformFloatBetween(-3.14159f, 3.14159f),Randomiser::uniformFloatBetween(-3.14159f, 3.14159f),0.0f));
		//transformComp.translation = glm::vec3(Randomiser::uniformFloatBetween(transformComp.scale.x, 4096.0f - transformComp.scale.x), Randomiser::uniformFloatBetween(transformComp.scale.y, 4096.0f - transformComp.scale.y), -1.0f);
		//transformComp.recalc();
	}

	m_sceneryRenderer.render();
}





