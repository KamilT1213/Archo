#pragma once
#include "DemonRenderer.hpp"
#include "MainScene.hpp"
#include <functional>

struct FunctionAllocationData {
	int DigSlotsUsed{ 0 };
	std::function<void()> BoundFunc;
};

FunctionAllocationData BindNewFunction(int ItemID, int ItemGrade, Archo* GameRef, std::pair<int, int>DigSpotRange);

void Relic_1_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);//Increase radius by 1 plus (0 to 1) depeding on grade //Common
void Relic_2_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);//Decrease segments by 1 plus (0 to 6) depeding on grade //Common
void Relic_3_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);//Decrease dig time by 0.1 plus (0 to 0.1 depending on grade //Common
void Relic_4_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);//Decrease scenery extraction segments by 16 plus (0 to 16) depending on grade //Common
void Relic_5_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_6_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_7_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_8_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_9_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_10_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_11_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_12_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_13_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_14_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_15_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_16_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_17_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_18_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_19_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_20_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_21_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_22_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_23_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_24_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_25_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_26_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_27_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_28_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_29_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_30_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_31_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_32_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_33_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_34_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_35_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_36_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_37_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_38_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_39_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_40_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_41_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_42_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_43_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_44_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_45_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_46_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_47_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);
void Relic_48_Function(int Grade, Archo* GameRef, std::pair<int, int>DigSpotRange);