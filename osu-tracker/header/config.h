#pragma once
#include <utility>
#include <string>
#include "console.h"
#include <algorithm>

class config {
public:
	enum class gameMode {
		osu = 0
		,taiko = 1
		,fruits = 2
		,mania = 3
	};

	enum class server {
		bancho = 0
		,titanic = 1
	};

	struct application {
		// definition and initialization
		#define applicationFields(_field) \
			_field(int, osuId, 0) \
			_field(int, clientId, 0) \
			_field(std::string, clientSecret, "") \
			_field(int, apiInterval, 7000) \
			_field(config::gameMode, mode, config::gameMode::osu) \
			_field(config::server, server, config::server::bancho)

		static application& instance() {
			static application ctx;
			return ctx;
		}

		#define DECLARE(type, name, default_val) type name = default_val;
			applicationFields(DECLARE)
		#undef DECLARE

		// Convert to array
		std::vector<std::tuple<std::string, std::string>> toArray() const {
			std::vector<std::tuple<std::string, std::string>> result;
			#define TO_STRING(type, name, default_val) result.emplace_back(#name, to_string(name));
				applicationFields(TO_STRING)
			#undef TO_STRING
				return result;
		}
		std::string get(const std::string& key) const {
			#define GET_CASE(type, name, default_val) if (key == #name) return to_string(name);
				applicationFields(GET_CASE)
			#undef GET_CASE
			throw std::invalid_argument("Invalid key: " + key);
		}
		static void set(const std::string& key, const std::string& value) {
			application& ctx = instance();
			#define SET_CASE(type, name, default_val) if (key == #name) { from_string(value, ctx.name); return; }
				applicationFields(SET_CASE)
			#undef SET_CASE
			throw std::invalid_argument("Invalid key: " + key);
		}
	private:
		// Generic string conversion
		template<typename T>
		static std::string to_string(const T& val) {
			if constexpr (std::is_enum_v<T>) {
				return std::to_string(static_cast<int>(val));
			}
			else {
				return std::to_string(val);
			}
		}

		static std::string to_string(const std::string& val) {
			return val;
		}

		// Parsing from string
		template<typename T>
		static void from_string(const std::string& str, T& out) {
			std::istringstream ss(str);
			ss >> out;
		}

		static void from_string(const std::string& str, std::string& out) {
			out = str;
		}

		static void from_string(const std::string& str, config::gameMode& out) {
			out = static_cast<config::gameMode>(std::stoi(str));
		}

		static void from_string(const std::string& str, config::server& out) {
			out = static_cast<config::server>(std::stoi(str));
		}
	};

	struct user {
		std::string username = "";
		std::string avatar = "";

		std::vector<std::tuple<std::string, std::string>> toArray() const {
			return {
				{"username", username}
				,{ "avatar", avatar}
			};
		}
	};

	enum class dataType {
		t_string = 0
		,t_int = 1
		,t_decimal = 2
	};

	enum class formatType {
		f_string = 0
		,f_int = 1
		,f_decimal = 2
		,f_rank = 3
		,f_time = 4
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
				,{"display", ext::bool2str(display)}
				,{"banchoSupport", ext::bool2str(banchoSupport)}
				,{"titanicSupport", ext::bool2str(titanicSupport)}
			};
		}
	};

	static inline std::vector<dataEntry> vecData {
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

	enum fileHeader {
		ApplicationConfig
		,TrackerConfig
	};
	fileHeader resolve(std::string str) {
		if (str == "[ApplicationConfig]") return ApplicationConfig;
		if (str == "[TrackerConfig]") return TrackerConfig;
	}

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
		for (const config::dataEntry _vecData : config::vecData) {
			input += _vecData.key + ";" + ext::bool2str(_vecData.display) + "\n";
		}
		std::ofstream file;
		file.open("config.txt");
		file << input;
		file.close();
		writeLog("Config written");
	}

	void readConfig() {
		if (std::ifstream file{ "config.txt" }; file.is_open()) {
			writeLog("Reading config file...");
			int configHeader = -1;
			for (std::string line; std::getline(file, line); ){
				if (!line.starts_with("//") && !line.empty())
					continue;
				if (line.starts_with("[")) {
					switch (resolve(line)) {
						case ApplicationConfig:
							configHeader = 0;
							break;
						case TrackerConfig:
							configHeader = 1;
							break;
					}
					continue;
				}
				std::string key, value;
				std::tie(key,value) = ext::split2tuple(line, ';');
				switch (configHeader) {
					case 0:
						config::application::set(key, value);
						break;
					case 1:
						config::vecData::set(key, value);
						break;
				}
			}
			file.close();
		}
		else {
			writeLog("Error reading config file", true, 255, 0, 0);
		}
	}

	void rmConfig() {
		std::filesystem::remove("config.txt");
	}
};