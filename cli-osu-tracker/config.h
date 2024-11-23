#pragma once
#include "ext.h"
#include <utility>
#include <string>
#include <format>  // C++23 for string 

// Example struct
struct Example {
	int id;
	double value;
	std::string name;

	// Constructor
	Example(int i, double v, std::string n) : id(i), value(v), name(n) {}
};

// Helper function to get member names and values from the struct using reflection
template <typename T>
auto getStructMembers(const T& obj) {
	std::vector<std::vector<std::string>> result;

	// Use reflection to iterate over the members of the struct
	for (const auto& member : std::reflect::members<T>()) {
		// Member name and value
		auto name = std::string(member.name());
		auto value = std::format("{}", member.get(obj));  // Formatting the member value

		result.push_back({ name, value });
	}

	return result;
}


struct s_application_config {
	std::string key;			// key
	std::string name;			// name
	std::string type;			// type
	std::string def;			// default value upon config creation
	std::string description;	// description
	bool secret;				// if value should be shown in console or not
	// value is still plaintext in config file - DO NOT SHARE
};

struct s_tracker_config {
	std::string key;		// key of name
	std::string name;		// name of value
	std::string type;		// type of the json value
	std::string group;		// group name
	std::string path;		// json path
	bool track;				// should be tracked or not
	bool display;			// display in console or not
};

struct s_api_endpoints {
	std::string name;	// group name
	std::string url;	// api url
};

std::vector<s_application_config> vec_application{
	// Key					Name					Type	 Default	Description											Secret
	{"osu_id",				"User ID",				"int",		"",		"Your osu! user id.",								false},
	{"client_id",			"Client ID",			"int",		"",		"osu! API V2 Client ID.",							false},
	{"client_secret",		"Client Secret",		"string",	"",		"osu! API V2 Client Secret ( DO NOT SHARE )!",		true},
	{"api_refreshInterval",	"API Refresh Interval",	"int",		"8",	"Time in (ms) till api fetches again in the loop.",	false}
};

std::vector<s_api_endpoints> vec_api{
	// group, url
	{
		"osu",
		"https://osu.ppy.sh/api/v2/users/"
	},
	{
		"inspector",
		"https://api.kirino.sh/inspector/users/stats/"
	},
	{
		"respektive_user",
		"https://score.respektive.pw/u/{osu_id}"
	},
	{
		"respektive_rank",
		"https://score.respektive.pw/rank/{scoreRank}"
	},
};

std::vector<s_tracker_config> vec_tracker{
	// key			 name				 type		 group				jsonPath			track?	display?
	{"scoreRank",	"Score Rank",		"int",		"respektive_user",	"/0/rank",			true,	true},
	{"scoreScore",	"res_RankedScore",	"int",		"respektive_user",	"/0/score",			false,	false},
	{"rankedScore",	"Ranked Score",		"int",		"osu",				"",					true,	true},
	{"totalScore",	"Total Score",		"int",		"osu",				"",					true,	true},
	{"level",		"Lvl",				"float",	"osu",				"",					true,	true},
	{"pp",			"PP",				"int",		"osu",				"",					true,	true},
	{"silverSS",	"Silver SS",		"int",		"osu",				"",					true,	true},
	{"goldSS",		"Gold SS",			"int",		"osu",				"",					true,	true},
	{"silverS",		"Silver S",			"int",		"osu",				"",					true,	true},
	{"goldS",		"Gold S",			"int",		"osu",				"",					true,	true},
	{"a",			"A",				"int",		"osu",				"",					true,	true},
	{"b",			"B",				"int",		"inspector",		"/stats/b",			true,	true},
	{"c",			"C",				"int",		"inspector",		"/stats/c",			true,	true},
	{"d",			"D",				"int",		"inspector",		"/stats/d",			true,	true},
	{"playcount",	"Playcount",		"int",		"osu",				"",					true,	true},
	{"completionP",	"Completion%",		"float",	"inspector",		"/stats/completion",true,	true},
	{"targetRank",	"Target Rank",		"string",	"respektive_rank",	"/0/rank",			true,	true},
	{"targetUser",	"Target User",		"string",	"respektive_rank",	"/0/username",		true,	true},
	{"targetScore",	"Target Score",		"string",	"respektive_rank",	"/0/score",			true,	true}
};

std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != std::string::npos) {
		result.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start));

	return result;
}

std::string boolToString(bool b) {
	return b ? "true" : "false";
}

// get set values
template <typename T>
void setConfig(std::string key, std::string value) {
	//for (int i = 0; i < s_config.size(); i++) {
	//
	//}
}

template <typename T>
std::string getConfig(std::vector<T> vec, std::string key, std::string type = "") {
	for (auto entry : vec) {
		
	}
	return "";
}

// config file handling
void createConfig() {
	//std::string input;
	//input += "//PLEASE DONT MANUALLY EDIT THIS FILE\n//(you still can,but you might break it)\n";
	//input += "[Config]\n";
	//for (int i = 0; i < s_config.size(); i++) {
	//	input += "\"" + s_config[i].first + "\"=" + s_config[i].second + "\n";
	//}
	//input += "\n";
	//input += "[Toggle]\n";
	//for (int i = 0; i < s_toggle.size(); i++) {
	//	input += s_toggle[i].first + "=" + boolToString(s_toggle[i].second) + "\n";
	//}
	//std::ofstream file;
	//file.open("config.txt");
	//file << input;
	//file.close();
}

void readConfig() {
	std::ifstream file("config.txt");
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (!line.starts_with("//") || !line.empty() || !line.starts_with("[")) {
				if (line[0] == '"') {
					std::string key = split(line, '"')[0];
					std::string val = split(line, '=')[1];
					//setConfig(key, val);
				}
				else {
					std::vector<std::string> temp = split(line, '=');
					if (temp[1] == "true") {
						//setToggle(temp[0], true);
					}
					else {
						//setToggle(temp[0], false);
					}
				}
			}
		}
		file.close();
	}
}

bool checkConfig() {
	if (!std::filesystem::exists("config.txt")) {
		return false;
	}
	return true;
}