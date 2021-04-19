#include "OpCode.hpp"
#include "Runtime.hpp"
#include "Word.hpp"
#include "CountedString.hpp"
#include <string>
#include <iostream>

int digitValue(int base, char c) {
	if (c >= '0' && c <= '9') {
		if (c - '0' < base) {
			return c - '0';
		} else {
			return -1;
		}
	} else if (c >= 'a' && c <= 'z') {
		if (c - 'a' < base - 10) {
			return c - 'a' + 10;
		} else {
			return -1;
		}
	} else {
		return -1;
	}
}

int doWord(Runtime* runtime, char delim) {
	int tibAddr = runtime->getInt(TIBAddr);
	int tibLength = runtime->getInt(tibAddr);
	int tibInOffset = runtime->getInt(ToInAddr);
	char delimeter = runtime->popData();
	int pad = runtime->getInt(DictionaryPointerAddr) + 1;
	
	int len = 0;
	bool done = false;
	while (tibInOffset < tibLength && !done) {
		char c = runtime->getChar(tibAddr + tibInOffset);
		runtime->setChar(pad++, c);
		if (c == delimeter) {
			runtime->setChar(runtime->getInt(DictionaryPointerAddr), len);
			done = true;
		} else {
			len++;
		}
	}
	return runtime->getInt(DictionaryPointerAddr);
}


OpCode::OpCode(int codeIn)
: code(codeIn)
{
}

