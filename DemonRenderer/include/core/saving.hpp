/** \file saving.hpp */

#pragma once
//#include "DemonRenderer.hpp"
#include <vector>
#include <array>
#include <utility>

//* * * * * * * * * * * * * * * * * * * * 
// Settings Serialization and Saving
//* * * * * * * * * * * * * * * * * * * * 

struct Settings_Save {
	float s_MouseSensitivity{ 1.0f };
	bool s_Fullscreen{ true };
	int s_ResolutionFract{ 1 };
};

void Save_Settings(const Settings_Save& sS);

Settings_Save Load_Settings();

//* * * * * * * * * * * * * * * * * * * * 
// Game Save Serialization and Saving
//* * * * * * * * * * * * * * * * * * * * 


struct Game_Save {
	std::vector<std::pair<int,int>> s_Items;
	std::array<int, 3> s_Equiped{ -1,-1,-1 };
};

void Save_Game(const Game_Save& sS);

Game_Save Load_Game();