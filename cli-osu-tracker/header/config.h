#pragma once
#include <utility>
#include <string>

static const std::vector<std::vector<std::any>> vec_application_default{
	{"key",					"name",					"type",		"def",	"description",										"secret"},
	
	{"osu_id",				"User ID",				"int",		"",		"Your osu! user id.",								false},
	{"client_id",			"Client ID",			"int",		"",		"osu! API V2 Client ID.",							false},
	{"client_secret",		"Client Secret",		"string",	"",		"osu! API V2 Client Secret ( DO NOT SHARE )!",		true},
	{"api_refreshInterval",	"API Refresh Interval",	"int",		"8",	"Time in (ms) till api fetches again in the loop.",	false}
};

std::vector<std::vector<std::any>> vec_application{
	{"key", "name", "type", "def","description", "secret"}
};

static const std::vector<std::vector<std::any>> vec_api_default{
	{"name","url"},
	{
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

std::vector<std::vector<std::any>> vec_api{
	// group, url
	{"name","url"}
};

static const std::vector<std::vector<std::any>> vec_tracker_default{

	{"key",			"name",				"type",		"group",			"path",				"track","display"},

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

std::vector<std::vector<std::any>> vec_tracker{
	{"key", "name",	"type", "group", "path", "track", "display"}
};


void setConfig(std::vector<std::vector<std::any>> vec, std::string key, std::string value) {
	//for (int i = 0; i < s_config.size(); i++) {
	//
	//}
}

std::any getConfig(std::vector<std::vector<std::any>>& vec, std::string key, std::string field) {
	std::vector<std::any> fields = vec[0];

	for (int i = 1; i < vec.size(); i++) {
		if(vec[i])
	}
	return;
}

// config file handling
void createConfig(bool save) {
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