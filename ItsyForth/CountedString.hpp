#ifndef CountedString_hpp
#define CountedString_hpp

#include <string>
#include "Types.hpp"

class CountedString {
public:
	static int compare(char* str1, char* str2);
	static void fromCString(const std::string& source, char* dest, int maxLen);
	static std::string toCString(char* str);
};

#endif /* CountedString_hpp */
