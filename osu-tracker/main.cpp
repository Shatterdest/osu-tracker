// suppress warnings bc they spam output
#pragma warning( disable : 4305)
#pragma warning( disable : 4566)

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

#ifdef _WIN32
void enableVirtualTerminalProcessing() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}
#endif

void printHeader() {
	setColor(conCol::b_defaultColor, conCol::f_cyan);
	std::cout << OSU_TRACKER_NAME << " ";
	setColor(conCol::b_defaultColor, conCol::f_white);
	std::cout << OSU_TRACKER_VERSION << " " << OSU_TRACKER_RELEASE_TYPE;
#ifdef DEBUG_BUILD
	setColor(conCol::b_defaultColor, conCol::f_red);
	std::cout << " DEBUG";
#endif
	resetColor();
	std::cout << " by " << OSU_TRACKER_CREATOR << "\n-------------------------\n";
}

int main()
{
	#ifdef _WIN32 
		enableVirtualTerminalProcessing(); //poopoo cmd.exe 
	#endif
	printHeader();

	bool run = true;
	bool skipInit = false;
	while (run) {
		if (!std::filesystem::exists("config.txt")) {
			writeLog("Config file not found");

			setConfig(vec_application, "osu_id", "value", "");
			setConfig(vec_application, "client_id", "value", "");
			setConfig(vec_application, "client_secret", "value", "");
			setConfig(vec_application, "api_refreshInterval", "value", "7000");
			writeConfig();
			readConfig();
		}
		else {
			writeLog("Config file found");
			readConfig();
			writeConfig();
		}
		#if OSU_TRACKER_ENABLE_WEBSERVER == 1
			api_init();
			run = !webserver_start(skipInit); // blocking
			skipInit = true;
		#endif
		}
	return 0;
}