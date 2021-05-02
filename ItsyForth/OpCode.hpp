#ifndef OpCode_hpp
#define OpCode_hpp

typedef enum OpCode {
	DoColon,
	DoSemicolon,
	DoLit,
	DoConstant,
	DoVariable,
	Comma,
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
	Create,
	Constant,
	
	ERROR
} OpCode;

#endif /* OpCode_hpp */