void OpCode::execute(Runtime* runtime) {
	switch (code) {
	case DoColon:
		runtime->pushReturn(runtime->getInstructionPointer());
		runtime->setInstructionPointer(runtime->getCurrentWordAddr() + (int)offsetof(struct Word, data));
		break;
	case DoSemicolon:
		runtime->setInstructionPointer(runtime->popReturn());
		break;
	case DoConstant:
		runtime->pushData(runtime->getInt(runtime->getCurrentWordAddr() + (int)offsetof(struct Word, data)));
		break;
	case DoVariable:
		runtime->pushData(runtime->getCurrentWordAddr() + (int)offsetof(struct Word, data));
		break;
	case Abort:
		runtime->reset(runtime->getInt(AbortVectorAddr));
		break;
	case Comma: {
		int dp = runtime->getInt(DictionaryPointerAddr);
		runtime->setInt(dp, runtime->popData());
		runtime->setInt(DictionaryPointerAddr, dp + sizeof(int));
		break; }
	case Lit:
		runtime->pushData(runtime->consumeNextInstruction());
		break;
	case Rot: {
		int temp = runtime->peekData(2);
		runtime->pokeData(2, runtime->peekData(1));
		runtime->pokeData(1, runtime->peekData(0));
		runtime->pokeData(0, temp);
		break; }
	case Drop:
		runtime->popData();
		break;
	case Dup:
		runtime->pushData(runtime->tos());
		break;
	case Swap: {
		int temp = runtime->tos();
		runtime->pokeData(0, runtime->peekData(1));
		runtime->pokeData(1, temp);
		break; }
	case Plus:
		runtime->pushData(runtime->popData() + runtime->popData());
		break;
	case Equals:
		runtime->pushData(runtime->popData() == runtime->popData());
		break;
	case At:
		runtime->pushData(runtime->getInt(runtime->popData()));
		break;
	case Put: {
		int addr = runtime->popData();
		runtime->setInt(addr, runtime->popData());
		break; }
	case ZeroBranch: {
		int destination = runtime->consumeNextInstruction();
		if (runtime->popData() == 0) {
			runtime->setInstructionPointer(destination);
		}
		break; }
	case Branch:
		runtime->setInstructionPointer(runtime->consumeNextInstruction());
		break;
	case Execute:
		runtime->pushReturn(runtime->getInstructionPointer());
		runtime->setInstructionPointer(runtime->popData());
		break;
	case Exit:
		runtime->setInstructionPointer(runtime->popReturn());
		break;
	case Count: {	//( addr -- addr2 len )
		int addr = runtime->popData();
		runtime->pushData(addr + 1);
		runtime->pushData(runtime->getChar(addr));
		break; }
	case ToNumber: {	//( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
		//>number - ( double addr len -- double2 addr2-zero    ) if successful, or
		//			( double addr len -- int     addr2-nonzero ) on error.
		int len = runtime->popData();
		int addr = runtime->popData();
		int dval = runtime->popData();
		int base = runtime->getInt(NumberBaseAddress);
		bool done = false;
		while (len > 0 && !done) {
			int val = digitValue(base, runtime->getChar(addr));
			if (val >= 0) {
				dval = dval * base + val;
				len = len - 1;
				addr = addr + 1;
			} else {
				done = true;
			}
		}
		runtime->pushData(dval);
		runtime->pushData(addr);
		runtime->pushData(len);
		break; }
	case Accept: {	//( addr len -- len2 )
		std::string str;
		std::getline(std::cin, str);
		int maxLen = runtime->popData();
		int addr = runtime->popData();
		int ndx = 0;
		while (maxLen > 0 && ndx < str.length()) {
			runtime->setChar(addr, str[ndx]);
			maxLen--;
			addr++;
			ndx++;
		}
		runtime->pushData(ndx);
		break; }
	case Word: { //( char -- addr )
		int tibAddr = runtime->getInt(TIBAddr);
		int tibLength = runtime->getChar(tibAddr);
		int tibInOffset = runtime->getInt(ToInAddr);
		char delimeter = runtime->popData();
		int pad = runtime->getInt(DictionaryPointerAddr) + 1;
		
		int len = 0;
		bool done = false;
		while (tibInOffset < tibLength && !done) {
			char c = runtime->getChar(tibAddr + tibInOffset);
			runtime->setChar(pad++, c);
			if (c == delimeter) {
				runtime->setChar(runtime->getInt(DictionaryPointerAddr), len);
				done = true;
			} else {
				len++;
			}
		}
		runtime->pushData(runtime->getInt(DictionaryPointerAddr));
		break; }
	case Emit:	// ( char -- )
		std::cout << (char)runtime->popData();
		std::cout.flush();
		break;
	case Find: {
		// addr -- addr2 flag  look up word in the dictionary
		// find looks in the Forth dictionary for a word with the name given in the
		// counted string at addr. One of the following will be returned:
		// flag =  0, addr2 = counted string --> word was not found
		// flag =  1, addr2 = call address   --> word is immediate
		// flag = -1, addr2 = call address   --> word is not immediate
		int stringAddr = runtime->popData();
		int thisWord = runtime->getInt(DictionaryPointerAddr);
		bool done = false;
		while (thisWord != 0 && !done) {
			int thisWordNameAddr = thisWord + (int)offsetof(struct Word, name);
			if (compareCountedString(runtime, stringAddr, thisWordNameAddr) != 0) {
				thisWord = runtime->getInt(thisWord + (int)offsetof(struct Word, previous));
			} else {
				done = true;
			}
		}
		if (thisWord != 0) {
			runtime->pushData(thisWord);
			runtime->pushData(runtime->getInt(thisWord + (int)offsetof(struct Word, flags)) & Word::IMMEDIATE_MASK);
		} else {
			runtime->pushData(stringAddr);
			runtime->pushData(0);
		}
		break; }
	case Colon:	{	// compiling word
		// set state to compile
		runtime->setInt(StateAddr, -1);
	
		// set previous link
		int dp = runtime->getInt(DictionaryPointerAddr);
		int lastWord = runtime->getInt(LastWord);
		
		class Word* w = (class Word*)runtime->getHeadAddr(dp);
		w->previous = lastWord;
		runtime->setInt(LastWord, dp);
		w->flags = 0;
		w->opcode = Colon;
		CountedString name(w->name);
		
		
		
		
		break; }
	case SemiColon:
	case Create:
	case SemiCode:
	case Constant:
	case Interpret:
	default: {
		break; }
	}
}


