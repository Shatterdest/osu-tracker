#pragma once

bool isNumeric(const std::string& str) {
	return std::all_of(str.begin(), str.end(), ::isdigit);
}

static std::string formatNumber(long long num) {
	std::string str = std::to_string(num);
	int n = str.length();
	for (int i = n - 3; i > 0; i -= 3) {
		str.insert(i, ".");
	}
	return str;
}

std::string boolToString(bool b) {
	return b ? "true" : "false";
}

std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != std::string::npos) {
		result.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start));

	return result;
}