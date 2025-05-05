#include "../header/config.h"

static std::string auth_token;
bool init_api_failed = false;

int api_auth() {
	try {
		nlohmann::json request;
		std::string body = R"({"client_id":)" + vec_application[2][1] + R"(, "client_secret":")" + vec_application[3][1] + R"(", "grant_type":"client_credentials", "scope":"public"})";
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
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		writeLog(std::string("api_auth() -> CPR exception: ") + e.message, true, 255, 0, 0);
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("api_auth() -> Standard exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("api_auth() -> Unknown exception occurred", true, 255, 0, 0);
	}
}

void osu_api(bool init) {
	try {
		std::string mode;
		switch (std::stoi(vec_application[5][1])) {
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
				{ "Content-Type", "apaplication/json" },
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
		vec_data_osu[1][3] = std::to_string(std::stoll(vec_data_osu[1][2]) - std::stoll(vec_data_osu[1][1]));
		vec_data_osu[2][3] = std::to_string(std::stoll(vec_data_osu[2][2]) - std::stoll(vec_data_osu[2][1]));
		vec_data_osu[3][3] = std::to_string(std::stoi(vec_data_osu[3][2]) - std::stoi(vec_data_osu[3][1]));
		vec_data_osu[4][3] = std::to_string(std::stof(vec_data_osu[4][2]) - std::stof(vec_data_osu[4][1])); // pp
		vec_data_osu[5][3] = std::to_string(std::stof(vec_data_osu[5][2]) - std::stof(vec_data_osu[5][1])); // acc
		vec_data_osu[6][3] = std::to_string(std::stoll(vec_data_osu[6][2]) - std::stoll(vec_data_osu[6][1]));
		vec_data_osu[7][3] = std::to_string(std::stoi(vec_data_osu[7][2]) - std::stoi(vec_data_osu[7][1]));
		vec_data_osu[8][3] = std::to_string(std::stoi(vec_data_osu[8][2]) - std::stoi(vec_data_osu[8][1]));
		vec_data_osu[9][3] = std::to_string(std::stoi(vec_data_osu[9][2]) - std::stoi(vec_data_osu[9][1]));
		vec_data_osu[10][3] = std::to_string(std::stoi(vec_data_osu[10][2]) - std::stoi(vec_data_osu[10][1]));
		vec_data_osu[11][3] = std::to_string(std::stoi(vec_data_osu[11][2]) - std::stoi(vec_data_osu[11][1]));
		vec_data_osu[12][3] = std::to_string(std::stoi(vec_data_osu[12][2]) - std::stoi(vec_data_osu[12][1]));
		vec_data_osu[13][3] = std::to_string(std::stoi(vec_data_osu[13][2]) - std::stoi(vec_data_osu[13][1]));
		vec_data_osu[14][3] = std::to_string(std::stoi(vec_data_osu[14][2]) - std::stoi(vec_data_osu[14][1]));
		vec_data_osu[15][3] = std::to_string(std::stoi(vec_data_osu[15][2]) - std::stoi(vec_data_osu[15][1]));
	}
	catch (const nlohmann::json::exception& e) {
		// Catch JSON-specific exceptions
		writeLog(std::string("osu_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (const cpr::Error& e) {
		// Catch cpr-specific exceptions
		writeLog(std::string("osu_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
	}
	catch (const std::exception& e) {
		// Catch other standard exceptions
		writeLog(std::string("osu_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
	}
	catch (...) {
		// Catch any other unknown exceptions
		writeLog("osu_api() -> Unknown exception occurred", true, 255, 0, 0);
	}
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

		std::string url = "https://score.respektive.pw/rank/" + std::to_string(_user[0]["rank"].get<int>() - 1) + "?m=" + vec_application[5][1];

		cpr::Response r_target = cpr::Get(
			cpr::Url{ url },
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
	osu_api(init);
	respektive_api(init);
	inspector_api(init);
}

// data that get send to client
nlohmann::json api_data() {
	try {
		// Create a JSON object
		nlohmann::json _j;
		_j["osu"] = vec_data_osu;
		_j["respektive"] = vec_data_respektive;
		_j["respektive_target"] = vec_data_respektive_target;
		_j["inspector"] = vec_data_inspector;
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
