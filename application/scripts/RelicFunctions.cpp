#include "RelicFunctions.hpp"

FunctionAllocationData BindNewFunction(int ItemID, int ItemGrade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	FunctionAllocationData outData = FunctionAllocationData();

	if (DigSpotRange.first <= 0) outData.DigSlotsUsed = 1;

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
	}	
	else if (ItemID == 8) {
		outData.BoundFunc = std::bind(Relic_9_Function, ItemGrade, GameRef, DigSpotRange);
	}	
	else if (ItemID == 9) {
		outData.BoundFunc = std::bind(Relic_10_Function, ItemGrade, GameRef, DigSpotRange);
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

void Relic_7_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	GameRef->m_digBOs[0].DigMask = 3;
}

void Relic_8_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	GameRef->m_digBOs[0].DigMask = 4;
}

void Relic_9_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	GameRef->m_digBOs[0].DigMask = 5;
}

void Relic_10_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_11_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_12_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_13_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_14_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_15_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_16_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_17_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_18_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_19_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_20_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_21_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_22_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_23_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_24_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
}

void Relic_25_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
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
