#include "../header/config.h"

static std::string auth_token;

int api_auth() {
	nlohmann::json request;
	std::string body = R"({"client_id":)" + vec_application[2][1] + R"(, "client_secret":")" + vec_application[3][1] + R"(", "grant_type":"client_credentials", "scope":"public"})";
	auto r = cpr::Post(cpr::Url{ "https://osu.ppy.sh/oauth/token" },
		cpr::Body{body},
		cpr::Header{ { "Content-Type", "application/json" } });
	request = nlohmann::json::parse(r.text);
	auth_token = "Bearer " + request.value("access_token", "");
	request.clear();

	return r.status_code;
}

void osu_api(bool init) {
	nlohmann::json request;
	auto r = cpr::Get(cpr::Url{ "https://osu.ppy.sh/api/v2/users/" + vec_application[1][1] + "/" + vec_application[5][1] + "?key=id"},
		cpr::Header{
			{ "Content-Type", "apaplication/json" },
			{ "Accept", "application/json" },
			{ "Authorization", auth_token }
		});
}

static void respektive_api(bool init) {
	try {
		std::ofstream myfile;
		nlohmann::json selfJson;
		nlohmann::json nextJson;

		auto selfRank = cpr::Get(cpr::Url{ "https://score.respektive.pw/u/osuidhere" },
			cpr::Header{ { "Content-Type", "application/json" } });
		selfJson = nlohmann::json::parse(selfRank.text);
		long long self_score = selfJson[0]["score"];
		long self_rank = selfJson[0]["rank"];
		std::string url = "https://score.respektive.pw/rank/" + std::to_string(self_rank - 1);
		auto nextRank = cpr::Get(cpr::Url{ url },
			cpr::Header{ { "Content-Type", "application/json" } });
		nextJson = nlohmann::json::parse(nextRank.text);
		long long next_score = nextJson[0]["score"];
		long next_rank = nextJson[0]["rank"];
		std::string next_username = nextJson[0]["username"];
		std::stringstream ss;
		ss << "Score to #" << next_rank << " : " << formatNumber(next_score - self_score) << "\nPlayer: " << next_username;
		myfile.open("output.txt");
		myfile << ss.str();
		myfile.close();
	}
	catch (...) {
		std::cout << "Error respektive_api()\n";
	}
}

static void inspector_api(bool init) {
	try {
		nlohmann::json _j;

		cpr::Response r = cpr::Get(
			cpr::Url{ "https://api.kirino.sh/inspector/users/stats/" + vec_application[1][1] + "?skipDailyData=true&skipOsuData=true&skipExtras=true"},
			cpr::Header{ { "Content-Type", "application/json" } }
		);

		_j = nlohmann::json::parse(r.text);

		if (init) {
			vec_data[14][1] = _j["stats"]["b"];
			vec_data[15][1] = _j["stats"]["b"];
			vec_data[16][1] = _j["stats"]["b"];
			vec_data[20][1] = _j["stats"]["clears"];
			vec_data[21][1] = _j["stats"]["completion"];

		}

		vec_data[14][2] = _j["stats"]["b"];
		vec_data[15][2] = _j["stats"]["b"];
		vec_data[16][2] = _j["stats"]["b"];
		vec_data[20][2] = _j["stats"]["clears"];
		vec_data[21][2] = _j["stats"]["completion"];

	}
	catch (...) {
	}
}

void calcDifference() {
	for (size_t i = 0; i < vec_data.size(); i++) {

	}
}

void api_init() {
	try {
		api_auth();
		osu_api(true);
		respektive_api(true);
		inspector_api(true);

	}
	catch (...) {
	}
}

void getStats() {

}
