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