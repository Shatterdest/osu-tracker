#include <cmath>
#include <iomanip>
#include <thread>
#include "../header/config.h"

static std::string auth_token;
bool init_api_failed = false;

long double scoreNeeded(int n) {
	if (n <= 100) {
		return (5000.0L / 3.0L) * (4.0L * n * n * n - 3.0L * n * n - n)
			+ 1.25L * powl(1.8L, n - 60);
	}
	else {
		return 26931190827.0L + 99999999999.0L * (n - 100);
	}
}

long double getLevelFromScore(long double totalScore) {
	int low = 1, high = 1500;
	int level = 0;

	while (low <= high) {
		int mid = (low + high) / 2;
		if (scoreNeeded(mid) <= totalScore) {
			level = mid;
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}

	long double baseScore = scoreNeeded(level);
	long double nextScore = scoreNeeded(level + 1);
	long double fraction = (totalScore - baseScore) / (nextScore - baseScore);

	return std::round((level + fraction) * 100.0L) / 100.0L;
}


int api_auth() {
	try {
		nlohmann::json request;
		std::string body = R"({"client_id":)" + std::to_string(config::application::instance().clientId) + R"(, "client_secret":")" + config::application::instance().clientSecret + R"(", "grant_type":"client_credentials", "scope":"public"})";
		cpr::Response r = cpr::Post(cpr::Url{ "https://osu.ppy.sh/oauth/token" },
			cpr::Body{body},
			cpr::Header{ { "Content-Type", "application/json" } });
		request = nlohmann::json::parse(r.text);
		auth_token = "Bearer " + request.value("access_token", "");
		request.clear();
		writeLog((std::string)"api_auth() -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);
		return r.status_code;
	}
	catch (const nlohmann::json::exception& e) {
		// Catch JSON-specific exceptions
		writeLog(std::string("api_auth() -> JSON exception: ") + e.what(), true, 255, 0, 0);
		// segmentation fault -> linux fix
		return -1;
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		writeLog(std::string("api_auth() -> CPR exception: ") + e.message, true, 255, 0, 0);
		// segmentation fault -> linux fix
		return -1;
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("api_auth() -> Standard exception: ") + e.what(), true, 255, 0, 0);
		// segmentation fault -> linux fix
		return -1;
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("api_auth() -> Unknown exception occurred", true, 255, 0, 0);
		// segmentation fault -> linux fix
		return -1;
	}
	// segmentation fault -> linux fix
	return -1;
}

//
// PRIVATE SERVER IMPLEMENTATIONS
//
// #############
//	TITANIC
// #############
int titanic_api(bool init) {
	writeLog(std::string("titanic_api(init) -> ") + ext::bool2str(init), false, 0, 0, 255);
	try {
		int mode = 0;

		static const std::string _mode = std::to_string(static_cast<int>(config::application::instance().mode));

		cpr::Response r;
		cpr::Response r2;

		// Thread function for the first request
		auto fetch_users = [&r]() {
			r = cpr::Get(
				cpr::Url{ "https://api.titanic.sh/users/" + std::to_string(config::application::instance().osuId) },
				cpr::Header{
					{ "Content-Type", "application/json" },
					{ "Accept", "application/json" }
				}
			);
			writeLog("titanic_api(/users) -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);
			};

		// Thread function for the second request
		auto fetch_stats = [&r2]() {
			r2 = cpr::Get(
				cpr::Url{ "https://api.titanic.sh/stats" },
				cpr::Header{
					{ "Content-Type", "application/json" },
					{ "Accept", "application/json" }
				}
			);
			writeLog("titanic_api(/stats) -> Status Code: " + std::to_string(r2.status_code), false, 0, 255, 0);
			};

		// Launch threads
		std::thread t1(fetch_users);
		std::thread t2(fetch_stats);
		// Wait for both to finish
		t1.join();
		t2.join();

		nlohmann::json _j = nlohmann::json::parse(r.text);
		nlohmann::json _j2 = nlohmann::json::parse(r2.text);

		static const int count_graveyard = _j2["beatmap_modes"][_mode]["count_graveyard"].get<int>();
		static const int count_wip = _j2["beatmap_modes"][_mode]["count_wip"].get<int>();
		static const int count_pending = _j2["beatmap_modes"][_mode]["count_pending"].get<int>();
		static const int count_ranked = _j2["beatmap_modes"][_mode]["count_ranked"].get<int>();
		static const int count_approved = _j2["beatmap_modes"][_mode]["count_approved"].get<int>();
		static const int count_qualified = _j2["beatmap_modes"][_mode]["count_qualified"].get<int>();
		static const int count_loved = _j2["beatmap_modes"][_mode]["count_loved"].get<int>();

		for (size_t i = 0; i < 4; i++) {
			if (_mode == std::to_string(_j["stats"][i]["mode"].get<int>())) {
				mode = i; 			
				break;
			}
		}

		config::user::instance().username = _j["name"].get<std::string>();

		if (init) {
			config::data::arr[config::data::getIndex("level")].init = std::to_string(getLevelFromScore(_j["stats"][mode]["tscore"].get<long long>())); // level
			config::data::arr[config::data::getIndex("rankedScore")].init = std::to_string(_j["stats"][mode]["rscore"].get<long long>());
			config::data::arr[config::data::getIndex("totalScore")].init = std::to_string(_j["stats"][mode]["tscore"].get<long long>());
			config::data::arr[config::data::getIndex("ppRank")].init = std::to_string(_j["stats"][mode]["rank"].get<int>());
			config::data::arr[config::data::getIndex("pp")].init = std::to_string(_j["stats"][mode]["pp"].get<float>());
			config::data::arr[config::data::getIndex("ppv1")].init = std::to_string(_j["stats"][mode]["ppv1"].get<float>());
			config::data::arr[config::data::getIndex("acc")].init = std::to_string(_j["stats"][mode]["acc"].get<float>()*100);
			config::data::arr[config::data::getIndex("playtime")].init = std::to_string(_j["stats"][mode]["playtime"].get<long long>());
			config::data::arr[config::data::getIndex("playcount")].init = std::to_string(_j["stats"][mode]["playcount"].get<int>());
			config::data::arr[config::data::getIndex("totalHits")].init = std::to_string(_j["stats"][mode]["total_hits"].get<int>());
			config::data::arr[config::data::getIndex("silverSS")].init = std::to_string(_j["stats"][mode]["xh_count"].get<int>());
			config::data::arr[config::data::getIndex("goldSS")].init = std::to_string(_j["stats"][mode]["x_count"].get<int>());
			config::data::arr[config::data::getIndex("silverS")].init = std::to_string(_j["stats"][mode]["sh_count"].get<int>());
			config::data::arr[config::data::getIndex("goldS")].init = std::to_string(_j["stats"][mode]["s_count"].get<int>());
			config::data::arr[config::data::getIndex("a")].init = std::to_string(_j["stats"][mode]["a_count"].get<int>());
			config::data::arr[config::data::getIndex("totalSS")].init = std::to_string(std::stoi(config::data::arr[config::data::getIndex("silverSS")].init) + std::stoi(config::data::arr[config::data::getIndex("goldSS")].init)); // total ss
			config::data::arr[config::data::getIndex("totalS")].init = std::to_string(std::stoi(config::data::arr[config::data::getIndex("silverS")].init) + std::stoi(config::data::arr[config::data::getIndex("goldS")].init)); // total s
			config::data::arr[config::data::getIndex("clears")].init = std::to_string(std::stoi(config::data::arr[config::data::getIndex("totalSS")].init) + std::stoi(config::data::arr[config::data::getIndex("totalS")].init) + std::stoi(config::data::arr[config::data::getIndex("a")].init)); // profile clears

			// score rank, not supported by titanic (yet)
			//vec_data_respektive[0][1] = "";

			// b,c,d
			config::data::arr[config::data::getIndex("b")].init = std::to_string(_j["stats"][mode]["b_count"].get<int>());
			config::data::arr[config::data::getIndex("c")].init = std::to_string(_j["stats"][mode]["c_count"].get<int>());
			config::data::arr[config::data::getIndex("d")].init = std::to_string(_j["stats"][mode]["d_count"].get<int>());
			// calculate for titanic
			vec_data_inspector[3][1] = std::to_string(std::stoi(vec_data_titanic[17][1]) + std::stoi(vec_data_inspector[0][1]) + std::stoi(vec_data_inspector[1][1]) + std::stoi(vec_data_inspector[2][1]));
			// completion%
			vec_data_inspector[4][1] = std::to_string((static_cast<float>(std::stoi(vec_data_inspector[3][1])) / static_cast<float>(count_ranked + count_approved + count_loved + count_qualified))*100);

		}

		// current
		vec_data_titanic[0][2] = std::to_string(getLevelFromScore(_j["stats"][mode]["tscore"].get<long long>())); // level
		vec_data_titanic[1][2] = std::to_string(_j["stats"][mode]["rscore"].get<long long>());
		vec_data_titanic[2][2] = std::to_string(_j["stats"][mode]["tscore"].get<long long>());
		vec_data_titanic[3][2] = std::to_string(_j["stats"][mode]["rank"].get<int>());
		vec_data_titanic[4][2] = std::to_string(_j["stats"][mode]["pp"].get<float>());
		vec_data_titanic[5][2] = std::to_string(_j["stats"][mode]["ppv1"].get<float>());
		vec_data_titanic[6][2] = std::to_string(_j["stats"][mode]["acc"].get<float>() * 100);
		vec_data_titanic[7][2] = std::to_string(_j["stats"][mode]["playtime"].get<long long>());
		vec_data_titanic[8][2] = std::to_string(_j["stats"][mode]["playcount"].get<int>());
		vec_data_titanic[9][2] = std::to_string(_j["stats"][mode]["total_hits"].get<int>());
		vec_data_titanic[10][2] = std::to_string(_j["stats"][mode]["xh_count"].get<int>());
		vec_data_titanic[11][2] = std::to_string(_j["stats"][mode]["x_count"].get<int>());
		vec_data_titanic[12][2] = std::to_string(_j["stats"][mode]["sh_count"].get<int>());
		vec_data_titanic[13][2] = std::to_string(_j["stats"][mode]["s_count"].get<int>());
		vec_data_titanic[14][2] = std::to_string(_j["stats"][mode]["a_count"].get<int>());
		vec_data_titanic[15][2] = std::to_string(std::stoi(vec_data_titanic[10][2]) + std::stoi(vec_data_titanic[11][2])); // total ss
		vec_data_titanic[16][2] = std::to_string(std::stoi(vec_data_titanic[12][2]) + std::stoi(vec_data_titanic[13][2])); // total s
		vec_data_titanic[17][2] = std::to_string(std::stoi(vec_data_titanic[15][2]) + std::stoi(vec_data_titanic[16][2]) + std::stoi(vec_data_titanic[14][2])); // profile clears

		vec_data_inspector[0][2] = std::to_string(_j["stats"][mode]["b_count"].get<int>());
		vec_data_inspector[1][2] = std::to_string(_j["stats"][mode]["c_count"].get<int>());
		vec_data_inspector[2][2] = std::to_string(_j["stats"][mode]["d_count"].get<int>());
		// calc for titanic
		vec_data_inspector[3][2] = std::to_string(std::stoi(vec_data_titanic[17][2]) + std::stoi(vec_data_inspector[0][2]) + std::stoi(vec_data_inspector[1][2]) + std::stoi(vec_data_inspector[2][2]));
		vec_data_inspector[4][2] = std::to_string((static_cast<float>(std::stoi(vec_data_inspector[3][2])) / static_cast<float>(count_ranked + count_approved + count_loved + count_qualified)) * 100);
		// score rank, not supported by titanic (yet)
		vec_data_respektive[0][2] = "";

		// change
		vec_data_titanic[0][3] = std::to_string(std::stof(vec_data_titanic[0][2]) - std::stof(vec_data_titanic[0][1])); // lvl
		vec_data_titanic[1][3] = std::to_string(std::stoll(vec_data_titanic[1][2]) - std::stoll(vec_data_titanic[1][1])); // ranked score
		vec_data_titanic[2][3] = std::to_string(std::stoll(vec_data_titanic[2][2]) - std::stoll(vec_data_titanic[2][1])); // total score
		vec_data_titanic[3][3] = std::to_string(std::stoi(vec_data_titanic[3][2]) - std::stoi(vec_data_titanic[3][1])); // global rank
		vec_data_titanic[4][3] = std::to_string(std::stof(vec_data_titanic[4][2]) - std::stof(vec_data_titanic[4][1])); // pp
		vec_data_titanic[5][3] = std::to_string(std::stof(vec_data_titanic[5][2]) - std::stof(vec_data_titanic[5][1])); // ppv1
		vec_data_titanic[6][3] = std::to_string(std::stof(vec_data_titanic[6][2]) - std::stof(vec_data_titanic[6][1])); // acc
		vec_data_titanic[7][3] = std::to_string(std::stoll(vec_data_titanic[7][2]) - std::stoll(vec_data_titanic[7][1])); // play time
		vec_data_titanic[8][3] = std::to_string(std::stoi(vec_data_titanic[8][2]) - std::stoi(vec_data_titanic[8][1])); // play count
		vec_data_titanic[9][3] = std::to_string(std::stoi(vec_data_titanic[9][2]) - std::stoi(vec_data_titanic[9][1])); // total hits
		vec_data_titanic[10][3] = std::to_string(std::stoi(vec_data_titanic[10][2]) - std::stoi(vec_data_titanic[10][1])); // ssh
		vec_data_titanic[11][3] = std::to_string(std::stoi(vec_data_titanic[11][2]) - std::stoi(vec_data_titanic[11][1])); // ss
		vec_data_titanic[12][3] = std::to_string(std::stoi(vec_data_titanic[12][2]) - std::stoi(vec_data_titanic[12][1])); // sh
		vec_data_titanic[13][3] = std::to_string(std::stoi(vec_data_titanic[13][2]) - std::stoi(vec_data_titanic[13][1])); // s
		vec_data_titanic[14][3] = std::to_string(std::stoi(vec_data_titanic[14][2]) - std::stoi(vec_data_titanic[14][1])); // a
		vec_data_titanic[15][3] = std::to_string(std::stoi(vec_data_titanic[15][2]) - std::stoi(vec_data_titanic[15][1])); // total ss
		vec_data_titanic[16][3] = std::to_string(std::stoi(vec_data_titanic[16][2]) - std::stoi(vec_data_titanic[16][1])); // total s
		vec_data_titanic[17][3] = std::to_string(std::stoi(vec_data_titanic[17][2]) - std::stoi(vec_data_titanic[17][1])); // cleares (without bcd)

		vec_data_inspector[0][3] = std::to_string(std::stoi(vec_data_inspector[0][2]) - std::stoi(vec_data_inspector[0][1])); // b
		vec_data_inspector[1][3] = std::to_string(std::stoi(vec_data_inspector[1][2]) - std::stoi(vec_data_inspector[1][1])); // c
		vec_data_inspector[2][3] = std::to_string(std::stoi(vec_data_inspector[2][2]) - std::stoi(vec_data_inspector[2][1])); // d
		vec_data_inspector[3][3] = std::to_string(std::stoi(vec_data_inspector[3][2]) - std::stoi(vec_data_inspector[3][1])); // total clears, calculated
		vec_data_inspector[4][3] = std::to_string(std::stof(vec_data_inspector[4][2]) - std::stof(vec_data_inspector[4][1])); // completion%

		// score rank, not supported by titanic (yet)
		vec_data_respektive[0][3] = "";

		// extra, not supported by titanic
		vec_data_respektive_target[0][1] = "";
		vec_data_respektive_target[1][1] = "";
		vec_data_respektive_target[2][1] = "";

		// return
		return r.status_code;
	}
	catch (const nlohmann::json::exception& e) {
		// Catch JSON-specific exceptions
		writeLog(std::string("titanic_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		writeLog(std::string("titanic_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("titanic_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("titanic_api() -> Unknown exception occurred", true, 255, 0, 0);
	}
}

// ##############
//	bancho api
// ##############
static void osu_api(bool init) {
	try {
		std::string mode;
		switch (static_cast<int>(config::application::instance().mode)) {
			case 0:
				mode = "osu";
				break;
			case 1:
				mode = "taiko";
				break;
			case 2:
				mode = "fruits";
				break;
			case 3:
				mode = "mania";
				break;
		}
		cpr::Response r = cpr::Get(
			cpr::Url{ "https://osu.ppy.sh/api/v2/users/" + vec_application[1][1] + "/" + mode + "?key=id"},
			cpr::Header{
				{ "Content-Type", "application/json" },
				{ "Accept", "application/json" },
				{ "Authorization", auth_token }
			}
		);
		writeLog((std::string)"osu_api() -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);

		nlohmann::json _j = nlohmann::json::parse(r.text);
		username = _j["username"].get<std::string>();

		if (init) {
		
			vec_data_osu[0][1] = std::to_string(_j["statistics"]["level"]["current"].get<int>()) + "." + std::to_string(_j["statistics"]["level"]["progress"].get<int>());
			vec_data_osu[1][1] = std::to_string(_j["statistics"]["ranked_score"].get<long long>());
			vec_data_osu[2][1] = std::to_string(_j["statistics"]["total_score"].get<long long>());
			vec_data_osu[3][1] = std::to_string(_j["statistics"]["global_rank"].get<int>());
			vec_data_osu[4][1] = std::to_string(_j["statistics"]["pp"].get<float>());
			vec_data_osu[5][1] = std::to_string(_j["statistics"]["hit_accuracy"].get<float>());
			vec_data_osu[6][1] = std::to_string(_j["statistics"]["play_time"].get<long long>());
			vec_data_osu[7][1] = std::to_string(_j["statistics"]["play_count"].get<int>());

			// grade_counts
			vec_data_osu[8][1] = std::to_string(_j["statistics"]["grade_counts"]["ssh"].get<int>());
			vec_data_osu[9][1] = std::to_string(_j["statistics"]["grade_counts"]["ss"].get<int>());
			vec_data_osu[10][1] = std::to_string(_j["statistics"]["grade_counts"]["sh"].get<int>());
			vec_data_osu[11][1] = std::to_string(_j["statistics"]["grade_counts"]["s"].get<int>());
			vec_data_osu[12][1] = std::to_string(_j["statistics"]["grade_counts"]["a"].get<int>());

			// not available in api, get from ranks
			vec_data_osu[13][1] = std::to_string(std::stoi(vec_data_osu[8][1]) + std::stoi(vec_data_osu[9][1]));
			vec_data_osu[14][1] = std::to_string(std::stoi(vec_data_osu[10][1]) + std::stoi(vec_data_osu[11][1]));
		
			// profile clears
			vec_data_osu[15][1] = std::to_string(std::stoi(vec_data_osu[12][1]) + std::stoi(vec_data_osu[13][1]) + std::stoi(vec_data_osu[14][1]));
		}

		// current
		vec_data_osu[0][2] = std::to_string(_j["statistics"]["level"]["current"].get<int>()) + "." + std::to_string(_j["statistics"]["level"]["progress"].get<int>());
		vec_data_osu[1][2] = std::to_string(_j["statistics"]["ranked_score"].get<long long>());
		vec_data_osu[2][2] = std::to_string(_j["statistics"]["total_score"].get<long long>());
		vec_data_osu[3][2] = std::to_string(_j["statistics"]["global_rank"].get<int>());
		vec_data_osu[4][2] = std::to_string(_j["statistics"]["pp"].get<float>());
		vec_data_osu[5][2] = std::to_string(_j["statistics"]["hit_accuracy"].get<float>());
		vec_data_osu[6][2] = std::to_string(_j["statistics"]["play_time"].get<long long>());
		vec_data_osu[7][2] = std::to_string(_j["statistics"]["play_count"].get<int>());
		vec_data_osu[8][2] = std::to_string(_j["statistics"]["grade_counts"]["ssh"].get<int>());
		vec_data_osu[9][2] = std::to_string(_j["statistics"]["grade_counts"]["ss"].get<int>());
		vec_data_osu[10][2] = std::to_string(_j["statistics"]["grade_counts"]["sh"].get<int>());
		vec_data_osu[11][2] = std::to_string(_j["statistics"]["grade_counts"]["s"].get<int>());
		vec_data_osu[12][2] = std::to_string(_j["statistics"]["grade_counts"]["a"].get<int>());
		vec_data_osu[13][2] = std::to_string(std::stoi(vec_data_osu[8][2]) + std::stoi(vec_data_osu[9][2]));
		vec_data_osu[14][2] = std::to_string(std::stoi(vec_data_osu[10][2]) + std::stoi(vec_data_osu[11][2]));
		vec_data_osu[15][2] = std::to_string(std::stoi(vec_data_osu[12][2]) + std::stoi(vec_data_osu[13][2]) + std::stoi(vec_data_osu[14][2]));


		// change
		vec_data_osu[0][3] = std::to_string(std::stof(vec_data_osu[0][2]) - std::stof(vec_data_osu[0][1])); // lvl
		vec_data_osu[1][3] = std::to_string(std::stoll(vec_data_osu[1][2]) - std::stoll(vec_data_osu[1][1])); // ranked score
		vec_data_osu[2][3] = std::to_string(std::stoll(vec_data_osu[2][2]) - std::stoll(vec_data_osu[2][1])); // total score
		vec_data_osu[3][3] = std::to_string(std::stoi(vec_data_osu[3][2]) - std::stoi(vec_data_osu[3][1])); // global rank
		vec_data_osu[4][3] = std::to_string(std::stof(vec_data_osu[4][2]) - std::stof(vec_data_osu[4][1])); // pp
		vec_data_osu[5][3] = std::to_string(std::stof(vec_data_osu[5][2]) - std::stof(vec_data_osu[5][1])); // acc
		vec_data_osu[6][3] = std::to_string(std::stoll(vec_data_osu[6][2]) - std::stoll(vec_data_osu[6][1])); // play time
		vec_data_osu[7][3] = std::to_string(std::stoi(vec_data_osu[7][2]) - std::stoi(vec_data_osu[7][1])); // play count
		vec_data_osu[8][3] = std::to_string(std::stoi(vec_data_osu[8][2]) - std::stoi(vec_data_osu[8][1])); // ssh
		vec_data_osu[9][3] = std::to_string(std::stoi(vec_data_osu[9][2]) - std::stoi(vec_data_osu[9][1])); // ss
		vec_data_osu[10][3] = std::to_string(std::stoi(vec_data_osu[10][2]) - std::stoi(vec_data_osu[10][1])); // sh
		vec_data_osu[11][3] = std::to_string(std::stoi(vec_data_osu[11][2]) - std::stoi(vec_data_osu[11][1])); // s
		vec_data_osu[12][3] = std::to_string(std::stoi(vec_data_osu[12][2]) - std::stoi(vec_data_osu[12][1])); // a
		vec_data_osu[13][3] = std::to_string(std::stoi(vec_data_osu[13][2]) - std::stoi(vec_data_osu[13][1])); // total ss
		vec_data_osu[14][3] = std::to_string(std::stoi(vec_data_osu[14][2]) - std::stoi(vec_data_osu[14][1])); // total s
		vec_data_osu[15][3] = std::to_string(std::stoi(vec_data_osu[15][2]) - std::stoi(vec_data_osu[15][1])); // cleares (without bcd)
	}
	catch (const nlohmann::json::exception& e) {
		// Catch JSON-specific exceptions
		return;
		writeLog(std::string("osu_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		return;
		writeLog(std::string("osu_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("osu_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
		return;
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("osu_api() -> Unknown exception occurred", true, 255, 0, 0);
		return;
	}
	return;
}

static void respektive_api(bool init) {
	try {
		nlohmann::json _user;
		nlohmann::json _target;

		cpr::Response r_user = cpr::Get(
			cpr::Url{ "https://score.respektive.pw/u/" + vec_application[1][1] + "?m=" + vec_application[5][1] },
			cpr::Header{
				{ "Content-Type", "application/json" }
			}
		);
		writeLog((std::string)"respektive_api() -> Status Code: " + std::to_string(r_user.status_code), false, 0, 255, 0);

		_user = nlohmann::json::parse(r_user.text);

		if (init) {
			vec_data_respektive[0][1] = std::to_string(_user[0]["rank"].get<int>());
		}
		vec_data_respektive[0][2] = std::to_string(_user[0]["rank"].get<int>());
		vec_data_respektive[0][3] = std::to_string(std::stoi(vec_data_respektive[0][2]) - std::stoi(vec_data_respektive[0][1]));

		cpr::Response r_target = cpr::Get(
			cpr::Url{ "https://score.respektive.pw/rank/" + std::to_string(_user[0]["rank"].get<int>() - 1) + "?m=" + vec_application[5][1] },
			cpr::Header{
				{ "Content-Type", "application/json" }
			}
		);
		writeLog((std::string)"respektive_api() -> Status Code: " + std::to_string(r_target.status_code), false, 0, 255, 0);

		_target = nlohmann::json::parse(r_target.text);

		vec_data_respektive_target[0][1] = std::to_string(_target[0]["rank"].get<int>());
		vec_data_respektive_target[1][1] = _target[0]["username"].get<std::string>();
		vec_data_respektive_target[2][1] = std::to_string(_target[0]["score"].get<long long>() - _user[0]["score"].get<long long>());
	}
	catch (const nlohmann::json::exception& e) {
		// Catch JSON-specific exceptions
		writeLog(std::string("respektive_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		writeLog(std::string("respektive_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("respektive_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("respektive_api() -> Unknown exception occurred", true, 255, 0, 0);
	}
}

static void inspector_api(bool init) {
	try {
		cpr::Response r = cpr::Get(
			cpr::Url{ "https://api.kirino.sh/inspector/users/stats/" + vec_application[1][1] + "?skipDailyData=true&skipOsuData=true&skipExtras=true" },
			cpr::Header{
				{ "Content-Type", "application/json" }
			}
		);
		writeLog((std::string)"inspector_api() -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);

		nlohmann::json _j = nlohmann::json::parse(r.text);

		if (init) {
			vec_data_inspector[0][1] = std::to_string(_j["stats"]["b"].get<int>());
			vec_data_inspector[1][1] = std::to_string(_j["stats"]["c"].get<int>());
			vec_data_inspector[2][1] = std::to_string(_j["stats"]["d"].get<int>());
			vec_data_inspector[3][1] = _j["stats"]["clears"].get<std::string>(); // MIO >:(
			vec_data_inspector[4][1] = _j["stats"]["completion"].get<std::string>(); // MIO >:(
		}

		vec_data_inspector[0][2] = std::to_string(_j["stats"]["b"].get<int>());
		vec_data_inspector[1][2] = std::to_string(_j["stats"]["c"].get<int>());
		vec_data_inspector[2][2] = std::to_string(_j["stats"]["d"].get<int>());
		vec_data_inspector[3][2] = _j["stats"]["clears"].get<std::string>(); // MIO >:(
		vec_data_inspector[4][2] = _j["stats"]["completion"].get<std::string>(); // MIO >:(

		vec_data_inspector[0][3] = std::to_string(std::stoi(vec_data_inspector[0][2]) - std::stoi(vec_data_inspector[0][1]));
		vec_data_inspector[1][3] = std::to_string(std::stoi(vec_data_inspector[1][2]) - std::stoi(vec_data_inspector[1][1]));
		vec_data_inspector[2][3] = std::to_string(std::stoi(vec_data_inspector[2][2]) - std::stoi(vec_data_inspector[2][1]));
		vec_data_inspector[3][3] = std::to_string(std::stoi(vec_data_inspector[3][2]) - std::stoi(vec_data_inspector[3][1]));
		vec_data_inspector[4][3] = std::to_string(std::stof(vec_data_inspector[4][2]) - std::stof(vec_data_inspector[4][1]));
	}
	catch (const nlohmann::json::exception& e) {
		// Catch JSON-specific exceptions
		writeLog(std::string("inspector_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		writeLog(std::string("inspector_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("inspector_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("inspector_api() -> Unknown exception occurred", true, 255, 0, 0);
	}
}

void fetch_api_data(bool init) {
	switch (std::stoi(vec_application[6][1])) {
		case 0: {
			// bancho
			if (api_auth() != 200) {
				if (init) {
					init_api_failed = true;
				}
				return; // dont continue when auth fails
			}
			else {
				if (init_api_failed && !init) {
					// retry to init api data, since it failed to init
					init_api_failed = false;
					init = true;
				}
			}
			// Launch threads for each API function
			
			std::thread t_osu_api(osu_api, init);
			std::thread t_respektive_api(respektive_api, init);
			std::thread t_inspector_api(inspector_api, init);

			// Wait for all threads to complete
			t_osu_api.join();
			t_respektive_api.join();
			t_inspector_api.join();
			
			//osu_api(init);
			//respektive_api(init);
			//inspector_api(init);
			return;
			break;
		}
		case 1: {
			// titanic
			int result = titanic_api(init); // only one call to pull data
			if (result != 200) {
				if (init) {
					init_api_failed = true;
				}
				return;
			}

			if (init_api_failed && !init) {
				// The first init failed earlier, but now it worked — re-run with init = true
				init_api_failed = false;
				titanic_api(true); // intentionally re-pull with full init
			}
			return;
			break;
		}
	}
}

// data that get send to client
nlohmann::json api_data() {
	try {
		// Create a JSON object
		nlohmann::json _j;
		switch (std::stoi(vec_application[6][1])) {
			case 0: // bancho
				_j["osu"] = vec_data_osu;
				_j["respektive"] = vec_data_respektive;
				_j["respektive_target"] = vec_data_respektive_target;
				_j["inspector"] = vec_data_inspector;
				break;
			case 1: // titanic
				// same general json structure, bc my ass has to rewrite frontend js code
				_j["osu"] = vec_data_titanic;
				_j["respektive"] = vec_data_respektive;
				_j["respektive_target"] = vec_data_respektive_target;
				_j["inspector"] = vec_data_inspector;
				break;
		}
		return _j;
	}
	catch (const std::exception& e) {
		writeLog(std::string("api_data() -> Exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (const nlohmann::json::exception& e) {
		writeLog(std::string("api_data() -> JSON exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (...) {
		writeLog("api_data() -> Unknown exception occurred", true, 255, 0, 0);
	}
	return nlohmann::json(); // empty json
}
