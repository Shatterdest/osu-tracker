#pragma once
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

void vector_push(std::vector<std::string>* vec, std::string item, int maxSize) {
	if (vec->size() == maxSize) {
		vec->erase(vec->begin());
	}
	vec->push_back(item);
}

void vector_push(std::vector<int>* vec, int item, int maxSize) {
	if (vec->size() == maxSize) {
		vec->erase(vec->begin());
	}
	vec->push_back(item);
}

static void console_log(const int level, const std::string content, std::vector<std::string>* data, std::vector<int>* data_color, int max_log) {
	std::string error;
	// 0 : debug
	// 1 : info
	// 2 : warning
	// 3 : error
	// 4 : FATAL

	if (level == 0) { // DEBUG
		error = "DEBUG";
		//	color = "lightblue";
	}
	if (level == 1) { // INFO
		error = "INFO";
		//color = "white";
	}
	if (level == 2) { // WARN
		error = "WARN";
		//color = "yellow";
	}
	if (level == 3) { // ERROR
		error = "ERROR";
		//	color = "red";
	}
	if (level == 4) { // FATAL
		error = "FATAL";
		//color = "error";
	}
	// Get the current system time
	auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// Convert to a time structure
	std::tm timeInfo;
	localtime_s(&timeInfo, &currentTime);

	// Format the date and time as strings
	char dateBuffer[20];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);

	char timeBuffer[9];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);

	// Text to append with a new line
	std::string text = (std::string)dateBuffer + " " + timeBuffer + " [" + error + "] " + content;

	vector_push(data, text, max_log);
	vector_push(data_color, level, max_log);
}