#ifndef Terminal_hpp
#define Terminal_hpp

#include <string>

class Terminal {
public:
	Terminal();

	void emit(char c);
	char read();
	
private:
	std::string in;
	int nextCharPosition;
};

#endif /* Terminal_hpp */
