/** \file saving.hpp */

#pragma once
//#include "DemonRenderer.hpp"
#include <vector>

//* * * * * * * * * * * * * * * * * * * * 
// Settings Serialization and Saving
//* * * * * * * * * * * * * * * * * * * * 

struct Settings_Save {
	float s_MouseSensitivity{ 1.0f };
	bool s_Fullscreen{ true };
};

void Save_Settings(const Settings_Save& sS);

Settings_Save Load_Settings();

//* * * * * * * * * * * * * * * * * * * * 
// Game Save Serialization and Saving
//* * * * * * * * * * * * * * * * * * * * 

struct Game_Save {
	std::vector<int> s_Items;
};

void Save_Game(const Game_Save& sS);

Game_Save Load_Game();