#ifndef CountedString_hpp
#define CountedString_hpp

#include <string>

class CountedString {
public:
	static int compare(char* str1, char* str2);
	static void fromCString(const std::string& source, char* dest, int maxLen);
};


#endif /* CountedString_hpp */
