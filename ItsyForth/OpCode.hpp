#ifndef OpCode_hpp
#define OpCode_hpp

#include <string>
class Runtime;
class DictionaryWord;

struct OpCode {
	static const int padLength = 256;

	typedef enum Code {
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
		SemiColon,
		Colon,
		Create,
		Constant
	} Code;
	
	OpCode(OpCode::Code op);
	operator std::string() const;
	std::string toString() const;
	
	void execute(Runtime* runtime, DictionaryWord* currentWord);

	OpCode::Code code;
};


#endif /* OpCode_hpp */
