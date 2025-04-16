/** \file saving.hpp */
#include "core/saving.hpp"
#include "core/log.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <string>

//* * * * * * * * * * * * * * * * * * * * 
// Settings Serialization and Saving
//* * * * * * * * * * * * * * * * * * * * 

void to_json(nlohmann::json& j, const Settings_Save& sS) {
	j = nlohmann::json{
		{"MouseSensitivity", sS.s_MouseSensitivity}
	};	
	j = nlohmann::json{
		{"Fullscreen", sS.s_Fullscreen}
	};
}

void from_json(const nlohmann::json& j, Settings_Save& sS) {

	if (j.contains("MouseSensitivity")) {
		j.at("MouseSensitivity").get_to(sS.s_MouseSensitivity);
	}
	else {
		sS.s_MouseSensitivity = 1.0f;
	}

	if (j.contains("Fullscreen")) {
		j.at("Fullscreen").get_to(sS.s_Fullscreen);
	}
	else {
		sS.s_Fullscreen = true;
	}
}

void Save_Settings(const Settings_Save& sS) {
	nlohmann::json j = sS;

	std::string filePath = "./saves/Settings.json";
	std::ofstream outputFile(filePath);

	if (outputFile.is_open()) {
		outputFile << j.dump();
		outputFile.close();
	}
	else {
		spdlog::error("Failed to open file: {}", filePath);

		std::filesystem::path newPath(filePath);
		std::filesystem::create_directories(newPath.parent_path());

		outputFile = std::ofstream(filePath);

		if (outputFile.is_open()) {
			outputFile << j.dump();
			outputFile.close();
		}
		else {
			spdlog::error("Failed to create file: {}", filePath);
		}
	}
}

Settings_Save Load_Settings() {
	nlohmann::json j;

	std::string filePath = "./saves/Settings.json";
	std::ifstream inputFile(filePath);

	if (inputFile.is_open()) {
		inputFile >> j;
		inputFile.close();
	}
	else {
		spdlog::error("Failed to open file: {}", filePath);
		return Settings_Save();
	}

	return j.get<Settings_Save>();
}

//* * * * * * * * * * * * * * * * * * * * 
// Game Save Serialization and Saving
//* * * * * * * * * * * * * * * * * * * * 

void to_json(nlohmann::json& j, const Game_Save& gS) {
	j = nlohmann::json{
		{"Items", gS.s_Items}
	};
}

void from_json(const nlohmann::json& j, Game_Save& gS) {
	if (j.contains("Items")) {
		j.at("Items").get_to(gS.s_Items);
	}
	else {
		gS.s_Items = std::vector<int>();
	}
}

void Save_Game(const Game_Save& sS) {
	nlohmann::json j = sS;

	std::string filePath = "./saves/Game.json";
	std::ofstream outputFile(filePath);

	if (outputFile.is_open()) {
		outputFile << j.dump();
		outputFile.close();
	}
	else {
		spdlog::error("Failed to open file: {}", filePath);

		std::filesystem::path newPath(filePath);
		std::filesystem::create_directories(newPath.parent_path());

		outputFile = std::ofstream(filePath);

		if (outputFile.is_open()) {
			outputFile << j.dump();
			outputFile.close();
		}
		else {
			spdlog::error("Failed to create file: {}", filePath);
		}
	}
}

Game_Save Load_Game() {
	nlohmann::json j;

	std::string filePath = "./saves/Game.json";
	std::ifstream inputFile(filePath);

	if (inputFile.is_open()) {
		inputFile >> j;
		inputFile.close();
	}
	else {
		spdlog::error("Failed to open file: {}", filePath);
		return Game_Save();
	}

	return j.get<Game_Save>();
}