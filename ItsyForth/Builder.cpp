//
//  Builder.cpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#include "Builder.hpp"

#include "Runtime.hpp"
#include "OpCode.hpp"
#include "DictionaryWord.hpp"
#include "CountedString.hpp"

void Builder::rebuildDictionary(Runtime* runtime) {
	// recreate the system dictionary entries
	
	// system words
	Builder::create(runtime, "(doColon)", OpCode::DoColon);
	DictionaryWord* semicolonWord = Builder::create(runtime, "(doSemicolon)", OpCode::DoSemicolon);
	Builder::create(runtime, "(doConstant)", OpCode::DoConstant);
	Builder::create(runtime, "(doVariable)", OpCode::DoVariable);
	create(runtime, "abort", OpCode::Abort);
	create(runtime, ",", OpCode::Comma);
	create(runtime, "(lit)", OpCode::Lit);
	create(runtime, "rot", OpCode::Rot);
	create(runtime, "drop", OpCode::Drop);
	create(runtime, "dup", OpCode::Dup);
	create(runtime, "swap", OpCode::Swap);
	create(runtime, "+", OpCode::Plus);
	create(runtime, "=", OpCode::Equals);
	DictionaryWord* atWord = create(runtime, "@", OpCode::At);
	create(runtime, "!", OpCode::Put);
	create(runtime, "(0branch)", OpCode::ZeroBranch);
	create(runtime, "(branch)", OpCode::Branch);
	create(runtime, "execute", OpCode::Execute);
	create(runtime, "exit", OpCode::Exit);
	create(runtime, "count", OpCode::Count);
	create(runtime, ">number", OpCode::ToNumber);
	create(runtime, "accept", OpCode::Accept);
	create(runtime, "word", OpCode::Word);
	create(runtime, "emit", OpCode::Emit);
	create(runtime, "find", OpCode::Find);
	create(runtime, ":", OpCode::Colon);
	create(runtime, ";", OpCode::SemiColon);
	create(runtime, "constant", OpCode::Constant);

	// system variables
	create(runtime, "abortWord", OpCode::Constant);
	runtime->append(&runtime->abortWord);
	create(runtime, "dp", OpCode::Constant);
	runtime->append(&runtime->dictionaryPtr);
	create(runtime, "base", OpCode::Constant);
	runtime->append(&runtime->numberBase);
	DictionaryWord* tibAddrWord = create(runtime, "tibAddr", OpCode::Constant);
	runtime->append(&runtime->tibAddr);
	create(runtime, "tib", OpCode::Colon);
		runtime->append(tibAddrWord);
		runtime->append(atWord);
		runtime->append(semicolonWord);
	create(runtime, "#tib", OpCode::Constant);
	runtime->append(&runtime->tibContentLength);
	create(runtime, ">in", OpCode::Constant);
	runtime->append(&runtime->tibInputOffset);
	create(runtime, "lastWord", OpCode::Constant);
	runtime->append(&runtime->lastWord);
	create(runtime, "compilerFlags", OpCode::Constant);
	runtime->append(&runtime->compilerFlags);
}

std::string Builder::getNextInputWord(Runtime* runtime, char delimeter) {
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

DictionaryWord* Builder::create(Runtime* runtime, const std::string& name, OpCode opcode, Num flags) {
	DictionaryWord* result = (DictionaryWord*)runtime->allocate(sizeof(DictionaryWord));
	result->previous = runtime->lastWord;
	runtime->lastWord = result;
	result->flags = flags;
	result->opcode = opcode;
	CountedString::fromCString(name, result->name, sizeof(result->name));
}

	/*
		: interpret
		begin
			#tib @ >in @ =
			if
				tib 50 accept #tib ! 0 >in !
			then
			32 word find dup
			if
				state @ =
				if
					,
				else
					execute
				then
			else
				dup rot count >number
				if
					state @
					if
						last @ dup @ last ! dp !
					then
					abort
				then
				drop drop state @
				if
					['] lit , ,
				then
			then
		again
		;
*/
