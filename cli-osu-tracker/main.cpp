#include <iostream>
#include <vector>
#include <string>
#include <json.hpp>
#include <cpr/cpr.h>
#include "../header/ext.h"
#include "../header/api.h"
#include "../header/console.h"
#include "../header/config.h"
#include "../header/webserver.h"

void enableVirtualTerminalProcessing() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

void printHeader() {
	setColor(conCol::b_defaultColor, conCol::f_cyan);
	std::cout << OSU_TRACKER_NAME << " ";
	setColor(conCol::b_defaultColor, conCol::f_white);
	std::cout << OSU_TRACKER_VERSION;
#if OSU_TRACKER_DEBUG_BUILD == true
	setColor(conCol::b_defaultColor, conCol::f_red);
	std::cout << " DEBUG";
#endif
	resetColor();
	std::cout << " by " << OSU_TRACKER_CREATOR << "\n---------------------- - \n";
}

// TODO: MOVE AWAY + REFACTOR
static void respektive_api() {
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

int b = 0, c = 0, d = 0;
int new_b = 0, new_c = 0, new_d = 0;
static void inspector_api() {
	try {
		std::ofstream myfile;
		nlohmann::json inspectorJson;

		auto selfInspector = cpr::Get(cpr::Url{ "https://api.kirino.sh/inspector/users/stats/(osuidhere)?skipDailyData=true&skipOsuData=true&skipExtras=true" },
			cpr::Header{ { "Content-Type", "application/json" } });

		inspectorJson = nlohmann::json::parse(selfInspector.text);

		new_b = inspectorJson["stats"]["b"];
		new_c = inspectorJson["stats"]["c"];
		new_d = inspectorJson["stats"]["d"];
		float completion = inspectorJson["stats"]["completion"];
		std::string completion_p = std::to_string(completion);
		std::stringstream ssa;
		ssa << "+" << (new_b - b);
		myfile.open("b_gained.txt");
		myfile << ssa.str();
		myfile.close();

		std::stringstream ssb;
		ssb << "+" << (new_c - c);
		myfile.open("c_gained.txt");
		myfile << ssb.str();
		myfile.close();

		std::stringstream ssc;
		ssc << "+" << (new_d - d);
		myfile.open("d_gained.txt");
		myfile << ssc.str();
		myfile.close();

		std::stringstream ssd;
		ssd << (new_b);
		myfile.open("b.txt");
		myfile << ssd.str();
		myfile.close();

		std::stringstream sse;
		sse << (new_c);
		myfile.open("c.txt");
		myfile << sse.str();
		myfile.close();

		std::stringstream ssf;
		ssf << (new_d);
		myfile.open("d.txt");
		myfile << ssf.str();
		myfile.close();

		std::stringstream ssg;
		ssg << "Completion: ";
		for (int ii = 0; ii < 6; ii++) {
			ssg << completion_p[ii];
		}
		ssg << "%";
		myfile.open("completion.txt");
		myfile << ssg.str();
		myfile.close();
	}
	catch (...) {
		std::cout << "Error inspector_api()\n";
	}
}

void counterThread() {
	while (true) {
		update_mustache();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main()
{
	// WEB SOCKET TEST
	//std::thread t1(counterThread);

	enableVirtualTerminalProcessing();
	printHeader();

	if (!checkConfig()) {
		writeLog("Config file not found");
		setConfig(vec_application, "osu_id", "value", "Your osu! 'user id'");
		setConfig(vec_application, "client_id", "value", "Your API v2 'Client ID'");
		setConfig(vec_application, "client_secret", "value", "Your API v2 'Client Secret'");
		setConfig(vec_application, "api_refreshInterval", "value", "8");
		writeConfig();
		readConfig();
	}
	else {
		writeLog("Config file found");
		readConfig();
	}

#if OSU_TRACKER_ENABLE_WEBSERVER == 1
	while (WEB_SERVER) {
		writeLog("Starting Web Server...");
		writeLog("Web Server should be accessible under:");
		std::cout << "-> http://" << OSU_TRACKER_WEBSERVER_IP << ":" << OSU_TRACKER_WEBSERVER_PORT << "\n";
		webserver_start(); // blocking
		writeLog("Web Server Terminated...");
	}
#endif
	return 0;
}