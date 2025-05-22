#include "RelicFunctions.hpp"

FunctionAllocationData BindNewFunction(int ItemID, int ItemGrade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	FunctionAllocationData outData = FunctionAllocationData();
	if (ItemID == 0) {
		outData.BoundFunc = std::bind(Relic_1_Function, ItemGrade, GameRef,DigSpotRange);
		if(DigSpotRange.first <= 0) outData.DigSlotsUsed = 1;
	}
	else if (ItemID == 1) {
		outData.BoundFunc = std::bind(Relic_2_Function, ItemGrade, GameRef, DigSpotRange);
		if (DigSpotRange.first <= 0) outData.DigSlotsUsed = 1;
	}
	else if (ItemID == 2) {
		outData.BoundFunc = std::bind(Relic_2_Function, ItemGrade, GameRef, DigSpotRange);
		if (DigSpotRange.first <= 0) outData.DigSlotsUsed = 1;
	}
	return outData;
}

void Relic_1_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	//std::cout << "\033[2J\033[1;1H";
	//spdlog::info("At Grade: {}", Grade);
	//spdlog::info("Before: {}", GameRef->m_digBOs[0].DigInfo.z);
	GameRef->m_digBOs[0].DigInfo.z /= 2 + (Grade/3.f);
	//spdlog::info("After: {}", GameRef->m_digBOs[0].DigInfo.z);
}

void Relic_2_Function(int Grade, Archo* GameRef, std::pair<int, int> DigSpotRange)
{
	GameRef->m_digBOs[0].DigMask = 4;
}
