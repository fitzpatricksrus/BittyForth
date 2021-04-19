#ifndef Word_hpp
#define Word_hpp

class Word {
public:
	static const int IMMEDIATE_MASK = 0x0001;
	
	int previous;
	int flags;
	int opcode;
	char name[32];
	char data[];
};

#endif /* Word_hpp */
