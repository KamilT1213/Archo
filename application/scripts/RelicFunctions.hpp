#pragma once
#include "DemonRenderer.hpp"
#include "MainScene.hpp"
#include <functional>

struct FunctionAllocationData {
	int DigSlotsUsed{ 0 };
	std::function<void()> BoundFunc;
};

FunctionAllocationData BindNewFunction(int ItemID, int ItemGrade, Archo* GameRef, std::pair<int, int>DigSpotRange);

void Relic_1_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_2_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);