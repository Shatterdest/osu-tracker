#pragma once
#include <utility>
#include <string>
#include "console.h"

// static data
static std::string username;

std::vector<std::vector<std::string>> vec_application {
	{"key","value"}, // 0
	{"osu_id",""},// 1
	{"client_id",""},// 2
	{"client_secret",""},// 3
	{"api_refreshInterval","7000"},// 4
	{"gameMode","0"}, // 5: osu, taiko, fruits, mania
	{"server","0"} // 6: [0: bancho], [1: titanic]
};

/*
0: key
1: init
2: current
3: change/diff
4: type -> string, int, float
*/
std::vector<std::vector<std::string>> vec_data_osu {
	{"level",		"",	"",	"", "2"}, // 0
	{"rankedScore",	"",	"",	"", "1"}, // 1
	{"totalScore",	"",	"",	"", "1"}, // 2
	{"ppRank",		"",	"",	"", "1"}, // 3
	{"pp",			"",	"",	"", "2"}, // 4
	{"acc",			"",	"",	"", "2"}, // 5
	{"playtime",	"",	"",	"", "1"}, // 6
	{"playcount",	"",	"",	"", "1"}, // 7
	
	{"silverSS",	"",	"",	"", "1"}, // 8
	{"goldSS",		"",	"",	"", "1"}, // 9
	{"silverS",		"",	"",	"", "1"}, // 10
	{"goldS",		"",	"",	"", "1"}, // 11
	{"a",			"",	"",	"", "1"}, // 12

	{"totalSS",		"",	"",	"", "1"}, // 13
	{"totalS",		"",	"",	"", "1"}, // 14
	{"clears",		"",	"",	"", "1"}, // 15
};

/*
0: key
1: init
2: current
3: change/diff
4: type -> string, int, float
*/
std::vector<std::vector<std::string>> vec_data_inspector {
	{"b",			"",	"",	"", "1"}, // 0
	{"c",			"",	"",	"", "1"}, // 1
	{"d",			"",	"",	"", "1"}, // 2
	{"totalClears",	"",	"",	"", "1"}, // 3
	{"completion",	"",	"",	"", "2"}  // 4
};

/*
0: key
1: init
2: current
3: change/diff
4: type -> string, int, float
*/
std::vector<std::vector<std::string>> vec_data_respektive{
	{"scoreRank","","","","1"} // 0
};

/*
0: key
1: current
2: type -> string, int, float
*/
std::vector<std::vector<std::string>> vec_data_respektive_target {
	{"targetRank",	"","1"}, // 0
	{"targetUser",	"","0"}, // 1
	{"targetScore",	"","1"}  // 2
};

std::vector<std::vector<std::string>> vec_tracker{
	{"key",			"name",				"display"}, // 0
	{"scoreRank",	"Score Rank",		"true"}, 	// 1
	{"level",		"Level",			"true"},	// 2
	{"rankedScore",	"Ranked Score",		"true"},	// 3
	{"totalScore",	"Total Score",		"true"},	// 4
	{"ppRank",		"Performance Rank",	"true"},	// 5
	{"pp",			"PP",				"true"},	// 6
	{"acc",			"Accuracy",			"true"},	// 7
	{"playtime",	"Play Time",		"true"},	// 8
	{"playcount",	"Play Count",		"true"},	// 9
	{"silverSS",	"Rank SSH",			"true"},	// 10
	{"goldSS",		"Rank SS",			"true"},	// 11
	{"silverS",		"Rank SH",			"true"},	// 12
	{"goldS",		"Rank S",			"true"},	// 13
	{"a",			"Rank A",			"true"},	// 14
	{"b",			"Rank B",			"true"},	// 15
	{"c",			"Rank C",			"true"},	// 16
	{"d",			"Rank D",			"true"},	// 17
	{"totalSS",		"Total SS",			"true"},	// 18
	{"totalS",		"Total S",			"true"},	// 19
	{"clears",		"Profile Clears",	"true"},	// 20
	{"totalClears",	"Total Clears",		"true"},	// 21
	{"completion",	"Completion%",		"true"},	// 22
	{"targetRank",	"NextScoreRank ",	"true"},	// 23
};

