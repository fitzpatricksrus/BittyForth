//
//  OpCode.hpp
//  ItsyForth
//
//  Created by Dad on 4/17/21.
//

#ifndef OpCode_hpp
#define OpCode_hpp

class Runtime;

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
