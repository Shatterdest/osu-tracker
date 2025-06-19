#pragma once
#include <utility>
#include <string>
#include "console.h"

class config {
public:
	enum class gameMode {
		osu = 0,
		taiko = 1,
		fruits = 2,
		mania = 3
	};

	enum class server {
		bancho = 0,
		titanic = 1
	};

	struct application {
		std::string username = "";
		int osuId = 0;
		int clientId = 0;
		std::string clientSecret = "";
		int apiInterval = 7000;
		gameMode mode = gameMode::osu;
		server server = server::bancho;

		std::vector<std::tuple<std::string, std::string>> toArray() const {
			return {
				{"username", username}
				,{"osuId", std::to_string(osuId)}
				,{"clientId", std::to_string(clientId)}
				,{"clientSecret", clientSecret}
				,{"apiInterval", std::to_string(apiInterval)}
				,{"mode", std::to_string(static_cast<int>(mode))}
				,{"server", std::to_string(static_cast<int>(server))}
			};
		}
	};

	enum class dataType {
		t_string = 0,
		t_int = 1,
		t_decimal = 2
	};

	enum class formatType {
		f_string = 0,
		f_int = 1,
		f_decimal = 2,
		f_rank = 3,
		f_time = 4,
	};

	struct dataEntry {
		std::string key;
		std::string name;
		std::string init;
		std::string current;
		std::string change;
		dataType dataType;
		formatType formatType;
		bool display;
		bool banchoSupport;
		bool titanicSupport;

		std::vector<std::tuple<std::string, std::string>> toArray() const {
			return {
				{"key", key}
				,{"name", name}
				,{"init", init}
				,{"current", current}
				,{"change", change}
				,{"dataType", std::to_string(static_cast<int>(dataType))}
				,{"formatType", std::to_string(static_cast<int>(formatType))}
				,{"display", boolToString(display)}
				,{"banchoSupport", boolToString(banchoSupport)}
				,{"titanicSupport", boolToString(titanicSupport)}
			};
		}
	};

	std::vector<dataEntry> vecData {
		{"level",		"Level",			"", "",	"", dataType::t_decimal,	formatType::f_decimal,	true,	true,	true}
		,{"rankedScore","Ranked Score",		"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"totalScore",	"Total Score",		"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"ppRank",		"PP Rank",			"", "",	"", dataType::t_int,		formatType::f_rank,		true,	true,	true}
		,{"pp",			"PP",				"", "",	"", dataType::t_decimal,	formatType::f_decimal,	true,	true,	true}
		,{"ppv1",		"PPv1",				"", "",	"", dataType::t_decimal,	formatType::f_decimal,	true,	false,	true}
		,{"acc",		"Accuracy",			"", "",	"", dataType::t_decimal,	formatType::f_decimal,	true,	true,	true}
		,{"playtime",	"Play Time",		"", "",	"", dataType::t_int,		formatType::f_time,		true,	true,	true}
		,{"playcount",	"Play Count",		"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"totalHits",	"Total Hits",		"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}

		,{"silverSS",	"Rank SSH",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"goldSS",		"Rank SS",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"silverS",	"Rank SH",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"goldS",		"Rank S",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"a",			"Rank A",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"b",			"Rank B",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"c",			"Rank C",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"d",			"Rank D",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"totalSS",	"Total SS",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"totalS",		"Total S",			"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}

		,{"clears",		"Profile Clears",	"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"totalClears","Total Clears",		"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
		,{"completion",	"Completion%",		"", "",	"", dataType::t_decimal,	formatType::f_decimal,	true,	true,	true}
		,{"scoreRank",	"Score Rank",		"", "",	"", dataType::t_int,		formatType::f_rank,		true,	true,	true}

		,{"targetRank",	"Target Rank",		"", "",	"", dataType::t_int,		formatType::f_rank,		true,	true,	true}
		,{"targetUser",	"Target Player",	"", "",	"", dataType::t_string,		formatType::f_string,	true,	true,	true}
		,{"targetScore","Target Score",		"", "",	"", dataType::t_int,		formatType::f_int,		true,	true,	true}
	};
};

// write config
void writeConfig() {
	writeLog("Writing config file...");
	std::string input;
	input += "//PLEASE DONT MANUALLY EDIT THIS FILE\n"
			 "//(you still can,but you might break it)\n"
			 "//(If you want to reset all settings, just delete this file)\n\n";
	input += "[ApplicationConfig]";
	for (const auto& [key, value] : config::application{}.toArray()) {
		input += "\n" + key + ";" + value;
	}
	input += "\n\n[TrackerConfig]";
	for (const auto& _vecData : config::vecData.size()) {
		// 0 = key : 0
		// 2 = display : 1
		input += vec_tracker[i][0] + ";" + vec_tracker[i][2] + "\n";
	}
	input += "\n";
	input += "[TrackerConfigTitanic]\n";
	for (int i = 1; i < vec_tracker_titanic.size(); i++) {
		// 0 = key : 0
		// 2 = display : 1
		input += vec_tracker_titanic[i][0] + ";" + vec_tracker_titanic[i][2] + "\n";
	}
	
	std::ofstream file;
	file.open("config.txt");
	file << input;
	file.close();
	writeLog("Config written");
}

// read config
void readConfig() {
	std::ifstream file("config.txt");
	if (file.is_open()) {
		writeLog("Reading config file...");
		std::string line;
		int configHeader = -1;
		std::vector<std::string> configLine;
		while (std::getline(file, line)) {
			if (line.starts_with("[")) {
				if (line == "[ApplicationConfig]") {
					configHeader = 0;
				}
				if (line == "[TrackerConfig]") {
					configHeader = 1;
				}
				if (line == "[TrackerConfigTitanic]") {
					configHeader = 2;
				}
			}
			if (!line.starts_with("//") && !line.empty() && !line.starts_with("[")) {
				configLine = split(line, ';');
				switch (configHeader)
				{
					case 0:
						setConfig(vec_application, configLine[0],"value", configLine[1]);
						break;
					case 1:
						setConfig(vec_tracker, configLine[0], "display", configLine[1]);
						break;
					case 2:
						setConfig(vec_tracker_titanic, configLine[0], "display", configLine[1]);
						break;
				}
			}
		}
		file.close();
	}
	else {
		writeLog("Error reading config file",true, 255,0,0);
	}
}

void rmConfig() {
	std::filesystem::remove("config.txt");
}