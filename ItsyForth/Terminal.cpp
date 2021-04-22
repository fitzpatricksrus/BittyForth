#include "Terminal.hpp"

#include <iostream>

Terminal::Terminal() {
	in = "";
	nextCharPosition = 0;
}

void Terminal::emit(char c) {
	std::cout << c;
	std::cout.flush();
}

char Terminal::read() {
	if (nextCharPosition >= in.length()) {
		std::cout << "OK" << std::endl;
		std::getline(std::cin, in);
		nextCharPosition = 0;
		return '\n';
	} else {
		return in.at(nextCharPosition++);
	}
}
