#include <iostream>
#include <vector>
#include <string>
#include <json.hpp>
#include <cpr/cpr.h>
#include "api.h"

std::string version = "v1.0";

bool isNumeric(const std::string& str) {
	return std::all_of(str.begin(), str.end(), ::isdigit);
}
void moveCursorUp(int lines) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		std::cerr << "Error getting console handle" << std::endl;
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		std::cerr << "Error getting console screen buffer info" << std::endl;
		return;
	}

	COORD newPosition = csbi.dwCursorPosition;
	newPosition.Y -= lines;
	if (newPosition.Y < 0) {
		newPosition.Y = 0; // Ensure the cursor doesn't move above the top of the console
	}

	if (!SetConsoleCursorPosition(hConsole, newPosition)) {
		std::cerr << "Error setting console cursor position" << std::endl;
	}
}
void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

static std::string formatNumber(long long num) {
	std::string str = std::to_string(num);
	int n = str.length();
	for (int i = n - 3; i > 0; i -= 3) {
		str.insert(i, ".");
	}
	return str;
}

// ansi console color codes (probably windows only)
enum conCol {
	f_black = 30,
	f_red = 31,
	f_green = 32,
	f_yellow = 33,
	f_blue = 34,
	f_mangenta = 35,
	f_cyan = 36,
	f_white = 37,
	f_defaultColor = 39,
	b_black = 40,
	b_red = 41,
	b_green = 42,
	b_yellow = 43,
	b_blue = 44,
	b_mangenta = 45,
	b_cyan = 46,
	b_white = 47,
	b_defaultColor = 49
};

void setColor(conCol bg = conCol::b_defaultColor, conCol fg = conCol::f_defaultColor) {
	std::cout << "\033[" << bg << ";" << fg << "m";
}

void resetColor() {
	std::cout << "\033[0m";
}

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
	std::cout << " by _Railgun_\n";
	std::cout << "-----------------------\n";
}

std::vector<std::string> mainOpt{
	{"Run Tracker"},
	{"Settings"},
	{"Exit Application"}
};

int drawMenu(int index,std::vector<std::string> menu) {
	//initial draw
	for (int i = 0; i < menu.size(); i++) {
		if (i == index) {
			setColor(b_white, f_black);
			std::cout << menu[i];
			resetColor();
			std::cout << "\n";
		}
		else {
			resetColor();
			std::cout << menu[i] << "\n";
		}
	}
	while (true) {
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			// Move up in the array
			if (index > 0) {
				index--;
				moveCursorUp(menu.size());
				for (int i = 0; i < menu.size(); i++) {
					if (i == index) {
						setColor(b_white, f_black);
						std::cout << menu[i];
						resetColor();
						std::cout << "\n";
					}
					else {
						resetColor();
						std::cout << menu[i] << "\n";
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			// Move down in the array
			if (index < menu.size() - 1) {
				index++;
				moveCursorUp(menu.size());
				for (int i = 0; i < menu.size(); i++) {
					if (i == index) {
						setColor(b_white, f_black);
						std::cout << menu[i];
						resetColor();
						std::cout << "\n";
					}
					else {
						resetColor();
						std::cout << menu[i] << "\n";
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			return index;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}
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
	// Create a vector of Example structs

	getConfig(vec_tracker, "", "");
	enableVirtualTerminalProcessing();
	printHeader();

	if (!checkConfig()) {
		
		std::string osu_id, client_id, client_secret;
		std::cout << "It seems like, you launched the tracker for the first time.\n";
		std::cout << "Please enter your osu! user ID (example: osu.ppy.sh/users/";
		setColor(conCol::b_defaultColor, conCol::f_cyan);
		std::cout << "13817114";
		resetColor();
		std::cout << ")\n>";
		osuid:
		std::cin >> osu_id;
		if (!isNumeric(osu_id)) {
			std::cout << "Your osu! id should only contain numbers!\n>";
			goto osuid;
		}
		std::cout << "Please enter a Client ID to use the osu! API v2\n>";
		clientid:
		std::cin >> client_id;
		if (!isNumeric(client_id)) {
			std::cout << "Your Client ID should only contain numbers!\n>";
			goto clientid;
		}
		std::cout << "Please enter a Client Secret to use the osu! API v2\n>";
		std::cin >> client_secret;

		//setConfig("osu_id", osu_id);
		//setConfig("client_id", client_id);
		//setConfig("client_secret", client_secret);
		//setConfig("api_refreshInterval", "8");


		createConfig();
		readConfig();
	} else {
		readConfig();
	}
	clear();
	printHeader();
	setColor(b_defaultColor, f_green);
	std::cout << "[Enter]";
	resetColor();
	std::cout << " to select an option | ";
	setColor(b_defaultColor, f_green);
	std::cout << "[Arrow Up / Down]";
	resetColor();
	std::cout << " to change selection\n";
	std::cout << "-----------------------\n";
	int index = drawMenu(0, mainOpt);
	

	return 0;
}






