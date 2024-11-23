#include "config.h"

std::string auth_token;

static void Auth(std::string client_id, std::string client_secret) {
	nlohmann::json request;
	std::string body = R"({"client_id":)" + client_id + R"(, "client_secret":")" + client_secret + R"(", "grant_type":"client_credentials", "scope":"public"})";
	auto r = cpr::Post(cpr::Url{ "https://osu.ppy.sh/oauth/token" },
		cpr::Body{
		body
		},
		cpr::Header{ { "Content-Type", "application/json" } });
	request = nlohmann::json::parse(r.text);
	auth_token = "Bearer " + request.value("access_token", "");
	request.clear();
}

static void getUser(std::string user_id, std::string mode) {
	nlohmann::json request;
	auto r = cpr::Get(cpr::Url{ "https://osu.ppy.sh/api/v2/users/" + user_id + "/" + mode + "?key=id" },
		cpr::Header{
			{ "Content-Type", "application/json" },
			{ "Accept", "application/json" },
			{ "Authorization", auth_token }
		});
}
