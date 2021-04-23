#include "TermialIO.hpp"

#include "Runtime.hpp"

TerminalIO::TerminalIO(Runtime* runtimeIn) {
	runtime = runtimeIn;
}
	
std::string TerminalIO::getNextInputWord(char delimeter) {
	std::string result;
	char* tibAddr = runtime->tibAddr;
	char* inputAddr = tibAddr + runtime->tibInputOffset;
	char* inputEndAddr = tibAddr + runtime->tibContentLength;
	
	//skip leading delimeters
	while ((inputAddr < inputEndAddr) && (*inputAddr == delimeter)) {
		inputAddr++;
	}
	
	while ((inputAddr < inputEndAddr) && (*inputAddr != delimeter)) {
		result += *inputAddr;
		inputAddr++;
	}
	
	runtime->tibInputOffset += result.length();
	
	return result;
}

void TerminalIO::emit(char c) {
}
