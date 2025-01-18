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

std::string version = "v1.0";

void enableVirtualTerminalProcessing() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

void printHeader() {
	setColor(conCol::b_defaultColor, conCol::f_cyan);
	std::cout << "CLI osu! Tracker ";
	setColor(conCol::b_defaultColor, conCol::f_white);
	std::cout << version;
	resetColor();
	std::cout << " by _Railgun_\n-----------------------\n";
}

std::vector<std::string> option_main{
	{"Run Tracker"},
	{"Settings"},
	{"API Status"},
	{"Debug"},
	{"Exit Application"}
};

std::vector<std::string> option_settings{
	{"Global Config"},
	{"Tracker Config"},
	{"API Config"},
	{"Set/Edit Custom Config"},
	{"Back"}
};


static void respektive_api() {
	try {
		std::ofstream myfile;
		nlohmann::json selfJson;
		nlohmann::json nextJson;

		auto selfRank = cpr::Get(cpr::Url{ "https://score.respektive.pw/u/osuidhere"},
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

int main()
{
	enableVirtualTerminalProcessing();
	printHeader();
	auto app = webserver_start();
	if (!checkConfig()) {
		
		std::string osu_id, client_id, client_secret;
		std::cout << "It seems like, you launched the tracker for the first time.\n";
		std::cout << "Please enter your osu! user ID (example: osu.ppy.sh/users/";
		setColor(conCol::b_defaultColor, conCol::f_cyan);
		std::cout << "13817114";
		resetColor();
		std::cout << ")\n>";

		getInput("osu! ID", osu_id);

		std::cout << "Please enter a Client ID to use the osu! API v2\n>";
		getInput("Client ID", client_id);

		std::cout << "Please enter a Client Secret to use the osu! API v2\n>";
		getInput("Client Secret");

		setConfig(vec_application, "osu_id", "value", osu_id);
		setConfig(vec_application, "client_id", "value", client_id);
		setConfig(vec_application, "client_secret", "value", client_secret);
		setConfig(vec_application, "api_refreshInterval", "value", "8");
		writeConfig();
		readConfig();
	} else {
		readConfig();
	}
	con_clear();
	printHeader();
	resetColor();
	std::cout << getConfig(vec_application, "osu_id", "value") << "\n";
	std::cout << getConfig(vec_application, "client_id", "value") << "\n";
	std::cout << getConfig(vec_application, "client_secret", "value") << "\n";

	int index = drawMenu(true,option_main);
	

	return 0;
}