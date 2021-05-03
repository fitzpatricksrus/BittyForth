#ifndef TermialIO_hpp
#define TermialIO_hpp

#include <string>
class Runtime;

class TerminalIO {
public:
	TerminalIO(Runtime* runtime);
	std::string getNextInputWord(char delim);
	void emit(char c);
	
private:
	Runtime* runtime;
};

#endif /* TermialIO_hpp */
