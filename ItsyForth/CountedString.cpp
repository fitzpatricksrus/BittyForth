#include "CountedString.hpp"

int CountedString::compare(char* str1, char* str2) {
	int len = *str1;
	int lenOther = *str2;
	int end = (len < lenOther) ? len : lenOther;
	for (int i = 1; i <= end; i++) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}
	return len - lenOther;
}

void CountedString::fromCString(const std::string& source, char* dest, int maxLen) {
	maxLen = (maxLen - 1 <= source.length()) ? maxLen - 1 : source.length();
	const char* sourceChars = source.c_str();
	for (int i = 0; i < maxLen; i++) {
		dest[i+1] = sourceChars[i];
	}
	dest[0] = maxLen;
}

std::string CountedString::toCString(char* str) {
	int len = *str;
	str++;
	
	std::string result;
	for (int i = 0; i < len; i++) {
		result += *str;
		str++;
	}
	return result;
}
