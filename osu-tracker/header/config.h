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

enum data_type {
	t_string = 0,
	t_int = 1,
	t_decimal  = 2
};

enum format_type {
	f_string = 0,
	f_int = 1,
	f_decimal = 2,
	f_rank = 3,
	f_time = 4,
};

struct data_entry {
	std::string key;
	std::string name;
	std::string init;
	std::string current;
	std::string change;
	data_type	type;
	format_type format;
	bool display;
	bool bancho;
	bool titanic;
};

std::vector<data_entry> vec_data{
	{"level",		"Level",			"", "",	"", data_type::t_decimal,	format_type::f_decimal, true,	true,	true},	// 0
	{"rankedScore",	"Ranked Score",		"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 1
	{"totalScore",	"Total Score",		"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 2
	{"ppRank",		"PP Rank",			"", "",	"", data_type::t_int,		format_type::f_rank,	true,	true,	true},	// 3
	{"pp",			"PP",				"", "",	"", data_type::t_decimal,	format_type::f_decimal, true,	true,	true},	// 4
	{"ppv1",		"PPv1",				"", "",	"", data_type::t_decimal,	format_type::f_decimal, true,	false,	true},	// 5
	{"acc",			"Accuracy",			"", "",	"", data_type::t_decimal,	format_type::f_decimal, true,	true,	true},	// 6
	{"playtime",	"Play Time",		"", "",	"", data_type::t_int,		format_type::f_time,	true,	true,	true},	// 7
	{"playcount",	"Play Count",		"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 8
	{"totalHits",	"Total Hits",		"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 9
					    
	{"silverSS",	"Rank SSH",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 10
	{"goldSS",		"Rank SS",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 11
	{"silverS",		"Rank SH",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 12
	{"goldS",		"Rank S",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 13
	{"a",			"Rank A",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 14
	{"b",			"Rank B",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 15
	{"c",			"Rank C",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 16
	{"d",			"Rank D",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 17
	{"totalSS",		"Total SS",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 18
	{"totalS",		"Total S",			"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 19
					    
	{"clears",		"Profile Clears",	"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 20
	{"totalClears",	"Total Clears",		"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 21
	{"completion",	"Completion%",		"", "",	"", data_type::t_decimal,	format_type::f_decimal, true,	true,	true},	// 22
	{"scoreRank",	"Score Rank",		"", "",	"", data_type::t_int,		format_type::f_rank,	true,	true,	true},	// 23
					    
	{"targetRank",	"Target Rank",		"", "",	"", data_type::t_int,		format_type::f_rank,	true,	true,	true},	// 24
	{"targetUser",	"Target Player",	"", "",	"", data_type::t_string,	format_type::f_string,	true,	true,	true},	// 25
	{"targetScore",	"Target Score",		"", "",	"", data_type::t_int,		format_type::f_int,		true,	true,	true},	// 26

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