//
//  OpCode.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef OpCode_hpp
#define OpCode_hpp

class Runtime;

static const int AbortVectorAddr = 0;					// where Abort jumps to
static const int DictionaryPointerAddr = sizeof(int);	// top of dictionary storage / pad
static const int NumberBaseAddress = sizeof(int) * 2;	// numeric base for string conversion
static const int TIBAddr = sizeof(int) * 3;				// address of the TIB
static const int ToInAddr = sizeof(int) * 4;			// next character in TIB to parse
static const int LastWord = sizeof(int) * 5;			// address of last word in dictionary

struct OpCode {
	enum code {
		DoColon,
		DoSemicolon,
		DoConstant,
		DoVariable,
		Abort,
		Comma,
		Lit,
		Rot,
		Drop,
		Dup,
		Swap,
		Plus,
		Equals,
		At,
		Put,
		ZeroBranch,
		Branch,
		Execute,
		Exit,
		Count,
		ToNumber,
		Accept,
		Word,
		Emit,
		Find,
		Colon,
		SemiColon,
		Create,
		SemiCode,
		Constant,
		Interpret
	};
	
	OpCode(int op);
	
	void execute(Runtime* runtime);

	int code;
};


#endif /* OpCode_hpp */
