#ifndef OpCode_hpp
#define OpCode_hpp

class Runtime;

struct OpCode {
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
		Colon,
		SemiColon,
		Create,
		SemiCode,
		Constant,
		Interpret
	} Code;
	
	OpCode(OpCode::Code op);
	
	void execute(Runtime* runtime);

	OpCode::Code code;
};


#endif /* OpCode_hpp */
