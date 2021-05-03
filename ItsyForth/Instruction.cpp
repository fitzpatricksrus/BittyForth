#include "Instruction.hpp"
#include "Runtime.hpp"

void Instruction::execute(Runtime* runtime, int currentInstructionPtr, int nextInstructionPtr) {
	Cell& currentCell = runtime->getCell(currentInstructionPtr);
	Instruction& currentInstruction = currentCell.asInstruction;
	switch (currentCell.asInstruction.opcode) {
	case DoColon: {
		runtime->pushReturn(nextInstructionPtr);
		runtime->setNextInstructionPointer(currentInstruction.data);
		break; }
	case DoSemicolon:
		runtime->setNextInstructionPointer(runtime->popReturn());
		break;
	case DoLit:
		runtime->pushData(currentInstruction.data);
		break;
	case DoConstant:
		//push the next cell on data stack and drop the return stack pointer
		runtime->pushData(runtime->getCell(nextInstructionPtr).asData);
		runtime->popReturn();
		break;
	case DoVariable:
		//push address of next cell on data stack and drop the return stack pointer
		runtime->pushData(nextInstructionPtr);
		runtime->popReturn();
		break;
/*	case Comma: {
		Builder(runtime).compileReference(runtime->popData());
		break; } */
	case Rot: {
		long temp = runtime->peekData(2);
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
		long temp = runtime->tos();
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
		runtime->pushData(runtime->getCell(runtime->popData()).asData);
		break;
	case Put: {
		int addr = runtime->popData();
		runtime->setCell(addr, runtime->popData());
		break; }
	case ZeroBranch: {
		if (runtime->popData() == 0) {
			runtime->setNextInstructionPointer(currentInstruction.data);
		}
		break; }
	case Branch:
		runtime->setNextInstructionPointer(currentInstruction.data);
		break;
	case Execute:
		runtime->pushReturn(nextInstructionPtr);
		runtime->setNextInstructionPointer(runtime->popData());
		break;
	case Exit:
		// should never get here.
		break;
	case Count: {	// ( addr -- addr2 len )
					// dup 1 + swap c@
		int addr = runtime->popData();
		runtime->pushData(addr + 1);
		runtime->pushData(runtime->getByte(addr));
		break; }
/*	case ToNumber: {	//( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
		//>number - ( double addr len -- double2 addr2-zero    ) if successful, or
		//			( double addr len -- long     addr2-nonzero ) on error.
		int len = runtime->popData();
		char* addr = runtime->popData();
		Num dval = runtime->popData();
		int base = runtime->getNumberBase();
		bool done = false;
		while (len > 0 && !done) {
			int val = digitValue(base, *addr);
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
		break; } */
/*	case Accept: {	//( addr len -- len2 )
		int maxLen = runtime->popData();
		char* addr = (char*)runtime->popData();
		long ndx;
		do {
			char c = runtime->read();
			for (ndx = 0; ndx < maxLen && c != '\n'; ndx++) {
				addr[ndx] = c;
				c = runtime->read();
			}
		} while (ndx == 0);	//skip empty lines
//		if (ndx >= maxLen) {
			// hey jf - this overflowed the input buffer
			// do you want to print a message on the
			// terminal and flush the remaining input?
//		}
		runtime->pushData(ndx);
		break; } */
/*	case Word: { // ( char -- addr )
		char* pad = (char*)runtime->getDictionaryPtr();
		char delimeter = runtime->popData();
		std::string nameIn = Builder(runtime).getNextInputWord(delimeter);
		CountedString::fromCString(nameIn, pad, padLength);
		runtime->pushData(pad);
		break; } */
	case Emit:	// ( char -- )
		runtime->emit((char)runtime->popData());
		break;
/*	case Find: {
		// addr -- addr2 flag  look up word in the dictionary
		// find looks in the Forth dictionary for a word with the name given in the
		// counted string at addr. One of the following will be returned:
		// flag =  0, addr2 = counted string --> word was not found
		// flag =  1, addr2 = call address   --> word is immediate
		// flag = -1, addr2 = call address   --> word is not immediate
		char* stringAddr = runtime->popData();
		DictionaryWord* thisWordAddr = runtime->getLastWordPtr();
		bool done = false;
		while (thisWordAddr != 0 && !done) {
			char* thisWordNameAddr = thisWordAddr->name;
			
			if (CountedString::compare(stringAddr, thisWordNameAddr) != 0) {
				thisWordAddr = thisWordAddr->previous;
			} else {
				done = true;
			}
		}
		if (thisWordAddr != 0) {
			runtime->pushData(thisWordAddr);
			runtime->pushData(thisWordAddr->flags & DictionaryWord::IMMEDIATE_MASK);
		} else {
			runtime->pushData(stringAddr);
			runtime->pushData(0L);
		}
		break; } */
/*	case Create: {
		// -- create a new word --
		Builder(runtime).createWord(OpCode::Colon);
		break; } */
	default: {
		break; }
	}
}

static const std::string OPCODE_NAMES[] = {
		"INVALID_OPCODE"
		"(colon)",
		"(semicolon)",
		"(lit)",
		"(constant)",
		"(variable)",
		"abort",
		",",
		"rot",
		"drop",
		"dup",
		"swap",
		"+",
		"=",
		"@",
		"!",
		"(0branch)",
		"(branh)",
		"execute",
		"exit",
		"count",
		">number",
		"accept",
		"word",
		"emit",
		"find",
		"create",
		"constant",
	};

std::string Instruction::toString() const {
	return OPCODE_NAMES[opcode] + "(" + std::to_string(data) + ")";
}

