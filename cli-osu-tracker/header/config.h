#pragma once
#include <utility>
#include <string>

std::vector<std::vector<std::string>> vec_application {
	{"key",					"name",					"value",	"desc",											   "secret"},
	{"osu_id",				"User ID",				"",			"Your osu! user id.",								"false"},
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
		"https://api.kirino.sh/inspector/users/stats/{osu_id}?skipDailyData=true&skipOsuData=true&skipExtras=true"
	},
	{
		"respektive_user",
		"https://score.respektive.pw/u/{osu_id}"
	},
	{
		"respektive_rank",
		"https://score.respektive.pw/rank/{scoreRank}"
	}
};

std::vector<std::vector<std::string>> vec_tracker {

	{"key",			"name",				"output",	"group",			"path",				"track",	"display"},
	{"scoreRank",	"Score Rank",		"int",		"respektive_user",	"/0/rank",			"true",		"true"},
	{"scoreScore",	"res_RankedScore",	"int",		"respektive_user",	"/0/score",			"false",	"false"},
	{"rankedScore",	"Ranked Score",		"int",		"osu",				"",					"true",		"true"},
	{"totalScore",	"Total Score",		"int",		"osu",				"",					"true",		"true"},
	{"level",		"Lvl",				"float",	"osu",				"",					"true",		"true"},
	{"pp",			"PP",				"int",		"osu",				"",					"true",		"true"},
	{"silverSS",	"Silver SS",		"int",		"osu",				"",					"true",		"true"},
	{"goldSS",		"Gold SS",			"int",		"osu",				"",					"true",		"true"},
	{"silverS",		"Silver S",			"int",		"osu",				"",					"true",		"true"},
	{"goldS",		"Gold S",			"int",		"osu",				"",					"true",		"true"},
	{"a",			"A",				"int",		"osu",				"",					"true",		"true"},
	{"playcount",	"Playcount",		"int",		"osu",				"",					"true",		"true"},
	{"b",			"B",				"int",		"inspector",		"/stats/b",			"true",		"true"},
	{"c",			"C",				"int",		"inspector",		"/stats/c",			"true",		"true"},
	{"d",			"D",				"int",		"inspector",		"/stats/d",			"true",		"true"},
	{"completionP",	"Completion%",		"float",	"inspector",		"/stats/completion","true",		"true"},
	{"targetRank",	"Target Rank",		"string",	"respektive_rank",	"/0/rank",			"true",		"true"},
	{"targetUser",	"Target User",		"string",	"respektive_rank",	"/0/username",		"true",		"true"},
	{"targetScore",	"Target Score",		"string",	"respektive_rank",	"/0/score",			"true",		"true"}
};

std::vector<std::vector<std::string>> vec_data {
	{"init_scoreRank",	""},
	{"scoreRank",		""},
	{"init_scoreScore",	""},
	{"scoreScore",		""},
	{"init_rankedScore",""},
	{"rankedScore",		""},
	{"init_totalScore",	""},
	{"totalScore",		""},
	{"init_level",		""},
	{"level",			""},
	{"init_pp",			""},
	{"pp",				""},
	{"init_silverSS",	""},
	{"silverSS",		""},
	{"init_goldSS",		""},
	{"goldSS",			""},
	{"init_silverS",	""},
	{"silverS",			""},
	{"init_goldS",		""},
	{"goldS",			""},
	{"init_a",			""},
	{"a",				""},
	{"init_playcount",	""},
	{"playcount",		""},
	{"init_b",			""},
	{"b",				""},
	{"init_c",			""},
	{"c",				""},
	{"init_d",			""},
	{"d",				""},
	{"init_completionP",""},
	{"completionP",		""},
	{"init_targetRank",	""},
	{"targetRank",		""},
	{"init_targetUser",	""},
	{"targetUser",		""},
	{"init_targetScore",""},
	{"targetScore",		""}
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
	for (size_t i = 1; i < vec.size(); i++) {
		if (vec[i][0] == key) {
			for (size_t j = 0; j < vec[i].size(); j++) {
				if (vec[0][j] == field) {
					vec[i][j] = value;
					return 0;
				}
			}
		}
	}
	return -1;
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
		// 5 = track : 1
		// 6 = display : 2
		input += vec_tracker[i][0] + ";" + vec_tracker[i][5] + ";" + vec_tracker[i][6] + "\n";
	}
	
	std::ofstream file;
	file.open("config.txt");
	file << input;
	file.close();
}

// read config
void readConfig() {
	std::ifstream file("config.txt");
	if (file.is_open()) {
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
					setConfig(vec_tracker, configLine[0], "track", configLine[1]);
					setConfig(vec_tracker, configLine[0], "display", configLine[2]);
					break;
				}
			}
		}
		file.close();
	}
}

// checks if config file exists
bool checkConfig() {
	if (!std::filesystem::exists("config.txt")) {
		return false;
	}
	return true;
}

// FEATURE: for custom api settings
void removeConfig(std::vector<std::vector<std::any>>& vec, std::string key) {

}

// FEATURE: for custom api settings
void addConfig() {

}