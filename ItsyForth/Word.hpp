//
//  Word.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef Word_hpp
#define Word_hpp

#include <string>

class OpCode;

struct Word {
	void set(std::string str, OpCode* op, bool immediate = false);
	int* getDataAsInt(int addr);
	
	int& operator[](int addr);
	
	int previous;
	bool immediate;
	int opcode;
	char name[32];
	char data[];
};

#endif /* Word_hpp */