// private server specific data
std::vector<std::vector<std::string>> vec_tracker_ps{
	{"key",			"name",				"display"}, // 0
	{"scoreRank",	"Score Rank",		"true"}, 	// 1
	{"level",		"Level",			"true"},	// 2
	{"rankedScore",	"Ranked Score",		"true"},	// 3
	{"totalScore",	"Total Score",		"true"},	// 4
	{"ppRank",		"Performance Rank",	"true"},	// 5
	{"pp",			"PP",				"true"},	// 6
	{"acc",			"Accuracy",			"true"},	// 7
	{"playtime",	"Play Time",		"true"},	// 8
	{"playcount",	"Play Count",		"true"},	// 9
	{"silverSS",	"Rank SSH",			"true"},	// 10
	{"goldSS",		"Rank SS",			"true"},	// 11
	{"silverS",		"Rank SH",			"true"},	// 12
	{"goldS",		"Rank S",			"true"},	// 13
	{"a",			"Rank A",			"true"},	// 14
	{"b",			"Rank B",			"true"},	// 15
	{"c",			"Rank C",			"true"},	// 16
	{"d",			"Rank D",			"true"},	// 17
	{"totalSS",		"Total SS",			"true"},	// 18
	{"totalS",		"Total S",			"true"},	// 19
	{"clears",		"Profile Clears",	"true"},	// 20
	{"totalClears",	"Total Clears",		"true"},	// 21
	{"completion",	"Completion%",		"true"},	// 22
	{"targetRank",	"NextScoreRank ",	"true"},	// 23
};


/*
*	1: vector that has string vectors
*	2: key string
*	3: field string
*	4: value to set
*/
int setConfig(std::vector<std::vector<std::string>>& vec, std::string key, std::string field, std::string value) {
	int keyIndex = -1;
	int fieldIndex = -1;

	for (size_t i = 0; i < vec[0].size(); i++) {
		if (vec[0][i] == field) {
			fieldIndex = i;
			break;
		}
	}
	for (size_t i = 1; i < vec.size(); i++) {
		if (vec[i][0] == key) {
			keyIndex = i;
			break;
		}
	}

	if (keyIndex == -1 || fieldIndex == -1) {
		return -1;
	}
	else {
		vec[keyIndex][fieldIndex] = value;
		return 0;
	}
}

/*
*	1: vector that has string vectors
*	2: key string
*	3: field string
*/
std::string getConfig(std::vector<std::vector<std::string>>& vec, std::string key, std::string field) {
	for (size_t i = 1; i < vec.size(); i++) {
		if (vec[i][0] == key) {
			for (size_t j = 0; j < vec[i].size(); j++) {	
				if (vec[0][j] == field) {
					return vec[i][j];
				}
			}
		}
	}
	return "";
}

// write config
void writeConfig() {
	writeLog("Writing config file...");
	std::string input;
	input += "//PLEASE DONT MANUALLY EDIT THIS FILE\n"
			 "//(you still can,but you might break it)\n"
			 "//(If you want to reset all settings, just delete this file)\n\n";
	input += "[ApplicationConfig]\n";
	for (int i = 1; i < vec_application.size(); i++) {
		// 0 = key : 0
		// 1 = value : 1
		input += vec_application[i][0] + ";" + vec_application[i][1] + "\n";
	}
	input += "\n";
	input += "[TrackerConfig]\n";
	for (int i = 1; i < vec_tracker.size(); i++) {
		// 0 = key : 0
		// 2 = display : 1
		input += vec_tracker[i][0] + ";" + vec_tracker[i][2] + "\n";
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

// FEATURE: for custom api settings
void removeConfig(std::vector<std::vector<std::any>>& vec, std::string key) {

}

// FEATURE: for custom api settings
void addConfig() {

}