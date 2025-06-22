#include <cmath>
#include <iomanip>
#include <thread>
#include "../header/config.h"

class api {
private: 
	bool init_api_failed = false;

	// Private Servers
	class pServer {
	private:
		// fetch titanic api data
		int titanic(bool init) {
			console::writeLog(std::string("titanic_api(init) -> ") + ext::bool2str(init), false, 0, 0, 255);
			try {

				cpr::Response r_titanicUsers;
				cpr::Response r_titanicStats;

				// Thread function for the first request
				auto fetchUsers = [&r_titanicUsers]() {
					r_titanicUsers = cpr::Get(
						cpr::Url{ "https://api.titanic.sh/users/" + std::to_string(config::application::instance().osuId) },
						cpr::Header{
							{ "Content-Type", "application/json" },
							{ "Accept", "application/json" }
						}
					);
					console::writeLog("titanic_api(/users) -> Status Code: " + std::to_string(r_titanicUsers.status_code), false, 0, 255, 0);
					};

				// Thread function for the second request
				auto fetchStats = [&r_titanicStats]() {
					r_titanicStats = cpr::Get(
						cpr::Url{ "https://api.titanic.sh/stats" },
						cpr::Header{
							{ "Content-Type", "application/json" },
							{ "Accept", "application/json" }
						}
					);
					console::writeLog("titanic_api(/stats) -> Status Code: " + std::to_string(r_titanicStats.status_code), false, 0, 255, 0);
					};

				std::thread t1(fetchUsers);
				std::thread t2(fetchStats);
				t1.join();
				t2.join();
				nlohmann::json _j = nlohmann::json::parse(r_titanicUsers.text);
				nlohmann::json _j2 = nlohmann::json::parse(r_titanicStats.text);
				static const std::string _mode = std::to_string(static_cast<int>(config::application::instance().mode));
				//static const int count_graveyard = _j2["beatmap_modes"][_mode]["count_graveyard"].get<int>();
				//static const int count_wip = _j2["beatmap_modes"][_mode]["count_wip"].get<int>();
				//static const int count_pending = _j2["beatmap_modes"][_mode]["count_pending"].get<int>();
				static const int count_ranked = _j2["beatmap_modes"][_mode]["count_ranked"].get<int>();
				static const int count_approved = _j2["beatmap_modes"][_mode]["count_approved"].get<int>();
				static const int count_qualified = _j2["beatmap_modes"][_mode]["count_qualified"].get<int>();
				static const int count_loved = _j2["beatmap_modes"][_mode]["count_loved"].get<int>();

				char mode = 0;
				for (size_t i = 0; i < 4; i++) {
					if (_mode == std::to_string(_j["stats"][i]["mode"].get<int>())) {
						mode = i;
						break;
					}
				}

				config::user::instance().username = _j["name"].get<std::string>();

				if (init) {
					config::data::arr[config::data::getIndex("level")].init = std::to_string(getLevelFromScore(_j["stats"][mode]["tscore"].get<long long>()));
					config::data::arr[config::data::getIndex("rankedScore")].init = std::to_string(_j["stats"][mode]["rscore"].get<long long>());
					config::data::arr[config::data::getIndex("totalScore")].init = std::to_string(_j["stats"][mode]["tscore"].get<long long>());
					config::data::arr[config::data::getIndex("ppRank")].init = std::to_string(_j["stats"][mode]["rank"].get<int>());
					config::data::arr[config::data::getIndex("pp")].init = std::to_string(_j["stats"][mode]["pp"].get<float>());
					config::data::arr[config::data::getIndex("ppv1")].init = std::to_string(_j["stats"][mode]["ppv1"].get<float>());
					config::data::arr[config::data::getIndex("acc")].init = std::to_string(_j["stats"][mode]["acc"].get<float>() * 100);
					config::data::arr[config::data::getIndex("playtime")].init = std::to_string(_j["stats"][mode]["playtime"].get<long long>());
					config::data::arr[config::data::getIndex("playcount")].init = std::to_string(_j["stats"][mode]["playcount"].get<int>());
					config::data::arr[config::data::getIndex("totalHits")].init = std::to_string(_j["stats"][mode]["total_hits"].get<int>());
					config::data::arr[config::data::getIndex("silverSS")].init = std::to_string(_j["stats"][mode]["xh_count"].get<int>());
					config::data::arr[config::data::getIndex("goldSS")].init = std::to_string(_j["stats"][mode]["x_count"].get<int>());
					config::data::arr[config::data::getIndex("silverS")].init = std::to_string(_j["stats"][mode]["sh_count"].get<int>());
					config::data::arr[config::data::getIndex("goldS")].init = std::to_string(_j["stats"][mode]["s_count"].get<int>());
					config::data::arr[config::data::getIndex("a")].init = std::to_string(_j["stats"][mode]["a_count"].get<int>());
					config::data::arr[config::data::getIndex("totalSS")].init = std::to_string(
						std::stoi(config::data::arr[config::data::getIndex("silverSS")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("goldSS")].init)
					);
					config::data::arr[config::data::getIndex("totalS")].init = std::to_string(
						std::stoi(config::data::arr[config::data::getIndex("silverS")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("goldS")].init)
					);
					config::data::arr[config::data::getIndex("clears")].init = std::to_string(
						std::stoi(config::data::arr[config::data::getIndex("totalSS")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("totalS")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("a")].init)
					);
					config::data::arr[config::data::getIndex("b")].init = std::to_string(_j["stats"][mode]["b_count"].get<int>());
					config::data::arr[config::data::getIndex("c")].init = std::to_string(_j["stats"][mode]["c_count"].get<int>());
					config::data::arr[config::data::getIndex("d")].init = std::to_string(_j["stats"][mode]["d_count"].get<int>());
					config::data::arr[config::data::getIndex("totalClears")].init = std::to_string(
						std::stoi(config::data::arr[config::data::getIndex("clears")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("b")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("c")].init)
						+ std::stoi(config::data::arr[config::data::getIndex("d")].init)
					);
					config::data::arr[config::data::getIndex("completion")].init = std::to_string(
						(static_cast<float>(std::stoi(config::data::arr[config::data::getIndex("totalClears")].init)) / static_cast<float>(count_ranked + count_approved + count_loved + count_qualified)) * 100
					);
				}

				config::data::arr[config::data::getIndex("level")].current	= std::to_string(getLevelFromScore(_j["stats"][mode]["tscore"].get<long long>()));
				config::data::arr[config::data::getIndex("rankedScore")].current = std::to_string(_j["stats"][mode]["rscore"].get<long long>());
				config::data::arr[config::data::getIndex("totalScore")].current = std::to_string(_j["stats"][mode]["tscore"].get<long long>());
				config::data::arr[config::data::getIndex("ppRank")].current = std::to_string(_j["stats"][mode]["rank"].get<int>());
				config::data::arr[config::data::getIndex("pp")].current = std::to_string(_j["stats"][mode]["pp"].get<float>());
				config::data::arr[config::data::getIndex("ppv1")].current = std::to_string(_j["stats"][mode]["ppv1"].get<float>());
				config::data::arr[config::data::getIndex("acc")].current = std::to_string(_j["stats"][mode]["acc"].get<float>() * 100);
				config::data::arr[config::data::getIndex("playtime")].current = std::to_string(_j["stats"][mode]["playtime"].get<long long>());
				config::data::arr[config::data::getIndex("playcount")].current = std::to_string(_j["stats"][mode]["playcount"].get<int>());
				config::data::arr[config::data::getIndex("totalHits")].current = std::to_string(_j["stats"][mode]["total_hits"].get<int>());
				config::data::arr[config::data::getIndex("silverSS")].current = std::to_string(_j["stats"][mode]["xh_count"].get<int>());
				config::data::arr[config::data::getIndex("goldSS")].current = std::to_string(_j["stats"][mode]["x_count"].get<int>());
				config::data::arr[config::data::getIndex("silverS")].current = std::to_string(_j["stats"][mode]["sh_count"].get<int>());
				config::data::arr[config::data::getIndex("goldS")].current = std::to_string(_j["stats"][mode]["s_count"].get<int>());
				config::data::arr[config::data::getIndex("a")].current = std::to_string(_j["stats"][mode]["a_count"].get<int>());
				config::data::arr[config::data::getIndex("totalSS")].current = std::to_string(
					std::stoi(config::data::arr[config::data::getIndex("silverSS")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("goldSS")].current)
				);
				config::data::arr[config::data::getIndex("totalS")].current = std::to_string(
					std::stoi(config::data::arr[config::data::getIndex("silverS")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("goldS")].current)
				);
				config::data::arr[config::data::getIndex("clears")].init = std::to_string(
					std::stoi(config::data::arr[config::data::getIndex("totalSS")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("totalS")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("a")].current)
				);
				config::data::arr[config::data::getIndex("b")].current = std::to_string(_j["stats"][mode]["b_count"].get<int>());
				config::data::arr[config::data::getIndex("c")].current = std::to_string(_j["stats"][mode]["c_count"].get<int>());
				config::data::arr[config::data::getIndex("d")].current = std::to_string(_j["stats"][mode]["d_count"].get<int>());
				config::data::arr[config::data::getIndex("totalClears")].current = std::to_string(
					std::stoi(config::data::arr[config::data::getIndex("clears")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("b")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("c")].current)
					+ std::stoi(config::data::arr[config::data::getIndex("d")].current)
				);
				config::data::arr[config::data::getIndex("completion")].init = std::to_string(
					(static_cast<float>(std::stoi(config::data::arr[config::data::getIndex("totalClears")].init)) / static_cast<float>(count_ranked + count_approved + count_loved + count_qualified)) * 100
				);

				calcDifference();
				return r_titanicUsers.status_code;
			}
			catch (const nlohmann::json::exception& e) {
				// Catch JSON-specific exceptions
				console::writeLog(std::string("titanic_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
			}
			catch (const cpr::Error& e) {
				// Catch cpr-specific exceptions
				console::writeLog(std::string("titanic_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
			}
			catch (const std::exception& e) {
				// Catch other standard exceptions
				console::writeLog(std::string("titanic_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
			}
			catch (...) {
				// Catch any other unknown exceptions
				console::writeLog("titanic_api() -> Unknown exception occurred", true, 255, 0, 0);
			}
		}	
	};

	class osu {
	private:
		static std::string auth_token;
		// auth osu api
		int api_auth() {
			try {
				nlohmann::json request;
				std::string body = R"({"client_id":)" + std::to_string(config::application::instance().clientId) + R"(, "client_secret":")" + config::application::instance().clientSecret + R"(", "grant_type":"client_credentials", "scope":"public"})";
				cpr::Response r = cpr::Post(cpr::Url{ "https://osu.ppy.sh/oauth/token" },
					cpr::Body{ body },
					cpr::Header{ { "Content-Type", "application/json" } });
				request = nlohmann::json::parse(r.text);
				auth_token = "Bearer " + request.value("access_token", "");
				request.clear();
				console::writeLog((std::string)"api_auth() -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);
				return r.status_code;
			}
			catch (const nlohmann::json::exception& e) {
				// Catch JSON-specific exceptions
				console::writeLog(std::string("api_auth() -> JSON exception: ") + e.what(), true, 255, 0, 0);
				// segmentation fault -> linux fix
				return -1;
			}
			catch (const cpr::Error& e) {
				// Catch cpr-specific exceptions
				console::writeLog(std::string("api_auth() -> CPR exception: ") + e.message, true, 255, 0, 0);
				// segmentation fault -> linux fix
				return -1;
			}
			catch (const std::exception& e) {
				// Catch other standard exceptions
				console::writeLog(std::string("api_auth() -> Standard exception: ") + e.what(), true, 255, 0, 0);
				// segmentation fault -> linux fix
				return -1;
			}
			catch (...) {
				// Catch any other unknown exceptions
				console::writeLog("api_auth() -> Unknown exception occurred", true, 255, 0, 0);
				// segmentation fault -> linux fix
				return -1;
			}
			// segmentation fault -> linux fix
			return -1;
		}
		// fetch osu api data
		static int api(bool init) {
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
					cpr::Url{ "https://osu.ppy.sh/api/v2/users/" + std::to_string(config::application::instance().osuId) + "/" + mode + "?key=id"},
					cpr::Header{
						{ "Content-Type", "application/json" },
						{ "Accept", "application/json" },
						{ "Authorization", auth_token }
					}
				);
				console::writeLog((std::string)"api() -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);

				nlohmann::json _j = nlohmann::json::parse(r.text);
				config::user::instance().username = _j["username"].get<std::string>();

				if (init) {
					config::data::arr[config::data::getIndex("level")].init = std::to_string(_j["statistics"]["level"]["current"].get<int>()) + "." + std::to_string(_j["statistics"]["level"]["progress"].get<int>());
					config::data::arr[config::data::getIndex("rankedScore")].init = std::to_string(_j["statistics"]["ranked_score"].get<long long>());
					config::data::arr[config::data::getIndex("totalScore")].init = std::to_string(_j["statistics"]["total_score"].get<long long>());
					config::data::arr[config::data::getIndex("ppRank")].init = std::to_string(_j["statistics"]["global_rank"].get<int>());
					config::data::arr[config::data::getIndex("pp")].init = std::to_string(_j["statistics"]["pp"].get<float>());
					config::data::arr[config::data::getIndex("acc")].init = std::to_string(_j["statistics"]["hit_accuracy"].get<float>());
					config::data::arr[config::data::getIndex("playtime")].init = std::to_string(_j["statistics"]["play_time"].get<long long>());
					config::data::arr[config::data::getIndex("playcount")].init = std::to_string(_j["statistics"]["play_count"].get<int>());
					config::data::arr[config::data::getIndex("silverSS")].init = std::to_string(_j["statistics"]["grade_counts"]["ssh"].get<int>());
					config::data::arr[config::data::getIndex("goldSS")].init = std::to_string(_j["statistics"]["grade_counts"]["ss"].get<int>());
					config::data::arr[config::data::getIndex("silverS")].init = std::to_string(_j["statistics"]["grade_counts"]["sh"].get<int>());
					config::data::arr[config::data::getIndex("goldS")].init = std::to_string(_j["statistics"]["grade_counts"]["s"].get<int>());
					config::data::arr[config::data::getIndex("a")].init = std::to_string(_j["statistics"]["grade_counts"]["a"].get<int>());
					config::data::arr[config::data::getIndex("totalSS")].init = std::to_string(std::stoi(config::data::arr[config::data::getIndex("silverSS")].init) + std::stoi(config::data::arr[config::data::getIndex("goldSS")].init));
					config::data::arr[config::data::getIndex("totalS")].init = std::to_string(std::stoi(config::data::arr[config::data::getIndex("silverS")].init) + std::stoi(config::data::arr[config::data::getIndex("goldS")].init));
					config::data::arr[config::data::getIndex("clears")].init = std::to_string(std::stoi(config::data::arr[config::data::getIndex("totalSS")].init) + std::stoi(config::data::arr[config::data::getIndex("totalS")].init) + std::stoi(config::data::arr[config::data::getIndex("a")].init));
				}

				config::data::arr[config::data::getIndex("level")].current = std::to_string(_j["statistics"]["level"]["current"].get<int>()) + "." + std::to_string(_j["statistics"]["level"]["progress"].get<int>());
				config::data::arr[config::data::getIndex("rankedScore")].current = std::to_string(_j["statistics"]["ranked_score"].get<long long>());
				config::data::arr[config::data::getIndex("totalScore")].current = std::to_string(_j["statistics"]["total_score"].get<long long>());
				config::data::arr[config::data::getIndex("ppRank")].current = std::to_string(_j["statistics"]["global_rank"].get<int>());
				config::data::arr[config::data::getIndex("pp")].current = std::to_string(_j["statistics"]["pp"].get<float>());
				config::data::arr[config::data::getIndex("acc")].current = std::to_string(_j["statistics"]["hit_accuracy"].get<float>());
				config::data::arr[config::data::getIndex("playtime")].current = std::to_string(_j["statistics"]["play_time"].get<long long>());
				config::data::arr[config::data::getIndex("playcount")].current = std::to_string(_j["statistics"]["play_count"].get<int>());
				config::data::arr[config::data::getIndex("silverSS")].current = std::to_string(_j["statistics"]["grade_counts"]["ssh"].get<int>());
				config::data::arr[config::data::getIndex("goldSS")].current = std::to_string(_j["statistics"]["grade_counts"]["ss"].get<int>());
				config::data::arr[config::data::getIndex("silverS")].current = std::to_string(_j["statistics"]["grade_counts"]["sh"].get<int>());
				config::data::arr[config::data::getIndex("goldS")].current = std::to_string(_j["statistics"]["grade_counts"]["s"].get<int>());
				config::data::arr[config::data::getIndex("a")].current = std::to_string(_j["statistics"]["grade_counts"]["a"].get<int>());
				config::data::arr[config::data::getIndex("totalSS")].current = std::to_string(std::stoi(config::data::arr[config::data::getIndex("silverSS")].current) + std::stoi(config::data::arr[config::data::getIndex("goldSS")].current));
				config::data::arr[config::data::getIndex("totalS")].current = std::to_string(std::stoi(config::data::arr[config::data::getIndex("silverS")].current) + std::stoi(config::data::arr[config::data::getIndex("goldS")].current));
				config::data::arr[config::data::getIndex("clears")].current = std::to_string(std::stoi(config::data::arr[config::data::getIndex("totalSS")].current) + std::stoi(config::data::arr[config::data::getIndex("totalS")].current) + std::stoi(config::data::arr[config::data::getIndex("a")].current));

				calcDifference();
				return r.status_code;
			}
			catch (const nlohmann::json::exception& e) {
				// Catch JSON-specific exceptions
				return -1;
				console::writeLog(std::string("osu_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
			}
			catch (const cpr::Error& e) {
				// Catch cpr-specific exceptions
				return -1;
				console::writeLog(std::string("osu_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
			}
			catch (const std::exception& e) {
				// Catch other standard exceptions
				console::writeLog(std::string("osu_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
				return -1;
			}
			catch (...) {
				// Catch any other unknown exceptions
				console::writeLog("osu_api() -> Unknown exception occurred", true, 255, 0, 0);
				return -1;
			}
			return -1;
		}
		class extended {
		private:
			static void respektive_api(bool init) {
				try {
					nlohmann::json _user;
					cpr::Response r_user = cpr::Get(
						cpr::Url{ "https://score.respektive.pw/u/" + std::to_string(config::application::instance().osuId) + "?m=" + std::to_string(static_cast<int>(config::application::instance().mode)) },
						cpr::Header{
							{ "Content-Type", "application/json" }
						}
					);
					console::writeLog((std::string)"respektive_api() -> Status Code: " + std::to_string(r_user.status_code), false, 0, 255, 0);
					_user = nlohmann::json::parse(r_user.text);
					
					if (init) {
						config::data::arr[config::data::getIndex("scoreRank")].init = std::to_string(_user[0]["rank"].get<int>());
					}
					config::data::arr[config::data::getIndex("scoreRank")].current = std::to_string(_user[0]["rank"].get<int>());
					calcDifference();

					nlohmann::json _target;
					cpr::Response r_target = cpr::Get(
						cpr::Url{ "https://score.respektive.pw/rank/" + std::to_string(_user[0]["rank"].get<int>() - 1) + "?m=" + std::to_string(static_cast<int>(config::application::instance().mode)) },
						cpr::Header{
							{ "Content-Type", "application/json" }
						}
					);
					console::writeLog((std::string)"respektive_api() -> Status Code: " + std::to_string(r_target.status_code), false, 0, 255, 0);

					_target = nlohmann::json::parse(r_target.text);

					config::data::arr[config::data::getIndex("targetRank")].current = std::to_string(_target[0]["rank"].get<int>());
					config::data::arr[config::data::getIndex("targetUser")].current = _target[0]["username"].get<std::string>();
					config::data::arr[config::data::getIndex("targetScore")].current = std::to_string(_target[0]["score"].get<long long>() - _user[0]["score"].get<long long>());
					return;
				}
				catch (const nlohmann::json::exception& e) {
					// Catch JSON-specific exceptions
					console::writeLog(std::string("respektive_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
					return;
				}
				catch (const cpr::Error& e) {
					// Catch cpr-specific exceptions
					console::writeLog(std::string("respektive_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
					return;
				}
				catch (const std::exception& e) {
					// Catch other standard exceptions
					console::writeLog(std::string("respektive_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
					return;
				}
				catch (...) {
					// Catch any other unknown exceptions
					console::writeLog("respektive_api() -> Unknown exception occurred", true, 255, 0, 0);
					return;
				}
				return;
			}
			
			static void inspector_api(bool init) {
				try {
					cpr::Response r = cpr::Get(
						cpr::Url{ "https://api.kirino.sh/inspector/users/stats/" + vec_application[1][1] + "?skipDailyData=true&skipOsuData=true&skipExtras=true" },
						cpr::Header{
							{ "Content-Type", "application/json" }
						}
					);
					console::writeLog((std::string)"inspector_api() -> Status Code: " + std::to_string(r.status_code), false, 0, 255, 0);

					nlohmann::json _j = nlohmann::json::parse(r.text);

					if (init) {
						config::data::arr[config::data::getIndex("b")].init = std::to_string(_j["stats"]["b"].get<int>());
						config::data::arr[config::data::getIndex("c")].init = std::to_string(_j["stats"]["c"].get<int>());
						config::data::arr[config::data::getIndex("d")].init = std::to_string(_j["stats"]["d"].get<int>());
						config::data::arr[config::data::getIndex("totalClears")].init = _j["stats"]["clears"].get<std::string>(); // MIO >:(
						config::data::arr[config::data::getIndex("completion")].init = _j["stats"]["completion"].get<std::string>(); // MIO >:(
					}

					config::data::arr[config::data::getIndex("b")].current = std::to_string(_j["stats"]["b"].get<int>());
					config::data::arr[config::data::getIndex("c")].current = std::to_string(_j["stats"]["c"].get<int>());
					config::data::arr[config::data::getIndex("d")].current = std::to_string(_j["stats"]["d"].get<int>());
					config::data::arr[config::data::getIndex("totalClears")].current = _j["stats"]["clears"].get<std::string>(); // MIO >:(
					config::data::arr[config::data::getIndex("completion")].current = _j["stats"]["completion"].get<std::string>(); // MIO >:(
					calcDifference();
				}
				catch (const nlohmann::json::exception& e) {
					// Catch JSON-specific exceptions
					console::writeLog(std::string("inspector_api() -> JSON exception: ") + e.what(), true, 255, 0, 0);
				}
				catch (const cpr::Error& e) {
					// Catch cpr-specific exceptions
					console::writeLog(std::string("inspector_api() -> CPR exception: ") + e.message, true, 255, 0, 0);
				}
				catch (const std::exception& e) {
					// Catch other standard exceptions
					console::writeLog(std::string("inspector_api() -> Standard exception: ") + e.what(), true, 255, 0, 0);
				}
				catch (...) {
					// Catch any other unknown exceptions
					console::writeLog("inspector_api() -> Unknown exception occurred", true, 255, 0, 0);
				}
			}
		};
	};

	static void calcDifference() {
		for (config::dataEntry data : config::data::arr) {
			if (data.init.length() < 1 || data.current.length() < 1)
				continue;
			switch (data.dataType) {
				case config::dataType::_int:
					config::data::arr[config::data::getIndex(data.key.c_str())].change = std::to_string(std::stoi(data.current) - std::stoi(data.init));
					break;
				case config::dataType::_float:
					config::data::arr[config::data::getIndex(data.key.c_str())].change = std::to_string(std::stof(data.current) - std::stof(data.init));
					break;
				case config::dataType::_longLong:
					config::data::arr[config::data::getIndex(data.key.c_str())].change = std::to_string(std::stoll(data.current) - std::stoll(data.init));
					break;
			}
		}
	}

	static long double scoreNeeded(int n) {
		if (n <= 100) {
			return (5000.0L / 3.0L) * (4.0L * n * n * n - 3.0L * n * n - n)
				+ 1.25L * powl(1.8L, n - 60);
		}
		else {
			return 26931190827.0L + 99999999999.0L * (n - 100);
		}
	}

	static long double getLevelFromScore(long double totalScore) {
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
	
public:
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
			console::writeLog(std::string("api_data() -> Exception: ") + e.what(), true, 255, 0, 0);
		}
		catch (const nlohmann::json::exception& e) {
			console::writeLog(std::string("api_data() -> JSON exception: ") + e.what(), true, 255, 0, 0);
		}
		catch (...) {
			console::writeLog("api_data() -> Unknown exception occurred", true, 255, 0, 0);
		}
		return nlohmann::json(); // empty json
	}
};