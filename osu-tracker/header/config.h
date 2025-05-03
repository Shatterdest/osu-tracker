#pragma once
#include <utility>
#include <string>
#include "console.h"

// static data
static std::string username;

std::vector<std::vector<std::string>> vec_application {
	{"key",					"name",					"value",	"desc",											   "secret"},
	{"osu_id",				"osu! User ID",			"",			"Your osu! user id.",								"false"},
	{"client_id",			"Client ID",			"",			"osu! API V2 Client ID.",							"false"},
	{"client_secret",		"Client Secret",		"",			"osu! API V2 Client Secret ( DO NOT SHARE )!",		 "true"},
	{"api_refreshInterval",	"API Refresh Interval",	"8",		"Time in (ms) till api fetches again in the loop.",	"false"}
};

std::vector<std::vector<std::string>> vec_api {
	{"name","url"},
	{
		// If people want to change to a private server and track their stats there
		// (if private server supports api)
		"osu",
		"https://osu.ppy.sh/api/v2/users/"
	},
	{
		"inspector",
		"https://api.kirino.sh/inspector/users/stats/{{osu_id}}?skipDailyData=true&skipOsuData=true&skipExtras=true"
	},
	{
		"respektive_user",
		"https://score.respektive.pw/u/{{osu_id}}"
	},
	{
		"respektive_rank",
		"https://score.respektive.pw/rank/{{scoreRank}}"
	}
};

std::vector<std::vector<std::string>> vec_tracker {

	{"key",			"name",				"group",			"path",				"display"},
	{"scoreRank",	"Score Rank",		"respektive_user",	"/0/rank",			"true"},
	{"scoreScore",	"res_RankedScore",	"respektive_user",	"/0/score",			"false"},
	{"level",		"Level",			"osu",				"",					"true"},
	{"rankedScore",	"Ranked Score",		"osu",				"",					"true"},
	{"totalScore",	"Total Score",		"osu",				"",					"true"},
	{"ppRank",		"Performance Rank",	"osu",				"",					"true"},
	{"pp",			"PP",				"osu",				"",					"true"},
	{"acc",			"Accuracy",			"osu",				"",					"true"},
	{"playtime",	"Play Time",		"osu",				"",					"true"},
	{"playcount",	"Play Count",		"osu",				"",					"true"},
	{"silverSS",	"Rank SSH",			"osu",				"",					"true"},
	{"goldSS",		"Rank SS",			"osu",				"",					"true"},
	{"silverS",		"Rank SH",			"osu",				"",					"true"},
	{"goldS",		"Rank S",			"osu",				"",					"true"},
	{"a",			"Rank A",			"osu",				"",					"true"},
	{"b",			"Rank B",			"inspector",		"/stats/b",			"true"},
	{"c",			"Rank C",			"inspector",		"/stats/c",			"true"},
	{"d",			"Rank D",			"inspector",		"/stats/d",			"true"},
	{"totalSS",		"Total SS",			"",					"",					"true"},
	{"totalS",		"Total S",			"",					"",					"true"},
	{"clears",		"Profile Clears",	"osu",				"",					"true"},
	{"clears",		"Total Clears",		"inspector",		"",					"true"},
	{"completionP",	"Completion%",		"inspector",		"/stats/completion","true"},
	{"targetRank",	"Target Rank",		"respektive_rank",	"/0/rank",			"true"},
	{"targetUser",	"Target User",		"respektive_rank",	"/0/username",		"true"},
	{"targetScore",	"Target Score",		"respektive_rank",	"/0/score",			"true"}
};

/*
0: key
1: init
2: current
3: change/diff
*/
std::vector<std::vector<std::string>> vec_data {
	{"scoreRank",		"",		"",		""},
	{"scoreScore",		"",		"",		""},
	{"rankedScore",		"",		"",		""},
	{"totalScore",		"",		"",		""},
	{"level",			"",		"",		""},
	{"pp",				"",		"",		""},
	{"playcount",		"",		"",		""},
	{"playtime",		"",		"",		""},
	{"silverSS",		"",		"",		""},
	{"goldSS",			"",		"",		""},
	{"silverS",			"",		"",		""},
	{"goldS",			"",		"",		""},
	{"a",				"",		"",		""},
	{"b",				"",		"",		""},
	{"c",				"",		"",		""},
	{"d",				"",		"",		""},
	{"totalSS",			"",		"",		""},
	{"totalS",			"",		"",		""},
	{"completionP",		"",		"",		""},
	{"targetRank",		"",		"",		""},
	{"targetUser",		"",		"",		""},
	{"targetScore",		"",		"",		""}
};

// STATS READ/WRITE
int setStats(std::string key, std::string value) {
	for (size_t i = 1; i < vec_data.size(); i++) {
		if (vec_data[i][0] == key) {
			vec_data[i][1] = value;
			return 0;
		}
	}
	return -1;
}

std::string getStats(std::string key) {
	for (size_t i = 1; i < vec_data.size(); i++) {
		if (vec_data[i][0] == key) {
			return vec_data[i][1];
		}
	}
	return "";
}

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

// config file handling
void writeConfig() {
	writeLog("Writing config file...");
	std::string input;
	input += "//PLEASE DONT MANUALLY EDIT THIS FILE\n"
			 "//(you still can,but you might break it)\n"
			 "//(If you want to reset all settings, just delete this file)\n\n";
	input += "[ApplicationConfig]\n";
	for (int i = 1; i < vec_application.size(); i++) {
		// 0 = key : 0
		// 2 = value : 1
		input += vec_application[i][0] + ";" + vec_application[i][2] + "\n";
	}
	input += "\n";
	input += "[ApiConfig]\n";
	for (int i = 1; i < vec_api.size(); i++) {
		// 0 = key : 0
		// 1 = url : 1
		input += vec_api[i][0] + ";" + vec_api[i][1] + "\n";
	}
	input += "\n";
	input += "[TrackerConfig]\n";
	for (int i = 1; i < vec_tracker.size(); i++) {
		// 0 = key : 0
		// 4 = display : 1
		input += vec_tracker[i][0] + ";" + vec_tracker[i][4] + "\n";
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
				if (line == "[ApiConfig]") {
					configHeader = 1;
				}
				if (line == "[TrackerConfig]") {
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
					setConfig(vec_api, configLine[0], "url", configLine[1]);
					break;
				case 2:
					setConfig(vec_tracker, configLine[0], "display", configLine[1]);
					break;
				}
			}
		}
		file.close();
	}
	else {
		writeLog("Error reading config file", 255,0,0);
	}
}

void rmConfig() {
	std::filesystem::remove("config.txt");
	setConfig(vec_application, "osu_id", "value", "");
	setConfig(vec_application, "client_id", "value", "");
	setConfig(vec_application, "client_secret", "value", "");
	setConfig(vec_application, "api_refreshInterval", "value", "8000");
	writeConfig();
}

// FEATURE: for custom api settings
void removeConfig(std::vector<std::vector<std::any>>& vec, std::string key) {

}

// FEATURE: for custom api settings
void addConfig() {

}