#pragma once
#include <sstream>

std::vector<std::string> vec_log;

// ansi console color codes
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

// foreground
void setColorRGB_f(int red, int green, int blue) {
	std::cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
}

// background
void setColorRGB_b(int red, int green, int blue) {
	std::cout << "\033[48;2;" << red << ";" << green << ";" << blue << "m";
}

void resetColor() {
	std::cout << "\033[0m";
}

void writeLog(std::string msg, int r = 255, int g = 255, int b = 255) {
#ifdef DEBUG_BUILD
	time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* timeInfo = localtime(&currentTime);
	char dateBuffer[20];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", timeInfo);
	char timeBuffer[9];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", timeInfo);
	setColorRGB_f(100, 100, 100);
	std::cout << (std::string)dateBuffer + " " + timeBuffer << " ";
	resetColor();
	std::cout << "[";
	setColorRGB_f(100, 100, 100);
	std::cout << "Internal";
	resetColor();
	std::cout << "] ";
	setColorRGB_f(r, g, b);
	std::cout << msg << "\n";
	resetColor();
	std::stringstream ss;
	ss << (std::string)dateBuffer + " " + timeBuffer << " " << "[" << "Internal" << "] " << msg;
	vec_log.push_back(ss.str());
#endif
}