#include "RelicFunctions.hpp"

//Bug to fix -----------> When relics that spawn extra dig spots are not the left most slot they will not work as intended

FunctionAllocationData BindNewFunction(int ItemID, int ItemGrade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	FunctionAllocationData outData = FunctionAllocationData();
	//spdlog::info("DigSpotRanges: {} - {}",DigSpotRange.first,DigSpotRange.second);

	if (ItemID == 0) {
		outData.BoundFunc = std::bind(Relic_1_Function, ItemGrade, GameRef,DigSpotRange);
	}
	else if (ItemID == 1) {
		outData.BoundFunc = std::bind(Relic_2_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 2) {
		outData.BoundFunc = std::bind(Relic_3_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 3) {
		outData.BoundFunc = std::bind(Relic_4_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 4) {
		outData.BoundFunc = std::bind(Relic_5_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 5) {
		outData.BoundFunc = std::bind(Relic_6_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 6) {
		outData.BoundFunc = std::bind(Relic_7_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 7) {
		outData.BoundFunc = std::bind(Relic_8_Function, ItemGrade, GameRef, DigSpotRange);
		outData.DigSlotsUsed += 1;
	}	
	else if (ItemID == 8) {
		outData.BoundFunc = std::bind(Relic_9_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 9) {
		outData.BoundFunc = std::bind(Relic_10_Function, ItemGrade, GameRef, DigSpotRange);
		outData.DigSlotsUsed += 1;
	}	
	else if (ItemID == 10) {
		outData.BoundFunc = std::bind(Relic_11_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 11) {
		outData.BoundFunc = std::bind(Relic_12_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 12) {
		outData.BoundFunc = std::bind(Relic_13_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 13) {
		outData.BoundFunc = std::bind(Relic_14_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 14) {
		outData.BoundFunc = std::bind(Relic_15_Function, ItemGrade, GameRef, DigSpotRange);
		outData.DigSlotsUsed += 1;
	}	
	else if (ItemID == 15) {
		outData.BoundFunc = std::bind(Relic_16_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 16) {
		outData.BoundFunc = std::bind(Relic_17_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 17) {
		outData.BoundFunc = std::bind(Relic_18_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 18) {
		outData.BoundFunc = std::bind(Relic_19_Function, ItemGrade, GameRef, DigSpotRange);
		outData.DigSlotsUsed += 1;
	}	
	else if (ItemID == 19) {
		outData.BoundFunc = std::bind(Relic_20_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 20) {
		outData.BoundFunc = std::bind(Relic_21_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 21) {
		outData.BoundFunc = std::bind(Relic_22_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 22) {
		outData.BoundFunc = std::bind(Relic_23_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 23) {
		outData.BoundFunc = std::bind(Relic_24_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 24) {
		outData.BoundFunc = std::bind(Relic_25_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 25) {
		outData.BoundFunc = std::bind(Relic_26_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 26) {
		outData.BoundFunc = std::bind(Relic_27_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 27) {
		outData.BoundFunc = std::bind(Relic_28_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 28) {
		outData.BoundFunc = std::bind(Relic_29_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 29) {
		outData.BoundFunc = std::bind(Relic_30_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 30) {
		outData.BoundFunc = std::bind(Relic_31_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 33) {
		outData.BoundFunc = std::bind(Relic_32_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 32) {
		outData.BoundFunc = std::bind(Relic_33_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 33) {
		outData.BoundFunc = std::bind(Relic_34_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 34) {
		outData.BoundFunc = std::bind(Relic_35_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 35) {
		outData.BoundFunc = std::bind(Relic_36_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 36) {
		outData.BoundFunc = std::bind(Relic_37_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 37) {
		outData.BoundFunc = std::bind(Relic_38_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 38) {
		outData.BoundFunc = std::bind(Relic_39_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 39) {
		outData.BoundFunc = std::bind(Relic_40_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 40) {
		outData.BoundFunc = std::bind(Relic_41_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 41) {
		outData.BoundFunc = std::bind(Relic_42_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 42) {
		outData.BoundFunc = std::bind(Relic_43_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 43) {
		outData.BoundFunc = std::bind(Relic_44_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 44) {
		outData.BoundFunc = std::bind(Relic_45_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 45) {
		outData.BoundFunc = std::bind(Relic_46_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 46) {
		outData.BoundFunc = std::bind(Relic_47_Function, ItemGrade, GameRef, DigSpotRange);
	}
	else if (ItemID == 47) {
		outData.BoundFunc = std::bind(Relic_48_Function, ItemGrade, GameRef, DigSpotRange);
	}
	return outData;
}

//Commons

void Relic_1_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Increase radius by 1 plus (0 to 1) depeding on grade //Common
{
	float size = GameRef->m_digBOs[0].DigInfo.z;
	size -= 1.0f  + (1.0f * (Grade/6.0f));
	if (size < 1) size = 1;
	GameRef->m_digBOs[0].DigInfo.z = size;
}

void Relic_2_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Decrease segments by 1 plus (0 to 6) depeding on grade //Common
{
	int Segments = GameRef->Segments;
	Segments -= 1 + Grade;
	if (Segments < 1) Segments = 1;
	GameRef->Segments = Segments;
}

void Relic_3_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Decrease dig time by 0.1 plus (0 to 0.1) depending on grade //Common
{
	float digTime = GameRef->timeToDig;
	digTime -= 0.1f + (0.1f * (Grade / 6.0f));
	if (digTime < 0.1f) digTime = 0.1f;
	GameRef->timeToDig = digTime;
}

void Relic_4_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Decrease scenery extraction segments by 16 plus (0 to 16) depending on grade //Common
{
	if (GameRef->isScenery) {
		int Segments = GameRef->Segments;
		Segments -= 16 + (16 * (Grade/6.0f));
		if (Segments < 1) Segments = 1;
		GameRef->Segments = Segments;
	}
}

void Relic_5_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Randomizes dig rotation for every segment and has a (1% to 25%) chance to make the dig larger by (5 to 15) depending on grade //Common
{
	if (GameRef->RelicSegmentTrigger) {
		GameRef->m_digBOs[0].rotation = Randomiser::uniformFloatBetween(-glm::pi<float>(), glm::pi<float>());
		float scale = Randomiser::uniformFloatBetween(0, 10.0f - (Grade * 1.6f));
		
		if (scale < 0.1f) {
			float size = GameRef->m_digBOs[0].DigInfo.z;
			size -= 5 + (10 * (Grade / 6));
			if (size < 1) size = 1;
			GameRef->m_digBOs[0].DigInfo.z = size;
		}
	}
}

void Relic_6_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) // chance to instantly destroy scenery (1% to 2%) depending on grade //Common
{
	if (GameRef->RelicSegmentTrigger) {
		if (GameRef->isScenery) {
			float chance = Randomiser::uniformFloatBetween(0, 10.0f - (5.0f * (Grade / 6.0f)));
			if (chance < 0.1f) {
				auto& sceneComp = GameRef->m_SceneryScene->m_entities.get<Scenery>(GameRef->m_Sceneries.at(GameRef->RelId - 1));
				sceneComp.DugOut = 2.0;

			}

		}
	}
}

void Relic_7_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Increases Dig depth by (0.05 to 0.15) depending on grade and reduces radius by 5 //Common
{
	GameRef->m_digBOs[0].DigInfo.z += 5;
	GameRef->m_digBOs[0].DigInfo.w += 0.05 + (0.1 * (Grade/6.0f));
}

void Relic_8_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //Increase Dig depth by (0.05 to 0.15) depending on grade and empty dig slot
{
	GameRef->m_digBOs[0].DigInfo.w += 0.05 + (0.1 * (Grade/6.0f));
	int target = DigSpotRange.first;
	//spdlog::info("8: Slot edited: {}",target);
	GameRef->m_digBOs[target].DigInfo.w += 0.05 + (0.1 * (Grade/6.0f));
}

void Relic_9_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) // chance to remove random scenery (0.5% to 1%) depending on grade
{
	if (GameRef->RelicSegmentTrigger) {
		float chance = Randomiser::uniformFloatBetween(0, 20.0f - (10.0f * (Grade / 6.0f)));
		if(chance < 0.1f){
			auto view = GameRef->m_SceneryScene->m_entities.view<Scenery>();
			for(auto Obj : view){
				Scenery& sceneComp = GameRef->m_SceneryScene->m_entities.get<Scenery>(Obj);
				if(sceneComp.DugOut < 1.0){
					auto& renderComp = GameRef->m_SceneryScene->m_entities.get<Render>(Obj);
					renderComp.material->setValue("u_active", 0.0f);
					sceneComp.DugOut = 1.0;
					GameRef->m_sceneryRenderer.render();
					break;
				}
			}
		}
	}
}

void Relic_10_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// chance to dig a small hole in a random spot near the player dig site
{
	int target = DigSpotRange.first;
	//spdlog::info("10: Slot edited: {}",target);
	//GameRef->m_digBOs[target].DigInfo = glm::vec4(0, 0, 0.0f, 0.0f);
	if (GameRef->RelicBeginTrigger) {
		float chance = Randomiser::uniformFloatBetween(0, 0.6f - (0.5f * (Grade / 6.0f)));
		glm::vec2 digLocal = glm::vec2(GameRef->m_digBOs[0].DigInfo);
		digLocal += glm::vec2(Randomiser::uniformFloatBetween(-0.03,0.03),Randomiser::uniformFloatBetween(-0.03,0.03));
		if(chance < 0.1f || true){

			GameRef->m_digBOs[target].DigInfo = glm::vec4(digLocal.x, digLocal.y, 25.0f, 0.1f);
		}

	}
	else if(GameRef->RelicFinishTrigger){
		GameRef->m_digBOs[target].DigInfo = glm::vec4(0, 0, 0.0f, 0.0f);
	}
}

void Relic_11_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) //changes primary dig to cross rotated 45 degrees, changes dig pos to random around cursor but is bigger
{
	GameRef->m_digBOs[0].DigMask = 1;
	float size = GameRef->m_digBOs[0].DigInfo.z;
	size -= 3.0f;
	if (size < 1) size = 1;
	GameRef->m_digBOs[0].DigInfo.z = size;
	GameRef->m_digBOs[0].rotation = glm::radians(45.0f);

	GameRef->m_digBOs[0].DigInfo.w -= 0.09f;
	GameRef->timeToDig -= 0.2f;

	glm::vec2 digLocal = glm::vec2(GameRef->m_digBOs[0].DigInfo);
	digLocal += glm::vec2(Randomiser::uniformFloatBetween(-0.03,0.03),Randomiser::uniformFloatBetween(-0.03,0.03));

	GameRef->m_digBOs[0].DigInfo.x = digLocal.x;
	GameRef->m_digBOs[0].DigInfo.y = digLocal.y;


}

void Relic_12_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// Increases Digging depth drastically, increases dig time just as much
{
	GameRef->timeToDig += 4 * (Grade / 6.0f);
	GameRef->m_digBOs[0].DigInfo.w += 0.2 * (Grade / 6.0f);
}

void Relic_13_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// Increases Radius of all Relics but reduces dig speed
{
	for(int i = 0; i < GameRef->m_digBOs.size(); i++){
		float size = GameRef->m_digBOs[i].DigInfo.z;
		if(size != 0){
			size -= 10.0f * (Grade / 6.0f);
			if (size < 1) size = 1;
			GameRef->m_digBOs[i].DigInfo.z = size;
		}

	}
	GameRef->timeToDig += 1.0f + (5.0f * (Grade / 6.0f));
}

void Relic_14_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// Increases Segments when extracting but reduces segment time
{
	GameRef->Segments += 30 * (Grade / 6.0f);
	GameRef->timePerSegment -= 0.1f * (Grade / 6.0f);
}

void Relic_15_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// digs in a single circle pattern around the player cursor scales to normal dig depth (100% to 200%) by grade
{
	
	if(GameRef->ProgressBar > 1.0f/3.0f){
		float progress = GameRef->ProgressBar - (1.0f/3.0f);
		progress *= 1.5f;
		int target = DigSpotRange.first;

		GameRef->m_digBOs[target].DigInfo.z += 55.0f;
		GameRef->m_digBOs[target].DigInfo.w *= 1.0f + (1.0f * (Grade / 6.0f));
		//GameRef->m_digBOs[target].DigInfo.w += 1.25f;

		glm::vec2 digLocal = glm::vec2(GameRef->m_digBOs[0].DigInfo);
		digLocal += glm::vec2(glm::sin(progress * glm::pi<float>() * 2.0f),glm::cos(progress * glm::pi<float>() * 2.0f)) * ((1.0f / GameRef->m_digBOs[0].DigInfo.z) + (1.0f / GameRef->m_digBOs[target].DigInfo.z));

		GameRef->m_digBOs[target].DigInfo.x = digLocal.x;
		GameRef->m_digBOs[target].DigInfo.y = digLocal.y;
		//spdlog::info("15: Slot edited: {}",target);
	}
	else{
		int target = DigSpotRange.first;
		GameRef->m_digBOs[target].DigInfo = glm::vec4(0,0,0,0);
		//spdlog::info("15: Slot edited: {}",target);
	}

}

void Relic_16_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) // pulls all dig spots closer to player cursor (0% to 60%) scales by grade
{
	if(GameRef->RelicSegmentTrigger){
		glm::vec2 localPos = glm::vec2(GameRef->m_digBOs[0].DigInfo);
		for(int i = 0; i < GameRef->m_digBOs.size(); i++){
			glm::vec2 currentPos = glm::vec2(GameRef->m_digBOs[i].DigInfo);
			currentPos = glm::mix(currentPos,localPos,0.6f * (Grade / 6.0f));
			GameRef->m_digBOs[i].DigInfo.x = currentPos.x;
			GameRef->m_digBOs[i].DigInfo.y = currentPos.y;
		}
	}

}

//Uncommons

void Relic_17_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// increase dig depth by (40% to 100%) but dig size by (40% to 100%)
{
	GameRef->m_digBOs[0].DigInfo.z *= 1.4f + (Grade/6.0f);
	GameRef->m_digBOs[0].DigInfo.w *= 1.4f + (Grade/6.0f);
}

void Relic_18_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// changes dig brush to cat scratch with random rotations increases dig depth (0.01 to 0.1)
{
	GameRef->m_digBOs[0].DigMask = 4;
	GameRef->m_digBOs[0].DigInfo.w += 0.01 + (0.09 * (Grade/6.0f));
	if (GameRef->RelicSegmentTrigger) {
		GameRef->m_digBOs[0].rotation = Randomiser::uniformFloatBetween(-glm::pi<float>(), glm::pi<float>());

	}
}

void Relic_19_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// dig spot that is placed randomly and goes towards the player cursour is sped up (0.05 to 0.35) by grade
{

	int target = DigSpotRange.first;

	//spdlog::info("19: Slot edited: {}",target);

	if (GameRef->RelicBeginTrigger) {
		GameRef->m_digBOs[target].DigInfo.x = Randomiser::uniformFloatBetween(0, 1);
		GameRef->m_digBOs[target].DigInfo.y = Randomiser::uniformFloatBetween(0, 1);
	}
	glm::vec2 direction = glm::normalize(glm::vec2(GameRef->m_digBOs[0].DigInfo) - glm::vec2(GameRef->m_digBOs[target].DigInfo));
	GameRef->m_digBOs[target].DigInfo.x += direction.x * GameRef->deltaTime * (0.05f * (Grade + 1));
	GameRef->m_digBOs[target].DigInfo.y += direction.y * GameRef->deltaTime * (0.05f * (Grade + 1));
	GameRef->m_digBOs[target].DigInfo.z *= 1.5f;
	GameRef->m_digBOs[target].DigInfo.w *= 1.1f;
	//spdlog::info("Changes of bbo: {} {} {} {}, at {}",GameRef->m_digBOs[target].DigInfo.x,GameRef->m_digBOs[target].DigInfo.y,GameRef->m_digBOs[target].DigInfo.z,GameRef->m_digBOs[target].DigInfo.w, GameRef->ProgressBar);
	//spdlog::info("Position: x:{} , y:{}",GameRef->m_digBOs[target].DigInfo.x,GameRef->m_digBOs[target].DigInfo.y);
	//spdlog::info("Values: x:{} , y:{}",GameRef->m_digBOs[target].DigInfo.z,GameRef->m_digBOs[target].DigInfo.w);
}

void Relic_20_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange) // increase all digspots size at random by (0% - 10% to 0% - 30%)
{
		for(int i = 0; i < GameRef->m_digBOs.size(); i++){
			float size = GameRef->m_digBOs[i].DigInfo.z;
			if(size != 0){
				float random = Randomiser::uniformFloatBetween(0.f,0.1f + (0.2f * (Grade / 6.0f)));
				size *= 1.0f - random;
				if (size < 1) size = 1;
				GameRef->m_digBOs[i].DigInfo.z = size;
			}
		}
}

void Relic_21_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// decreases dig and extraction time by (10% to 30%)
{
	GameRef->timeToDig *= 0.9f - (0.2f * (Grade / 6.0f));
	GameRef->timePerSegment *= 0.9f - (0.2f * (Grade / 6.0f));
}

void Relic_22_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// creates a ravine between dig pos and mouse pointer at a size limit
{
	GameRef->m_digBOs[0].DigMask = 6;
	glm::vec2 dir = GameRef->m_PointerPos_inGame - GameRef->m_DigPos;
	float dist = glm::clamp(glm::length(dir)/2.0f,0.001f,0.1f);
	dir = glm::normalize(dir);

	float preSize = GameRef->m_digBOs[0].DigInfo.z;


	glm::vec2 newPos = GameRef->m_DigPos + (dir * (dist));
	float newSize = 1.0f / dist;
	float newRot = glm::tanh(dir.x/dir.y);
	float newDepth = GameRef->m_digBOs[0].DigInfo.w * (((0.1 + (1.0f / preSize)) - dist) * 5) * (1 + (Grade / 6.0f));

	GameRef->m_digBOs[0].DigInfo = glm::vec4(newPos,newSize,newDepth);
	GameRef->m_digBOs[0].rotation = newRot;
}

void Relic_23_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// Moves currently removing scenery with mouse cursor increases scenery segments drastically (400% to 100%)
{
	if(GameRef->isScenery){
		GameRef->Segments *= 4.0f - (3.0f * (Grade / 6.0f));
	}
	if(GameRef->isScenery && GameRef->Pressed && GameRef->RelicSceneryOffsetHold.x == 0 && GameRef->RelId > 0){
		//spdlog::info("RelID: {}",GameRef->RelId);
		auto& transformComp = GameRef->m_SceneryScene->m_entities.get<Transform>(GameRef->m_Sceneries.at(GameRef->RelId - 1));
		GameRef->RelicSceneryOffsetHold = glm::vec2(transformComp.translation) - (glm::abs(glm::vec2(0,1) - GameRef->m_PointerPos_inGame) * 1024.f);
	}
	else if(GameRef->Pressed && glm::abs(GameRef->RelicSceneryOffsetHold.x) > 0 && GameRef->RelId > 0){
		auto& transformComp = GameRef->m_SceneryScene->m_entities.get<Transform>(GameRef->m_Sceneries.at(GameRef->RelId - 1));
		transformComp.translation = glm::vec3((glm::abs(glm::vec2(0,1) - GameRef->m_PointerPos_inGame) * 1024.f) + GameRef->RelicSceneryOffsetHold,transformComp.translation.z);
		transformComp.recalc();
		GameRef->m_sceneryRenderer.render();
		GameRef->m_DigPos = GameRef->m_PointerPos_inGame;

	}
	else{
		GameRef->RelicSceneryOffsetHold = glm::vec2(0);
		GameRef->m_interactionState = InteractionState::Idle;
	}
}

void Relic_24_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// locks all Digpos to grid of dig size increases dig depth by (0% to 100%)
{
	for(int i = 0; i < GameRef->m_digBOs.size(); i++){
		float size = GameRef->m_digBOs[i].DigInfo.z / 2.0f;
		glm::vec2 location = glm::vec2(GameRef->m_digBOs[i].DigInfo);
		location -= 0.5f;
		location *= size;
		location = glm::floor(location) + 0.5f;
		location /= size;
		location += 0.5f;
		GameRef->m_digBOs[i].DigInfo.x = location.x;
		GameRef->m_digBOs[i].DigInfo.y = location.y;
		GameRef->m_digBOs[i].DigInfo.w *= 1.0 + (Grade / 6.0f);
	}
}

void Relic_25_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)// increase radius and dig strength but mirror dig spot around center
{
	glm::vec2 location = glm::vec2(GameRef->m_digBOs[0].DigInfo);
	location -= 0.5f;
	location *= -1;
	location += 0.5f;
	GameRef->m_digBOs[0].DigInfo.x = location.x;
	GameRef->m_digBOs[0].DigInfo.y = location.y;
	GameRef->m_digBOs[0].DigInfo.x *= 0.9 - (0.4 * (Grade / 6.0f));
	GameRef->m_digBOs[0].DigInfo.w *= 1 + (1 * (Grade / 6.0f));
}

void Relic_26_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_27_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_28_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_29_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_30_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_31_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_32_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_33_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_34_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_35_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_36_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_37_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_38_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_39_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_40_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_41_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_42_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_43_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_44_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_45_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_46_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_47_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_48_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}
